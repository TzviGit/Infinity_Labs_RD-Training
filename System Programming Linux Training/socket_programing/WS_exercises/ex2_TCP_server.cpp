#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 80
#define PORT 8888

// Function designed for chat between client and server.
void func(int connfd)
{
	char buff[MAX];
      char send_m[10] = "pong\n";
	int count = 4;
	// infinite loop for chat
	while (count--) 
    {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
        recv(connfd, buff, sizeof(buff), 0); //MSG_WAITALL -  block the sever (recipient) until package arrive, not for datagram

		// print buffer which contains the client contents
		printf("From client: %s\n", buff);

		// and send that buffer to client
        if (-1 == send(connfd,(const void *)send_m, 10, 0))
        {
            perror("server- failed in send\n");
        }
	}
}

// Driver function
int main()
{
	int sockfd, connfd;
    socklen_t len;
  
	struct sockaddr_in servaddr, cli;

	// socket create and verification
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) == -1) {
		perror("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) == -1) {
		perror("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	if (connfd < 0)
	{
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}
