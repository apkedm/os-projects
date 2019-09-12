#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main(){
	int err1 = syscall(333);
	int a = 4;
	int b = 5;
	int result;
	int err2 = syscall(334, a, b, &result);
	printf("The result is %d\n", result);
	return err1 & err2;
}
