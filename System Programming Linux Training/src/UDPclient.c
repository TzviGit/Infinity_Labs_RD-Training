// // Server side implementation of UDP client-server model
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>

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

// #include "sockets.h"

// #define PORT	 8080
// #define MAXLINE 1024
// #define MYPORT "4950"
// // Driver code





// void *get_in_addr(struct sockaddr *sa)
// {
// 	if (sa->sa_family == AF_INET) {
// 		return &(((struct sockaddr_in*)sa)->sin_addr);
// 	}

// 	return &(((struct sockaddr_in6*)sa)->sin6_addr);
// }

// int main() 
// {
//     socklen_t addr_len;
//     char s[INET6_ADDRSTRLEN];
//     int sock_fd;
//     char buffer[MAXLINE];
//     char hello[] = "Hello from server";
// 	// struct sockaddr_in cliaddr;
//     struct sockaddr_storage their_addr;
//     struct addrinfo hints, *servinfo, *p;
//     memset(&hints, 0, sizeof hints);
//     hints.ai_family = AF_INET; // set to AF_INET to use IPv4
// 	hints.ai_socktype = SOCK_DGRAM;
// 	hints.ai_flags = AI_PASSIVE; // use my IP

// 	if ((getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		
// 		return 1;
// 	}
// printf("getaddrinfo: %s\n", servinfo->ai_addr);
//     // loop through all the results and bind to the first we can
// 	for(p = servinfo; p != NULL; p = p->ai_next) {
// 		if ((sock_fd = socket(p->ai_family, p->ai_socktype,
// 				p->ai_protocol)) == -1) {
// 			perror("listener: socket");
// 			continue;
// 		}

// 		if (bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
// 			close(sock_fd);
// 			perror("listener: bind");
// 			continue;
// 		}

// 		break;
// 	}

// 	if (p == NULL) {
// 		fprintf(stderr, "listener: failed to bind socket\n");
// 		return 2;
// 	}

// 	freeaddrinfo(servinfo);
//     int numbytes;
//     addr_len = sizeof their_addr;
// 	if ((numbytes = recvfrom(sock_fd, buffer, MAXLINE-1 , 0,
// 		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
// 		perror("recvfrom");
// 		exit(1);
// 	}

// 	printf("listener: got packet from %s\n",
// 		inet_ntop(their_addr.ss_family,
// 			get_in_addr((struct sockaddr *)&their_addr),
// 			s, sizeof s));
// 	printf("listener: packet is %d bytes long\n", numbytes);
// 	buffer[numbytes] = '\0';
// 	printf("listener: packet contains \"%s\"\n", buffer);

// 	close(sock_fd);

// //     sock_fd = OpenSocket(AF_INET, SOCK_DGRAM, 0);

// //    BindSocket(sock_fd, AF_INET, 0,0);

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
    
// //     sendto(sock_fd, (const char *)"PING", strlen(hello),
// // 		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
// // 			len);
	
    
// //     n = recvfrom(sock_fd, (char *)buffer, MAXLINE,
// // 				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
// // 				(socklen_t *)&len);
// // 	buffer[n] = '\0';
// // 	printf("Server Sent : %s\n", buffer);
	
// // 	//printf("Hello message sent.\n");

// //     close(sock_fd);
	
// 	return 0;
// }


// // int OpenSocket(int __domain, int __type, int __protocol)
// // {
// //     int sock_fd = 0;
// //     if ((sock_fd = socket(__domain, __type, __protocol)) < 0)
// //     {
// // 		ReportAndHandle("socket creation failed", TERMINATE);
// // 	}

// //     return sock_fd;
// // }

// // void BindSocket(int sock_fd, int family, int adrr, int port)
// // {
// //     struct sockaddr_in servaddr;
// //     memset(&servaddr, 0, sizeof(servaddr));

// //     // Filling server information
// // 	servaddr.sin_family = AF_INET; // IPv4
// // 	servaddr.sin_addr.s_addr = INADDR_ANY;
// // 	servaddr.sin_port = htons(PORT);
	
// // 	// Bind the socket with the server address
// // 	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
// // 			sizeof(servaddr)) < 0 )
// // 	{
// //         ReportAndHandle("bind failed", TERMINATE);
// // 	}
// // }

// Client side implementation of UDP client-server model
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
	char hello[] = "Hello from client -- PONG";
	struct sockaddr_in	 servaddr;

	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

    int n;

    socklen_t len;

    for (int i = 0; i < 10; ++i)
    {
        sendto(sockfd, (const char *)hello, strlen(hello),
        MSG_CONFIRM, (const struct sockaddr *)&servaddr,
        sizeof(servaddr));
        //printf("Hello message sent.\n");
            
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);
    }

	close(sockfd);
	return 0;
}
