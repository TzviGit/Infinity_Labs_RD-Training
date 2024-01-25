// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
   
#define PORT 6000
#define MAXLINE 1024

void StartChatWithServer(int sockfd, struct sockaddr_in servaddr);
// broad client code
int main() 
{
    int sockfd;
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    int broadcast = 1;
    if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,
        &broadcast, sizeof(broadcast)))    
    {
        printf("errno is %d\n", errno);
        perror("error in setsockopt: ");
        return -1;
    }

    // Filling server information
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    StartChatWithServer(sockfd, servaddr);

    return 0;
}

void StartChatWithServer(int sockfd, struct sockaddr_in servaddr)
{
    socklen_t len = 0;
    char buff[50] = {0};
    char send_m[50] = "broad ping\n";
    size_t count = 6;
    
    while(count--)
    {
        sendto(sockfd, send_m, strlen(send_m), 0,
        (const struct sockaddr *)&servaddr, sizeof(servaddr));
            
        recvfrom(sockfd, buff, sizeof(buff), 0,
        (struct sockaddr *) &servaddr, &len);
        printf("%s\n", buff);
        sleep(3);
    }
    close(sockfd);
}
