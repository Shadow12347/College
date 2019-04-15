#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#define LIMIT 1024

int get_command(char * command, char * envp[])
{
	char * arr[LIMIT];

	int count = 0;
	
	char temp[LIMIT];
	
	if (*command != 0)
		strcpy(temp, command);	
	else
	{
		printf("Please enter a command to run: ");
		fgets(temp, LIMIT, stdin);
	}

	char * token;
		

	token = strtok(temp, " \r\n\t");

	while (token != 0)
	{
		//printf("%s\n", token);	
		arr[count] = token;	
		token = strtok(0, " \r\n\t");
		count++;
	}
	arr[count] = (char *)NULL;
	
	execvpe(arr[0], arr, envp);
	perror("execvpe");	
}


int main (int argc, char * argv[], char * envp[])
{
	if (argc > 1)
		get_command(argv[1],envp);
	else
		get_command("\0", envp);	
	return 0;

}
