#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>
#include <cstring>

int main()
{
    int server_port = 3490;                               
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    int broadcast = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    struct sockaddr_in reciever;
    memset(&reciever, 0, sizeof(reciever));

    reciever.sin_family = AF_INET;
    reciever.sin_port = htons(server_port);
    reciever.sin_addr.s_addr = INADDR_BROADCAST;

    char buf_out[] = "broadcast sent";
    sendto(socket_fd, buf_out, strlen(buf_out), 0,
           (const sockaddr *)&reciever, sizeof(reciever));

    return 0;
}