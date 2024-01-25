 #include <stdio.h>
 #include <cstring>
//#include <stdlib.h>
// #include <unistd.h>
// #include <string>
// #include <cstring>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>

#include "UDPsocket.hpp"

// typedef enum
// {
//     CONTINUE,
//     TERMINATE
// }hanlde_t;

//void ReportAndHandle(const char *msg, hanlde_t handler);
static void PrepareAdrress(struct sockaddr_in *addr, int domain, long Adrr, short port);
static int CreateUDPSocket(int domain);

static const std::size_t MAX_MSG = 1000;

// enum
// {
//     BIND,
//     NO_BIND
// } bind_option_t;



// class UDPsocket
// {

// public:
//     UDPsocket(int domain);
//     UDPsocket(int domain, long serverAdrr, short port);
//     ~UDPsocket();

//     int SendMSG(std::string buffer, int flags, struct sockadrr_in *sendTo);
//     int RecMSG(std::string &buffer, std::size_t nbytes, int flags, struct sockadrr_in *senderAdrr,
//                socklen_t *len);
//    inline operator int() const 
//    {
//        return m_sockfd;
//    }

// private:
//     int m_sockfd;

//     struct sockaddr_in m_addr;
// };

UDPsocket::UDPsocket(int domain)
:m_sockfd(CreateUDPSocket(domain))
{
}

static int CreateUDPSocket(int domain)
{
    int sockfd;
    if ((sockfd = socket(domain, SOCK_DGRAM, 0)) < 0)
    {
        ReportAndHandle("socket Failed", TERMINATE);
    }
    return sockfd;
}

UDPsocket::UDPsocket(int domain, long Adrr, short port)
:m_sockfd(CreateUDPSocket(domain))
{
    PrepareAdrress(&m_addr, domain, Adrr, port);

    if (bind(m_sockfd, (const struct sockaddr *)&m_addr, sizeof(m_addr)) < 0)
    {
        ReportAndHandle("bind Failed", TERMINATE);
    }
}

UDPsocket::~UDPsocket()
{
    close(m_sockfd);
}

int UDPsocket::SendMSG(std::string &buffer, int flags, sockadrr_in *sendTo)
{
    socklen_t len = sizeof(sendTo);
    std::size_t msglen = buffer.length() ;
    const char *msgbuf = buffer.c_str();

    int n = sendto(m_sockfd, msgbuf, msglen, flags, (const sockaddr *)sendTo, len);
    return n;
}


int UDPsocket::RecMSG(std::string &buffer, int flags, sockadrr_in *senderAdrr,
               socklen_t *len)
{
    char *msgbuf = const_cast<char *>(buffer.c_str());
    sockaddr *sender = reinterpret_cast<sockaddr *> (senderAdrr);

    int n = recvfrom(m_sockfd, msgbuf, MAX_MSG, flags, sender, len);
    buffer = msgbuf;

    return n;
}

void ReportAndHandle(const char *msg, hanlde_t handler)
{
    perror(msg);
    if (TERMINATE == handler)
    {
        exit(-1); /* failure */
    }
}


static void PrepareAdrress(struct sockaddr_in *servaddr ,int domain, long Adrr, short port)
{
    memset(servaddr, 0, sizeof(*servaddr));
    servaddr->sin_family = domain;
    servaddr->sin_addr.s_addr = Adrr;
    servaddr->sin_port = htons(port);
}


