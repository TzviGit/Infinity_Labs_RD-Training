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
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &broadcast, sizeof(broadcast));

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(socket_fd, (const sockaddr *)&server, sizeof(server));

    char buf_in[100] = {'\0'};
    struct sockaddr_in from;
    socklen_t from_len = sizeof(struct sockaddr_in);

    recvfrom(socket_fd, buf_in, 100, 0, NULL, NULL);
    printf("%s\n", buf_in);

    return 0;
}