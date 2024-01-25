#include "UDPsocket.hpp"
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
 #include <arpa/inet.h>
#include <netinet/in.h>


static const int PORT = 8000;

int main()
{
    UDPsocket client(AF_INET);
    char buf[1000];
    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t len = sizeof(servaddr);

    std::string buffer("PING - from client");
    int n = client.SendMSG(buffer, MSG_CONFIRM, ( sockadrr_in *) &servaddr);
    
    std::string messagebuf("hello world");
    n = client.RecMSG(messagebuf, MSG_WAITALL, (sockadrr_in *)&servaddr, &len);
    buffer[n] = '\0';

    printf("server : %s\n", buffer.c_str());

    close(client);
    return 0;
}