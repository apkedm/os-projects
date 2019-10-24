#include "multi-lookup.h"

/***************************************************************************************************************************************************************
REFERENCE:
char * strcpy ( char * destination, const char * source );

char *fgets(char *str, int n, FILE *stream)

long int strtol (const char* str, char** endptr, int base);

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

int pthread_join(pthread_t thread, void **retval);

int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);

int dnslookup(const char* hostname, char* firstIPstr, int maxSize); (from util.c)
****************************************************************************************************************************************************************/

/***************************************************************************************************************************************************************
Function: requester
Inputs: takes struct containing file descriptor and mutexes
Outputs: N/A
Goal:These threads service a set of name files, each of which contains a list of domain names.
Each name that is read from each of the files is placed on a shared array.
****************************************************************************************************************************************************************/

char* arr[25];
void *requester(void *inputStruct){

  struct globStruct *requestS = inputStruct; //create a struct pointer that points to glbs, will give access to parameters

  for (int i = 0; i < requestS->var.fileCount; i++) { //iterate from 0 to the amount of input files we have

    if(!pthread_mutex_trylock(&requestS->mut.lock[i])) { //if successful, pthread_mutex_trylock returns 0, !0 = 1 -> thread now owns lock

      char fileTobeServiced[20]; //create an array to store the files to be serviced

      strcpy(fileTobeServiced,(requestS->array->fileArray + (i*20))); // copy the string

      //http://man7.org/linux/man-pages/man2/gettid.2.html
      pid_t gettid = syscall(SYS_gettid); //gets Thread ID for the service print

      /* Write to service.txt, need to lock */
      pthread_mutex_lock(&requestS->mut.serviceLock); //LOCK to write to service.txt
      FILE *service = fopen(requestS->var.textServ, "a"); //open service.txt and append

      /*Error Handler */
      if (service == NULL) {
        printf("Error opening file!\n");
        exit(1);
      }
      fprintf(service, "thread: %d is servicing %s\n", gettid, fileTobeServiced);
      fclose(service);
      pthread_mutex_unlock(&requestS->mut.serviceLock); //Unlock


      FILE* inputFile = fopen(fileTobeServiced, "r"); //read from file

      /* Error Handler */
      if(!inputFile) {
        printf("Error: invalid Input File\n");
      }

      //char *fgets(char *str, int n, FILE *stream)
      char domainName[20];
      /* Getting the domain name from the file and storing it in the shared buffer */
      while(fgets(domainName, sizeof(domainName), inputFile)) { //obtain domain
        domainName[strlen(domainName)-1] = '\0';
        strcpy(requestS->var.sharedBuffer[requestS->var.bufferIndex], domainName); //store into buffer
        requestS->var.bufferIndex++; //increment the buffer index to get to the next spot
      }
      pthread_mutex_destroy(&requestS->mut.lock[i]);
    }
  }
  return 0;
}

/***************************************************************************************************************************************************************
Function: resolver
Inputs: takes struct containing file descriptor and mutexes
Outputs: N/A
Goal:The resolver thread pool services the shared array by taking a name off the array and querying its IP address.
After the name has been mapped to an IP address, the output is written to a line in the results.txt file
If a resolver thread tries to read from the array but finds that it is empty, it should block until there is a
new item in the array or all names in the input files have been serviced.
*****************************************************************************************************************************************************************/
void *resolver(void *inputStruct){
  struct globStruct *requestS = inputStruct;

  if(!pthread_mutex_trylock(&requestS->mut.sharedBufferLock)) { //if successful, pthread_mutex_trylock returns 0, !0 = 1 -> thread now owns lock
    for(int i = 0; i < requestS->var.bufferIndex; i++) { //iterate through each line in the list

      char domain[20]; //initalize an empty "string" for the domain name
      char ip[20]; //initialize an empty "string" for the IP

      strcpy(domain, requestS->var.sharedBuffer[i]); //fill the empty domain string with the domain name
      printf("%s, ", domain); //print domain


      int invHost = 0; //for invalid hostnames
      if (dnslookup(domain, ip, sizeof(ip))) { //lookup IP address
        fprintf(stderr, "INVALID HOSTNAME: %s\n", domain);
        invHost  = 1; //invalid hostname
        printf("\n");
      }
      else {
        printf("%s\n", ip); //print IP
      }

      /* Write to results.txt, requires a lock */
      pthread_mutex_lock(&requestS->mut.resultsLock); //LOCK results lock
      FILE *outStream = fopen(requestS->var.textRes, "a"); //open results.txt to append IP
      if (outStream == NULL) {
        printf("Error: invalid output file!\n");
        exit(1);
      }
      /* add some text */
      if (!invHost) { //invHost checks for invalid hostnames
        fprintf(outStream, "%s, %s\n", domain, ip);
      }
      else {
        fprintf(outStream, "%s,\n", domain);
      }
      fclose(outStream);
      pthread_mutex_unlock(&requestS->mut.resultsLock);//Unlock

    }
    pthread_mutex_destroy(&requestS->mut.sharedBufferLock); //destroy mutex lock
  }
}

/****************************************************************************************************************************************************************
Function: getTimeOfDay
Inputs: N/A
Outputs: milliseconds (long long)
Goal:These threads service a set of name files, each of which contains a list of domain names.
Each name that is read from each of the files is placed on a shared array.
********************************************************************************************************************************************************************/
long long getTimeOfDay(){

  // SOURCE: https://linux.die.net/man/2/gettimeofday
  struct timeval evaltime;
  /* gets the current time */
  gettimeofday(&evaltime, NULL); /* system call */
  /* calculate the miliseconds */
  long long milliseconds = evaltime.tv_sec*1000LL + evaltime.tv_usec/1000;
  /* return the milliseconds */
  return milliseconds;
}
/******************************************************************************************************************************************************************/
int main (int argc, char *argv[]) {
  /* Format: ./multi-lookup <# requester> <# resolver> <requester log> <resolver log> [<data files>......]     */

  for(int i; i<25; i++){
      char* arr[i] = char* malloc(25*sizeof(char));
  }

  long long startTime = getTimeOfDay(); //get the start time to later compute how long it took

  struct globStruct gbls; //instantiate a globStruct

  gbls.var.bufferIndex = 0; //set the buffer index to zero

  /* copy the name of the serviced file from command line argument */
  strcpy(gbls.var.textServ, argv[4]);

  /* copy the name of the results file from command line argument */
  strcpy(gbls.var.textRes, argv[3]);


  /* Requester Threads   */
  int numReqThreads = strtol(argv[1], NULL, 10); //number of request threads
  pthread_t requesterThreads[numReqThreads];
  struct threadData threadDataArray[numReqThreads];
  gbls.array = threadDataArray;

  /* Resolver threads */
  int numResThreads = strtol(argv[2],NULL,10); //store in the number of resolver threads from the command line argument
  pthread_t resolverThreads[numResThreads];
  char inputFileArray[20][20];
  int fileCount = argc-5; //subtract the total argument count by 5 to store the amount of input files we have
  gbls.var.fileCount = fileCount; //store in struct
  for(int i = 0; i < fileCount; i++){
    strcpy(inputFileArray[i], argv[i+5]);
  }

  /* Initialize mutex locks w/ DEFAULT attributes*/
  pthread_mutex_init(&gbls.mut.resultsLock,NULL);
  pthread_mutex_init(&gbls.mut.serviceLock,NULL);
  pthread_mutex_init(&gbls.mut.sharedBufferLock,NULL);
  for (int i = 0; i < 10; i++){
    pthread_mutex_init(&gbls.mut.lock[i],NULL);
  }

  //populate thread data and global files to service
  for (int i = 0; i <= numReqThreads; i++){
    threadDataArray[i].fileArray = &inputFileArray;
  }

  //create requester threads
  for (int i = 0; i < numReqThreads; i++){
    //int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
    pthread_create(&requesterThreads[i], NULL, requester, &gbls);
  }
  //join requester threads
  for (int i = 0; i < numReqThreads; i++){
    //int pthread_join(pthread_t thread, void **retval);
    pthread_join(requesterThreads[i],NULL);
  }

  //create resolver threads
  for (int i = 0; i < numResThreads; i++){
    pthread_create(&resolverThreads[i], NULL, resolver, &gbls);
  }
  //join resolver threads
  for (int i =0; i< numResThreads; i++){
    pthread_join(resolverThreads[i],NULL);
  }


  /*  Get the time when the process finished  */
  long long endTime = getTimeOfDay();


  printf("\nTime elapsed (in seconds): %d\n", (endTime-startTime)/1000); //print total time elasped
}
