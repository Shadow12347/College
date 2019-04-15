#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int fork_switch(int pid, int times, char letter)
{
	switch (pid)
	{
		case -1:
			perror("fork");
			exit(1);
		case 0:
			for (int i = 0; i != times; i++)
			{
				putchar(letter);
				fflush(stdout);
			}	
			exit(0);	
		default:
			;
	}
}


int my_fork(int times)
{	
	char arr[] = {'A', 'B', 'C', 'D'};
	int waiting;
	int status;

	for (int i = 0; i != 4; i++)
	{
		fork_switch(fork(), times, arr[i]);
	}
	waiting = wait(&status);
	waiting = wait(&status);
	waiting = wait(&status);
	waiting = wait(&status);

	printf("\n");
}


int main (int argc, char * argv[])
{
	if (argc > 1)
		my_fork(atoi(argv[1]));
	else
		my_fork(10000);
	return 0;
}
