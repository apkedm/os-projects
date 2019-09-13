# Programming Assignment 1
Author: Rudy Hill
Email: rudyghill@gmail.com

## Building
Building requires that each file go in their respective directories to be compiled as a default Linux kernel.
- ``arch/x86/kernel/cs3753_add.c``: add two numbers and return the result as a user level pointer
- ``arch/x86/kernel/Makefile``: instructions on how to build the kernel including the added source
- ``arch/x86/entry/syscalls/syscall_64.tbl``: system call table with two new calls at 333 and 334
- ``include/linux/syscalls.h``: prototype functions of the new system calls

Additionally, this build requires a system call at 333 that requires the ``helloworld.c`` source in ``arch/x86/kernel/`` as described in the Assignment document.

## Testing
To compile the test program run:
``` sh
gcc test.c -o test
./test
```
If the build was successful you should see "The result is 9"

You can see the generated ``printk`` messages by running ``dmesg`` or by examining ``/var/log/syslog`` which I have attached my own in this repository.
