# Lab: Named Pipes

A lab designed to do basic IPC using named pipes (FIFOs).

You might have noticed that the.

## Your Task

* Use the provided code to create two new programs `pgm1.c` and `pgm2.c` such that `pgm1` sends a random string to `pgm2`, and `pgm2` prints the random string to `stdout`. Note that when we run the pgm1 and pgm2, the corresponding processes will *not* have a parent-child relation, yet we will still be able to perform IPC, as we are using a named pipe. 
Be sure to run pgm1 and pgm2 on two separate terminal windows so their outputs don't interleave.

* Once you've done this, modify the provided code (`FIFO.c`) so that the pipe is not a named pipe, but a unnamed pipe (see `pipe()`). Note that we can actually use an unnamed pipe here due to the fact that we have a parent-child relationship between our processes.
