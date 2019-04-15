#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 64576

#define h_addr h_addr_list[0]

int main(int argc, char * argv[])
{
	int sockfd, numbytes;

	char buf[300];

	struct hostent *he;

	struct sockaddr_in their_addr;

	if (argc <= 2)
	{
		perror("not enough arguments\n");
		exit(1);
	}


	if((he=gethostbyname(argv[1])) == NULL)
	{
		perror("gethostbyname\n");
		exit(1);
	}
	else
		printf("host name to connect to is: %s\n", argv[1]);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	else
		printf("socket is OK :D\n");

	their_addr.sin_family = AF_INET;

	printf("Attempting to connect to: %s on port %d\n", argv[1], PORT);
	
	their_addr.sin_port = htons(PORT);

	their_addr.sin_addr = *((struct in_addr *)he->h_addr);

	memset(&(their_addr.sin_zero), '\0', 8);

	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect\n");
		exit(1);
	}
	else
		printf("connection SUCCESS :D\n");
	
	char output[4096];
	for (int i = 2; i != argc; ++i)
	{
		write(sockfd, argv[i], strlen(argv[i]));
		write(sockfd, " ", strlen(" "));
	}
	for (int t = 2; t != argc; ++t)
	{
		read(sockfd, output, 4096);
		printf("%s\n", output);
		memset(output, '\0', 4096);
	}

	
	printf("Closing connection\n");
	close(sockfd);
	

	return 0;
}
