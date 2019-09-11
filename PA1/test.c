#include <unistd.h>
#include <sys/syscall.h>

int main(){
	int err = syscall(333);
	return err;
}
