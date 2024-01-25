/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Version       1.0.0                                           *
 ~ Author        OL110 Students                                  ~
 * Reviewer      Binyamin                                        *
 ~ Description   DHCP Tree - Header Module                        ~
 * Group         OL110                                           *
 ~ Company       ILRD Ramat Gan                                  ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#ifndef __DHCP_OL_110_H__
#define __DHCP_OL_110_H__


#include <stddef.h> /* size_t */


typedef struct dhcp dhcp_t;

typedef struct address
{
    unsigned char address[4];
}ipv4_t;


/* O(log n) */
/* param : subnet_length is num of masked bits, 0 <= subnet_length <= 30 */
/* allocate 3 special addresses for network(00), server(10), broadcast(11) */
dhcp_t *DHCPCreate(ipv4_t subnet_id, size_t subnet_length);

/* O(n) */
void DHCPDestroy(dhcp_t *dhcp);

/* O(log n) */
/* param : requested_ip - smallest accepted ip address */
/* if no bigger or equal address is free - return network address */
/* if ip isn't in mask range - UB */
/* otherwise return requested ip or next free ip */
ipv4_t DHCPAllocateIP(dhcp_t *dhcp, ipv4_t requested_ip);

/* O(log n) */
/* frees the given ip address */
/* undefined, if the address is free'd already */
void DHCPFreeIP(dhcp_t *dhcp, ipv4_t address_to_free);

/* O(n) */
size_t DHCPCountFree(const dhcp_t *dhcp);


#endif /* __DHCP_OL_110_H__ */
