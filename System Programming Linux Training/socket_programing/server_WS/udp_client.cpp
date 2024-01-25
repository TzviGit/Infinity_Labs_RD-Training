// UDP client program
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring> //memset
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> //net
#include <unistd.h> //close

#define PORT 5000

void StartChatWithServer(int sockfd, struct sockaddr_in servaddr);

int main()
{
    // Creating socket file descriptor
    int sockfd = 0;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket creation failed");
        exit(0);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
 
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    StartChatWithServer(sockfd,servaddr);

    return 0;
}

void StartChatWithServer(int sockfd, struct sockaddr_in servaddr)
{
    socklen_t len = 0;
    char send_m[50] = "udp ping";
    size_t count = 6;
    while(count--)
    {
        char buff[50] = {0};
        sendto(sockfd, send_m, strlen(send_m), 0,
        (const struct sockaddr *)&servaddr, sizeof(servaddr));
        printf("log:: udp- message sent\n");      

        len = sizeof(servaddr);
        recvfrom(sockfd, buff, sizeof(buff), 0,
        (struct sockaddr *) &servaddr, &len);
        printf("%s\n", buff);
        printf("log:: udp- message recieved\n");      

        sleep(3);
    }
    close(sockfd);
}
