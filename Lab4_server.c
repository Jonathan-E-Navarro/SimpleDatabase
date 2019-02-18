#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
	//create simple database by creating parallel arrays 
	//if we get email[i] return name[i]
	char* email[3];  // Make 5 strings
  	email[0] = "harris@ics.uci.edu\n";
  	email[1] = "joe@cnn.com\n";
 	email[2] = "jane@slashdot.org\n";
 	char* name[3];
	name[0] = "Ian G. Harris";
  	name[1] = "Joe Smith";
 	name[2] = "Jane Smith";
 	//printf("done with database\n");

 	//create socket
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	char *write_buffer = (char*) malloc(256 * sizeof(char));
	char *start = write_buffer;
	char *end   = &write_buffer[255];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	sockfd = socket(AF_INET, SOCK_STREAM,0);
	bzero((char*) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	bind(sockfd,(struct sockaddr*) &serv_addr, sizeof(serv_addr));
	printf("\n");
   	printf("address server started\n");

	while(1)
	{
		//wait for a connection
		listen(sockfd,5);
	
		clilen = sizeof(cli_addr);
		//accept
   		newsockfd = accept(sockfd,(struct sockaddr*) &cli_addr, &clilen);
   		//n != -1 to check if socket closed

		while(1)
		{
			//printf("connected\n");
			//read
			bzero(buffer,256);
			n = read(newsockfd, buffer, 255);
			int valid = 0;
			// printf("Here is the message %s",buffer);
			// printf("buffer[0] is %d\n",buffer[0] );
			//n = write(newsockfd,"I got your message",18);
			// printf("buffer+1 %s\n",buffer+1 );
			// printf("strlen of buffer: %lu\n",strlen(buffer));
			// printf("strlen of buffer+1: %lu\n",strlen(buffer+1));
			// printf("strlen of email[0] %lu\n" ,strlen(email[0]));

			if(strcmp(buffer+1,"+++\n") == 0)
			{
				//printf("exiting\n");
				break;
			}
			int i;
			for(i = 0; i < 3; i++)
			{
				//printf("%s\n",email[i] );
				if(strcmp(buffer+1,email[i]) == 0)
				{
					//printf("%s\n", name[i]);
					char *p = start;
					unsigned char tempchar = (unsigned char) (strlen(name[i]));
					*p = tempchar;
					strcpy((p+1),name[i]);
					printf("%s",email[i]);
					//printf("write_buffer[0]: %d\n", write_buffer[0]);
					//printf("write_buffer+1: %s\n", write_buffer+1);
					//n = send(newsockfd, name[i], strlen(name[i]), 0);
					n = send(newsockfd, write_buffer, strlen(write_buffer), 0);

					valid = 1;
				}
					
			}
			if(valid == 0)
				n = send(newsockfd, " unknown", 8, 0);
			valid = 0;

		}
		close(newsockfd);
	}
	
	//close(sockfd);

	return 0;

}	