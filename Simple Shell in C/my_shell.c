#define _GUN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char ** argv, char ** envp)
{
	int pid, w, status;
	size_t l = 0;
	ssize_t len;
	char * input = NULL;
	char * tok;

	printf("$(BASH) my_shell\n$ ");
	fflush(stdout);
	
	len = getline(&input, &l, stdin);
	while (len > 0)
	{
		if ((pid = fork()) == -1)
		{
			perror("fork");
			exit(1);
		}
		else if (pid == 0)
		{
			if (len < 2)
			{
				exit(0);
			}

			char * all_toks[1024];
			int num_toks = 0;

			memset(all_toks, 0, 1024);

			tok = strtok(input, " \n");
			while(tok != NULL)
			{
				all_toks[num_toks] = tok;
				tok = strtok(NULL, " \n");
				num_toks++;
			}
			all_toks[num_toks] = NULL;

			execvp(all_toks[0], all_toks);
			printf("-execvp:");
			fflush(stdout);
			for (int i = 0; i != num_toks; i++)
			{
				printf(" %s", all_toks[i]);
			}
			fflush(stdout);
			printf(": ");
			fflush(stdout);
			perror("");
			exit(1);
		}
		else
		{
			w = wait(&status);
		}
		printf("$ ");
		fflush(stdout);
		len = getline(&input, &l, stdin);
	}
	printf("^D\n$(BASH)\n");
}
