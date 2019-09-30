#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main(){
	int err1 = syscall(333);
	int result;
	int err2 = syscall(334, 4, 5, &result);
	printf("The result is %d\n", result);
	return err1 & err2;
}
