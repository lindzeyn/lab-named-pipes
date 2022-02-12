#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>


#define STR_LEN	10

// generates a random string of a specified size
static void randstring(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
}

int main(void) {

	// seed rand()
	srand(time(NULL));

	// create a named pipe
	char* fifopipe = "/tmp/myfifo";
	mkfifo(fifopipe, 0666);

	pid_t pid;
	pid = fork();

	// child
	if (pid == 0){

		// create a random string
		char mystring[STR_LEN];
		randstring(mystring, STR_LEN);

		printf("child is going to sleep for two seconds.\n");
		sleep(2);

		// opening the pipe for write only
		int fd = open(fifopipe, O_WRONLY);
		printf("child opened the pipe and will now write the string \"%s\" to pipe one character at a time\n", mystring);
		fflush(stdout);

		// write to pipe one byte at a time
		for (int i = 0; i < STR_LEN; i++){
			sleep(1);
			write(fd, &mystring[i], 1);
		}

		// close(fd) places a EOF on the pipe, which will indicate to read() that there is nothing left to read.
		close(fd);
		printf("child finished, closing pipe\n");
		fflush(stdout);
	}
	// parent
	else {

		int rv;
		char buffer;

		// open pipe for read only
		fprintf(stderr,"parent is blocking on open()...\n");
		int fd = open(fifopipe, O_RDONLY);
		fprintf(stderr,"parent no longer blocking since child opened pipe for writing.\n");

		// read from the pipe one byte at a time
		fprintf(stderr,"parent received: ");
		while((rv = read(fd, &buffer, 1)) > 0){
			fprintf(stderr,"%c:%d ", buffer, rv);

			// uncomment the sleep() below to see that we can continue reading
			// from the pipe long after our child closes the write end.
			//sleep(2);
		}
		fprintf(stderr,"rv = %d since we hit the EOF",rv);
		fflush(stderr);
		close(fd);
	}

	/*
	 * YOUR TASK:
	 * use this code to create two programs: pgm1.c and pgm2.c such that pgm1 sends a random str to pgm2, and pgm2 prints it out char by char.
	 * note that when we run the pgm1 and pgm2, the corresponding processes will *not* have a parent-child relation, yet we will still be able
	 * to perform IPC, as we are using a named pipe. be sure to run pgm1 and pgm2 on two separate terminal windows so their outputs don't interleave.
	 *
	 * once you've done this, modify this code (FIFO.c) so that the pipe is not a named pipe, but rather, a unnamed pipe (see pipe()).
	 * note that we can actually use an unnamed pipe here due to the fact that we have a parent-child relationship between our processes.
	 */

	return EXIT_SUCCESS;
}
