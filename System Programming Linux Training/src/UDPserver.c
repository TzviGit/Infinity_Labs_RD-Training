// // // Server side implementation of UDP client-server model
// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <unistd.h>
// // #include <string.h>
// // #include <sys/types.h>
// // #include <sys/socket.h>
// // #include <arpa/inet.h>
// // #include <netinet/in.h>

// // #include "sockets.h"

// // #define PORT	 8080
// // #define MAXLINE 1024

// // // Driver code
// // int main() 
// // {

// // 	char buffer[MAXLINE];
// // 	char message[] = "Hello from server --  PONG";
// // 	struct sockaddr_in cliaddr;
    
// //     int sock_fd = OpenSocket(AF_INET, SOCK_DGRAM, 0);

// //     BindSocket(sock_fd, AF_INET, INADDR_ANY, PORT);

// //     // // Creating socket file descriptor
// //     // if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
// //     // {
// //     // 	ReportAndHandle("socket creation failed", TERMINATE);
// //     // }

// //     // memset(&servaddr, 0, sizeof(servaddr));
// // 	// memset(&cliaddr, 0, sizeof(cliaddr));
	
// // 	// // Filling server information
// // 	// servaddr.sin_family = AF_INET; // IPv4
// // 	// servaddr.sin_addr.s_addr = INADDR_ANY;
// // 	// servaddr.sin_port = htons(PORT);
	
// // 	// // Bind the socket with the server address
// // 	// if ( bind(sockfd, (const struct sockaddr *)&servaddr,
// // 	// 		sizeof(servaddr)) < 0 )
// // 	// {
// //     //     ReportAndHandle("bind failed", TERMINATE);
// // 	// }
	
// // 	int len, n;

// // 	len = sizeof(cliaddr); //len is value/resuslt

// // 	n = recvfrom(sock_fd, (char *)buffer, MAXLINE,
// // 			MSG_WAITALL, ( struct sockaddr *) &cliaddr,(socklen_t *)&len);
	
// //     buffer[n] = '\0';
// // 	printf("Client sent: %s\n", buffer);

// // 	sendto(sock_fd, (const char *)message, strlen(message),
// // 		MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
	
    
// //     //printf("Hello message sent.\n");

// //     close(sock_fd);

// //     return 0;
// // }


// // // int OpenSocket(int __domain, int __type, int __protocol)
// // // {
// // //     int sock_fd = 0;
// // //     if ((sock_fd = socket(__domain, __type, __protocol)) < 0)
// // //     {
// // // 		ReportAndHandle("socket creation failed", TERMINATE);
// // // 	}

// // //     return sock_fd;
// // // }

// // // void BindSocket(int sock_fd, int family, int adrr, int port)
// // // {
// // //     struct sockaddr_in servaddr;
// // //     memset(&servaddr, 0, sizeof(servaddr));

// // //     // Filling server information
// // // 	servaddr.sin_family = AF_INET; // IPv4
// // // 	servaddr.sin_addr.s_addr = INADDR_ANY;
// // // 	servaddr.sin_port = htons(PORT);
	
// // // 	// Bind the socket with the server address
// // // 	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
// // // 			sizeof(servaddr)) < 0 )
// // // 	{
// // //         ReportAndHandle("bind failed", TERMINATE);
// // // 	}
// // // }

// /*
// ** talker.c -- a datagram "client" demo
// */

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <netdb.h>

// #define SERVERPORT "4950"	// the port users will be connecting to

// int main(int argc, char *argv[])
// {
// 	int sockfd;
// 	struct addrinfo hints, *servinfo, *p;
// 	int rv;
// 	int numbytes;

// 	if (argc != 3) {
// 		fprintf(stderr,"usage: talker hostname message\n");
// 		exit(1);
// 	}

// 	memset(&hints, 0, sizeof hints);
// 	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
// 	hints.ai_socktype = SOCK_DGRAM;

// 	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
// 		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
// 		return 1;
// 	}

// 	// loop through all the results and make a socket
// 	for(p = servinfo; p != NULL; p = p->ai_next) {
// 		if ((sockfd = socket(p->ai_family, p->ai_socktype,
// 				p->ai_protocol)) == -1) {
// 			perror("talker: socket");
// 			continue;
// 		}

// 		break;
// 	}

// 	if (p == NULL) {
// 		fprintf(stderr, "talker: failed to create socket\n");
// 		return 2;
// 	}

// 	if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
// 			 p->ai_addr, p->ai_addrlen)) == -1) {
// 		perror("talker: sendto");
// 		exit(1);
// 	}

// 	freeaddrinfo(servinfo);

// 	printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
// 	close(sockfd);

// 	return 0;
// }

// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080
#define MAXLINE 1024

// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];
	char hello[] = "Hello from server --- PING";
	struct sockaddr_in servaddr, cliaddr;
	
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	int  n;

	socklen_t len = sizeof(cliaddr); //len is value/resuslt

    for (int i = 0; i < 10; ++i)
    {
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *)&cliaddr,
                     &len);
	buffer[n] = '\0';
	printf("Client : %s\n", buffer);
	sendto(sockfd, (const char *)hello, strlen(hello),
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
			len);
	//printf("Hello message sent.\n");
    }

    close(sockfd);
    return 0;
}
