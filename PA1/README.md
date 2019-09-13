# Programming Assignment 1
Author: Rudy Hill
Email: rudyghill@gmail.com

## Building
Building requires that each file go in their respective directories to be compiled as a default Linux kernel.
- ``arch/x86/kernel/cs3753_add.c``
- ``arch/x86/kernel/Makefile``
- ``arch/x86/entry/syscalls/syscall_64.tbl``
- ``include/linux/syscalls.h``

Additionally, this build requires a system call at 333 that requires the ``helloworld.c`` source in ``arch/x86/kernel/`` as described in the Assignment document.

## Testing
To compile the test program run:
``` sh
gcc test.c -o test
./test
```
If the build was successful you should see "The result is 9"

You can see the generated ``printk`` messages by running ``dmesg`` or by examining ``/var/log/syslog`` which I have attached my own in this repository.
