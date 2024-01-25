#ifndef _SOCKETS_H_ILRD_
#define _SOCKETS_H_ILRD_
#include <stddef.h>

typedef enum
{
    CONTINUE,
    TERMINATE
}hanlde_t;

void ReportAndHandle(const char *msg, hanlde_t terminate);
int OpenSocket(int __domain, int __type, int __protocol);

void BindSocket(int sock_fd, int family, int adrr, int port);





#endif