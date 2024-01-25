// Server program
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring> //memset
#include <unistd.h> //STDIN_FILENO
#include <iostream> //cin

typedef enum
{
    CONTINUE,
    TERMINATE
}hanlde_t;


void HandleError(const char *msg, hanlde_t handler)
{
    perror(msg);
    if (TERMINATE == handler)
    {
        exit(-1); /* failure */
    }
}

#define PORT_BROAD 6000
#define PORT_UDP 5000
#define PORT_TCP 5500
#define MAXLINE 1024

int SetUpUdpSocket(int port);
int SetUpUDPBroadcast(int port);
int SetUpTCPListner(int port, int backlog);
void StartChatTCP(int *connfd, fd_set rset);
void StartChatUDP(int udpfd);
void StartChatSTDIN();


int max(int fd1, int fd2)
{
	return ((fd1 > fd2) ? fd1 : fd2);
}

int main()
{
    /* create listening TCP socket */
    int listenfd = SetUpTCPListner(PORT_TCP, 10);
    

	/* create UDP socket */
    int udpfd = SetUpUdpSocket(PORT_UDP);

    /* create UDP broadcast socket */
    int broadfd = SetUpUDPBroadcast(PORT_BROAD);
    

	// clear the descriptor set
    fd_set rset;
	FD_ZERO(&rset);

    
	int connfd = -1;
	for (;;) 
	{
		// get maxfd
		int maxfdp1 = max(max(listenfd, udpfd), broadfd);

		//maxfdp1 = max(maxfdp1, stdinfd);

		// set listenfd and udpfd in readset
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);
		FD_SET(broadfd, &rset);
		FD_SET(STDIN_FILENO, &rset);


        //set timeval to 7 sec
        struct timeval time;
        time.tv_sec = 7;
		time.tv_usec = 0;

		//check if there is a connfd waiting
		if(connfd != -1)
		{
			FD_SET(connfd, &rset);
			maxfdp1 = max(connfd, maxfdp1);

		}

		// select the ready descriptor
		int nready = select(maxfdp1 + 1, &rset, NULL, NULL, &time);
		if (nready == -1)
		{
			printf("error in nready\n");
		}
		else if (nready)
		{
			// if tcp socket is readable then handle it by accepting the connection
            if (FD_ISSET(connfd, &rset))
			{
				StartChatTCP(&connfd, rset);
			}
			else if (FD_ISSET(listenfd, &rset))
			{
				if (-1 == connfd)
				{
					struct sockaddr_in cliaddr;
					socklen_t len = sizeof(cliaddr);
					connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
					printf("Log: TCP Connection Accepted\n");
				}
			}
			// if any udp socket is readable receive the message.
			else if (FD_ISSET(udpfd, &rset))
			{   
				StartChatUDP(udpfd);
			
			} 
			else if(FD_ISSET(broadfd, &rset))
            {
                StartChatUDP(broadfd);
            }
			
			// if standart input is readable receive the message.
			else if(FD_ISSET(STDIN_FILENO, &rset))
			{
				StartChatSTDIN();
			}
		}
		else
		{
			printf("log: 7 sec passed\n");
		}
	}
}

void StartChatSTDIN()
{
	char buf[50] = {0};
	std::cin >> buf;
	
    if(0 == strcmp(buf, "quit"))
    {
        exit(0);
    }

    if(0 == strcmp(buf, "ping"))
	{
		std::cout << "pong" << std::endl;
	}
}

void StartChatUDP(int udpfd)
{
	struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    char send_m[50] = "pong\n";
    char buff[50] = {0};
    
	recvfrom(udpfd, (char *)buff, sizeof(buff), 0, (struct sockaddr *)&cliaddr, &len); 
	
	if(0 == (strcmp(buff, "quit")))
	{
		exit(0);
	}
    printf("%s\n", buff);

	sendto(udpfd, send_m, strlen(send_m), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len); 
}

void StartChatTCP(int *connfd, fd_set rset)
{
    char buff[50] = {0};
    char send_m[50] = "pong\n";

	int status = recv(*connfd, buff, sizeof(buff), 0);
	if (0 == status)
	{
		FD_CLR(*connfd, &rset);
        close(*connfd);
        *connfd = -1;
		printf("log: tcp client dies\n");
	}
	// print buffer which contains the client contents
	printf("%s\n", buff);

	// and send that buffer to client
	if (-1 == send(*connfd,(const void *)send_m, strlen(send_m), 0))
	{
		perror("server: failed in send\n");
	}
}


int SetUpTCPListner(int port, int backlog)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr_tcp;
	memset(&servaddr_tcp, 0, sizeof(servaddr_tcp));

	servaddr_tcp.sin_family = AF_INET;
	servaddr_tcp.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr_tcp.sin_port = htons(port);

	// binding server addr structure to listenfd
	bind(listenfd, (struct sockaddr*)&servaddr_tcp, sizeof(servaddr_tcp));
	listen(listenfd, backlog); //max of 10 requests in the queue

    return listenfd;
}


int SetUpUdpSocket(int port)
{
    struct sockaddr_in servaddr_udp;
	memset(&servaddr_udp, 0, sizeof(servaddr_udp));
	servaddr_udp.sin_family = AF_INET;
	servaddr_udp.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr_udp.sin_port = htons(port);

	/* create UDP socket */
	int udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	// binding server addr structure to udp sockfd
	bind(udpfd, (struct sockaddr*)&servaddr_udp, sizeof(servaddr_udp));

    return udpfd;
}

int SetUpUDPBroadcast(int port)
{
    struct sockaddr_in servaddr_udp;
	memset(&servaddr_udp, 0, sizeof(servaddr_udp));
	servaddr_udp.sin_family = AF_INET;
	servaddr_udp.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr_udp.sin_port = htons(port);

    /* create UDP broadcast socket */
	int broadfd = socket(AF_INET, SOCK_DGRAM, 0);
	bind(broadfd, (struct sockaddr*)&servaddr_udp, sizeof(servaddr_udp));
	int broadcast = 1;
    if(-1 == (setsockopt(broadfd, SOL_SOCKET,
			  SO_BROADCAST | SO_REUSEPORT, &broadcast, sizeof(broadcast))))
    {
        printf("errno is %s\n", strerror(errno));
        perror("error in setsockopt: ");
        return -1;
    }

    return broadfd;
}
