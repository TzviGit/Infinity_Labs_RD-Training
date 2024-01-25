// TCP Client program
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> //inet address
#include <unistd.h> //close

#define PORT 5500

void StartChatWithServer(int sickfd);

int main()
{
    // Creating socket file descriptor
    int sockfd = 0;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("socket creation failed");
        exit(0);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
 
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
    if (connect(sockfd, (struct sockaddr*)&servaddr,
                            sizeof(servaddr)) == -1)
    {
        printf("\n Error : Connect Failed \n");
    }
 
    StartChatWithServer(sockfd);
}

void StartChatWithServer(int sockfd)
{
    char send_m[50] = "tcp ping";
	int count = 6;

	while (count--) 
    {
        char buff[50] = {0};
        if (-1 == send(sockfd, (const void *)send_m, strlen(send_m), 0))
        {
            perror("client: failed in send\n");
        }
        printf("log:: tcp- message sent\n");        
        recv(sockfd, buff, sizeof(buff), 0);

		printf("%s", buff);
        printf("log:: tcp- message recived\n");      
        sleep(3);
	}
    close(sockfd);
}
