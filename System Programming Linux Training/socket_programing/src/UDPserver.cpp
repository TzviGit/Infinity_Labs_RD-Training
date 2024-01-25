#include "UDPsocket.hpp"

#include <stdio.h>
//#include <stdlib.h>
// #include <unistd.h>
// #include <string>
// #include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
 #include <arpa/inet.h>
#include <netinet/in.h>


static const int PORT = 8000;


int main()
{
    UDPsocket server(AF_INET, INADDR_ANY, PORT);

    sockaddr_in clientadr;
    socklen_t len = sizeof(clientadr);

    std::string buffer;
    int n = server.RecMSG(buffer, MSG_WAITALL, ( sockadrr_in *) &clientadr, &len);
    buffer[n] = '\0';

    printf("Client : %s\n", buffer.c_str());

    std::string messagebuf("PONG - from server");
    server.SendMSG(messagebuf, MSG_CONFIRM, (sockadrr_in *)&clientadr);
   
    return 0;
}