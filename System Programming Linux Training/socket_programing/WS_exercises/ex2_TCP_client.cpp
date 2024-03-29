#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
       
#define MAX 80
#define PORT 8888


void func(int sockfd)
{
	char buff[MAX] = {0};
    char send_m[10] = "ping\n";
	int count = 4;
	// infinite loop for chat
	while (count--) 
    {
		//write(sockfd, buff, sizeof(buff));
        if (-1 == send(sockfd, (const void *)send_m, 10, 0))
            {
                perror("server- failed in send\n");
            }

		bzero(buff, sizeof(buff));
		//read(sockfd, buff, sizeof(buff));
        
        recv(sockfd, buff, sizeof(buff), 0); //MSG_WAITALL -  block the sever (recipient) until package arrive, not for datagram

		printf("From Server : %s", buff);
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}
