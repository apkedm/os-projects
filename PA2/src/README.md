# Programmming Assignment 2

Author: Rudy Hill

Email: rudyghill@gmail.com

## Description
- ``simple_char_driver.c``: Device driver with read, write, seek, open, and close operations.

- ``test.c``: Test code using the file operations coded in the device driver. 

- ``Makefile``: Makefile for the device driver. Creates module object.

## Building
Build the LKM by running:
```
sudo make -C /lib/modules/$(uname -r)/build M=$PWD modules
```
Build the test program with:
```
gcc test.c -o test
```

## Testing

### Insert LKM

In the directory of ``simple_char_driver.ko``, run the following command:
```sh
sudo insmod simple_char_driver.ko
```
Use ``lsmod`` to check if it was inserted correctly.

### Create Device File

run the following line of code
``` sh
sudo mknod –m 777 /dev/simple_character_device c 240 0
```

Check using ``ls /dev`` to see if it was created.

### Run the Test code
Run
```sh
./test
```
Follow the prompts and enter e to exit the test.
