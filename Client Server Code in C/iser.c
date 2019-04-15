#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define PORT 64576

#define BACKLOG 10


int my_fork(int new_fd, char * buf, char * envp[])
{
	
	int waiting, status;

	char * token;
	char * arr[1024];
	arr[0] = "/bin/ls";
	arr[1] = "-l";

	token = strtok(buf, " \r\n");
	while(token != NULL)
	{	
		switch (status = fork())
		{
			case -1:
				perror("fork\n");
				exit(1);
			case 0:
				arr[2] = token;
				arr[3] = '\0';
				close(0);
				dup2(new_fd, 0);
				dup2(new_fd, 1);
				dup2(new_fd, 2);
				execvpe(arr[0], arr, envp); 
				exit(0);
			default:
				waiting = wait(&status);
		}
		token = strtok(NULL, " \r\n");
	}

	printf("\n");
	return(0);
}



int main(int argc, char * argv[], char * envp[])
{
	int sockfd, new_fd, n;

	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;

	int sin_size;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd == -1)
	{
		perror("socket\n");
		exit(1);
	}
	else
		printf("socket is OK :D\n");

	my_addr.sin_family = AF_INET;

	my_addr.sin_port = htons(PORT);

	my_addr.sin_addr.s_addr = INADDR_ANY;

	memset(&(my_addr.sin_zero), 0, 8);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind\n");
		exit(1);
	}
	else
		printf("bind is OK :D\n");

	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen\n");
		exit(1);
	}
	else
		printf("listen is OK :D\n");

	sin_size = sizeof(struct sockaddr_in);

	new_fd = accept(sockfd, (struct sockaddr*)&their_addr, (socklen_t *)&sin_size);

	if (new_fd == -1)
	{
		perror("accept\n");
		exit(1);
	}
	else
		printf("accept is OK :D\n");

	char buf[1024];
	
	int num;
	while ((num = read(new_fd, buf, 1024)) > 0)
	{
		my_fork(new_fd, buf, envp);
	}
	
	printf("Closing connection\n");
	close(new_fd);
	close(sockfd);
	return 0;
}








