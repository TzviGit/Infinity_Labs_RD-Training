#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "sockets.h"


void ReportAndHandle(const char *msg, hanlde_t handler)
{
    perror(msg);
    if (TERMINATE == handler)
    {
        exit(-1); /* failure */
    }
}

void BindSocket(int sock_fd, int family, int adrr, int port)
{
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
	servaddr.sin_family = family; // IPv4
	servaddr.sin_addr.s_addr = adrr;
	servaddr.sin_port = htons(port);
	
	// Bind the socket with the server address
	if ( bind(sock_fd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
        ReportAndHandle("bind failed", TERMINATE);
	}
}

int OpenSocket(int __domain, int __type, int __protocol)
{
    int sock_fd = 0;
    if ((sock_fd = socket(__domain, __type, __protocol)) < 0)
    {
		ReportAndHandle("socket creation failed", TERMINATE);
	}

    return sock_fd;
}





