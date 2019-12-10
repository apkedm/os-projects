# Programming Assignment 3

Author: Rudy Hill

Email: rudyghill@gmail.com

## Description

See assignment pdf for details.

UPDATE LATER.

## Building

To compile, run ``make``.

## Running

To run:

```
./multi-lookup <# requester> <# resolver> <requester log> <resolver log> [ <data file> ...]
```
If you compiled with make, the log files are initialized automatically and can therefore be used immediately.
You can run with something like this example:
```
./multi-lookup 3 3 req.log res.log input/*
```

### Test program
To run the test program run:
```
make performance > performance.txt

```
and inspect ``performance.txt``
