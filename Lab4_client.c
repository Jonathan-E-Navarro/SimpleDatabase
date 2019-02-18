#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// args: hostname, port 
int main(int argc, char const *argv[])
{
	//create socket
	int sockfd, port;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char read_buffer[256];
	//char write_buffer[256];
	char temp[256];
	char *write_buffer = (char*) malloc(256 * sizeof(char));
	char *start = write_buffer;
	char *end   = &write_buffer[255];

	printf("%s\n", argv[1]);
	port = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM,0);
	server = gethostbyname(argv[1]);
	printf("%d\n", port);
	if(server == NULL)
	{
		printf("host not found!\n");
		return 0;
	}
		
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	bcopy((char *) server->h_addr,
		(char *) &serv_addr.sin_addr.s_addr, 
		server-> h_length);

	//request connection
	connect(sockfd,(struct sockaddr*) &serv_addr, sizeof(serv_addr));
	printf("connected\n");
	while(strcmp(write_buffer+1,"+++\n") != 0)
	{
		//connected
		//write to server
		char *p = start;
		bzero(write_buffer,256);
		printf("> ");
		fgets(temp, 255, stdin);
		//printf("strlen(temp) %lu\n", strlen(temp));
		//printf("strlen(temp)-1 %lu\n", strlen(temp)-1);
		unsigned char tempchar = (unsigned char) (strlen(temp)-1);
		*p = tempchar;
		strcpy((p+1),temp);

		//printf("*p is %d\n",(unsigned char) *p );
		
		write(sockfd,write_buffer, strlen(write_buffer));
		bzero(read_buffer,256);
		read(sockfd, read_buffer, 256);
		printf("%s\n",read_buffer+1 );
		
	}
	close(sockfd);
	return 0;
}