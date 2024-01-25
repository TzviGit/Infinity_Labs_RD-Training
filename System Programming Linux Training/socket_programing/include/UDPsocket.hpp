


#ifndef __VJVJDAVF__PD__
#define __VJVJDAVF__PD__


#include <stdlib.h>
#include <unistd.h>
#include <string>
//#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


typedef enum
{
    CONTINUE,
    TERMINATE
}hanlde_t;

void ReportAndHandle(const char *msg, hanlde_t handler);

class UDPsocket
{

public:
    UDPsocket(int domain);
    UDPsocket(int domain, long serverAdrr, short port);
    ~UDPsocket();

    int SendMSG(std::string &buffer, int flags, struct sockadrr_in *sendTo);
    int RecMSG(std::string &buffer, int flags, struct sockadrr_in *senderAdrr,
               socklen_t *len);
   inline operator int() const 
   {
       return m_sockfd;
   }

private:
    int m_sockfd;

    struct sockaddr_in m_addr;
};

#endif