/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      -------------
* Description   DHCP ip address allocator module. it's responsible for
*                allocating to user a unique ip adress in a given sub-network.
*
* Date          November 11, 2021
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/



#include <assert.h> /*assert*/
#include <string.h> /*memcpy*/
#include <limits.h> /*CHAR_BIT*/
#include <stdlib.h> /*malloc, free*/
#include <math.h> /*the power function -- MUST BE COMPILED WITH libmath */
#include "dhcp.h" /* the API for this module */



static const ipv4_t BAD_ADDRESS =  {{0,0,0,0}};
static const size_t IPV4_BITS = 32;
static const size_t IPV4_BYTES = 4;


typedef enum
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
} children_t;

typedef enum
{
    OFF,
    ON
}bit_status_t;


typedef struct dhcp_node
{
    int is_full;
    struct dhcp_node *children[NUM_OF_CHILDREN];
}dhcp_node_t;


struct dhcp
{
    size_t subnet_length;
    dhcp_node_t *root;
    ipv4_t subnet_id;

};


static int InitDHCPStruct(dhcp_t *dhcp, ipv4_t subnet_id, size_t subnet_length);
static void DHCPCountImple(dhcp_node_t *node, size_t height, size_t *count);
static int HasLeftSon(dhcp_node_t *node);
static int HasRightSon(dhcp_node_t *node);
static int HasNoChildren(dhcp_node_t *node);
static dhcp_node_t *DHCPFreeImple(dhcp_node_t *node,
                                    ipv4_t address_to_free, size_t bit_index);
static int IPv4IsEqual(ipv4_t ip1, ipv4_t ip2);
static int IsFull(dhcp_node_t *node);
static void UpdateFullness(dhcp_node_t *node);
static bit_status_t GetIpv4Bit(ipv4_t ip_address, size_t index);
static void SetIpv4Bit(ipv4_t *ip_address, size_t index, bit_status_t status);
static void ZeroOutAddress(ipv4_t *ip, size_t bit_index);
static int InitializeReservedIPs(dhcp_t *dhcp);
static void DestroyImple(dhcp_node_t **node);
static void AddSubnetAddress(dhcp_t *dhcp ,ipv4_t *ip_address);
static dhcp_node_t *DhcpNewNode();
static int DHCPAllocateIPImple(dhcp_node_t *node, ipv4_t *requested_ip,
                                                            size_t bit_index);






/*****************************************************************************
*       A slew of useful utility functions to help readability of code.
*        please make sure you understand them.
******************************************************************************/

static void SetSon(dhcp_node_t *parent, children_t side, dhcp_node_t *son)
{
    parent->children[side] = son;
}

static dhcp_node_t *GetSon(dhcp_node_t *parent, children_t side)
{
    return parent->children[side];
}


static int HasNoChildren(dhcp_node_t *node)
{
    return NULL == node->children[LEFT] &&  NULL == node->children[RIGHT];
}


static int HasLeftSon(dhcp_node_t *node)
{
    return NULL != node->children[LEFT];
}

static int HasRightSon(dhcp_node_t *node)
{
    return NULL != node->children[RIGHT];
}




/*****************************************************************************
*       Here are the functions which get and set individual bits
* of the address. They are NOT trivial..please make sure you understand them.

    Additionaly we have here the IPv4IsEqual function,
     which equates to address structs
******************************************************************************/

static bit_status_t GetIpv4Bit(ipv4_t ip_address, size_t index)
{
    size_t cell = (IPV4_BYTES - 1) - index / CHAR_BIT;
    size_t bit_offset = index % CHAR_BIT;

    char segment = ip_address.address[cell];

    return (segment >> bit_offset) & (unsigned char)1;
}

static void SetIpv4Bit(ipv4_t *ip_adress, size_t index, bit_status_t status)
{
    size_t cell = (IPV4_BYTES - 1) - index / CHAR_BIT;
    size_t bit_offset = index % CHAR_BIT;

    ip_adress->address[cell] &= ~((unsigned char)1 << bit_offset);
    ip_adress->address[cell] |= ((unsigned char)status << bit_offset);
}


static int IPv4IsEqual(ipv4_t ip1, ipv4_t ip2)
{
    return 0 == memcmp(ip1.address, ip2.address, 4);
}



/****************************************************************************
*       The DHCPCreate functions and its helper functions.
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~size_t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* O(log n) */
/* param : subnet_length is num of masked bits, 0 <= subnet_length <= 30 */
/* allocate 3 special addresses for network(00), server(10), broadcast(11) */

dhcp_t *DHCPCreate(ipv4_t subnet_id, size_t subnet_length)
{
    int malloc_status = 0;
    int init_network_status = 0;

    dhcp_t *dhcp = (dhcp_t *)malloc(sizeof(*dhcp));

    if (NULL == dhcp)
    {
        return NULL;
    }

    malloc_status = InitDHCPStruct(dhcp,subnet_id, subnet_length);

    if (0 != malloc_status)
    {
        DHCPDestroy(dhcp);
        dhcp = NULL;
    }

    init_network_status = InitializeReservedIPs(dhcp);

    if (0 != init_network_status)
    {
        DHCPDestroy(dhcp); dhcp = NULL;
    }

    return dhcp;

}


static int InitDHCPStruct(dhcp_t *dhcp, ipv4_t subnet_id, size_t subnet_length)
{
    dhcp_node_t *node = DhcpNewNode();
    if (NULL == node)
    {
        return -1;
    }

    dhcp->root = node;
    dhcp->subnet_id = subnet_id;
    dhcp->subnet_length = subnet_length;

    return 0;
}

static int InitializeReservedIPs(dhcp_t *dhcp)
{
	int status = 1;
	ipv4_t network = {{0,0,0,0}};
    ipv4_t server = {{255,255,255,254}};
    ipv4_t  broadcast = {{255,255,255,255}};


	DHCPAllocateIP(dhcp, network);

	status *= !IPv4IsEqual(BAD_ADDRESS, DHCPAllocateIP(dhcp, server));

	status *= !IPv4IsEqual(BAD_ADDRESS, DHCPAllocateIP(dhcp, broadcast));

	return !status;
}

static void AddSubnetAddress(dhcp_t *dhcp ,ipv4_t *ip_address)
{
	size_t i = 0;
    for (i = 0; i < dhcp->subnet_length; ++i)
    {
        size_t bit_index = (IPV4_BITS - 1) - i;
        SetIpv4Bit(ip_address, bit_index, GetIpv4Bit(dhcp->subnet_id, bit_index));
    }
}



static dhcp_node_t *DhcpNewNode()
{
    dhcp_node_t *new_node = (dhcp_node_t *)malloc(sizeof(*new_node));
    if (NULL == new_node)
    {
        return NULL;
    }

    new_node->is_full = 0;
    SetSon(new_node, RIGHT, NULL);
    SetSon(new_node, LEFT, NULL);

    return new_node;
}



/*****************************************************************************
*       DHCPDestroy and its Imple function
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void DHCPDestroy(dhcp_t *dhcp)
{
    assert (dhcp);

    DestroyImple(&dhcp->root);

    free(dhcp); dhcp = NULL;
}

static void DestroyImple(dhcp_node_t **node)
{
    if (NULL == *node)
    {
        return ;
    }

    /* <<<< needs l-value so cant use GetSon >>> */
    DestroyImple(&((*node)->children[LEFT]));
    DestroyImple(&((*node)->children[RIGHT]));

    free(*node);
    *node = NULL;
}



/*****************************************************************************
*       DHCPAllocateIP and its Imple function and util func "ZeroOutAddress"
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* O(log n)
* param : requested_ip - smallest accepted ip address
* if no bigger or equal address is free - return network address
* if ip isn't in mask range - UB
* otherwise return requested ip or next free ip
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


ipv4_t DHCPAllocateIP(dhcp_t *dhcp, ipv4_t requested_ip)
{
    size_t bit_index = 0;
    int status = 0;

    assert (dhcp);

    AddSubnetAddress(dhcp, &requested_ip);
    bit_index = (IPV4_BITS - 1) - dhcp->subnet_length;

    status =  DHCPAllocateIPImple(dhcp->root, &requested_ip, bit_index);

    return (0 == status) ? requested_ip : BAD_ADDRESS;
}


static int DHCPAllocateIPImple(dhcp_node_t *node, ipv4_t *requested_ip,
                                                            size_t bit_index)
{
    int status = 0;
    children_t direction = 0;

    if (IsFull(node))
    {
        return -1;
    }

    if ((size_t)-1 == bit_index)
    {
        node->is_full = 1;
        return 0;
    }


    direction = GetIpv4Bit(*requested_ip, bit_index);

    if (NULL == GetSon(node, direction))
    {
        dhcp_node_t *new_node = DhcpNewNode();
        if (NULL == new_node)
        {
            return -1;
        }

        SetSon(node, direction, new_node);
    }

    status = DHCPAllocateIPImple(GetSon(node, direction),
                                requested_ip, bit_index - 1);

    if (0 != status)
    {
        if (RIGHT == direction)
        {
            status = -1;
        }
        else
        {
            direction = RIGHT;

            SetIpv4Bit(requested_ip, bit_index, ON);
            ZeroOutAddress(requested_ip, bit_index - 1);
            status = DHCPAllocateIPImple(node, requested_ip, bit_index);
        }
    }


    UpdateFullness(node);

    return status;

}


/***  Updates the address to be the next address starting fron given bit ***/

static void ZeroOutAddress(ipv4_t *ip, size_t bit_index)
{
    size_t i = 0;
    if ((size_t)-1 == bit_index)
    {
        return;
    }

    for (i = 0; i <= bit_index; ++i)
    {
        SetIpv4Bit(ip, i, OFF);
    }

}


/****************************************************************************
*   The Functions which deal with updating the "is_full" field
*    of the nodes during the recursive proscess.
*****************************************************************************/


static int IsFull(dhcp_node_t *node)
{
    return NULL != node && node->is_full;
}


static void UpdateFullness(dhcp_node_t *node)
{
    node->is_full = IsFull(GetSon(node, LEFT)) &&
     IsFull(GetSon(node, RIGHT)) ? 1 : 0;
}




/*****************************************************************************
*       DHCPFreeIP and its Imple function
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
* O(log n)
* frees the given ip address
* undefined, if the address is free'd already , or not previously alocated */

void DHCPFreeIP(dhcp_t *dhcp, ipv4_t address_to_free)
{
    size_t bit_index = 0;

    assert (dhcp);

    bit_index = (IPV4_BITS - 1) - dhcp->subnet_length;

    dhcp->root = DHCPFreeImple(dhcp->root, address_to_free, bit_index);

    UpdateFullness(dhcp->root);
}


static dhcp_node_t *DHCPFreeImple(dhcp_node_t *node,
                                    ipv4_t address_to_free, size_t bit_index)
{
    children_t direction = GetIpv4Bit(address_to_free, bit_index);

    if (0 == bit_index)
    {
        free(GetSon(node, direction)); SetSon(node, direction, NULL);
    }
    else
    {
        dhcp_node_t *son =
        DHCPFreeImple(node->children[direction], address_to_free, bit_index - 1);

        SetSon(node, direction, son);
    }

    if (HasNoChildren(node))
    {
        free(node); node = NULL;

    }
    else
    {
        UpdateFullness(node);
    }

    return node;
}



/*****************************************************************************
    <<<<< The DHCPCountFree Function and its recursive Imple function >>>>>

*****************************************************************************/

size_t DHCPCountFree(const dhcp_t *dhcp)
{
    size_t height = 0;
    size_t count = 0;

    assert (dhcp);

    height =  IPV4_BITS - dhcp->subnet_length;

    DHCPCountImple(dhcp->root, height,  &count);

    return pow(2, height) - count;
}

static void DHCPCountImple(dhcp_node_t *node, size_t height, size_t *count)
{
    if (IsFull(node))
    {
        *count += pow(2, height);
    }
    else
    {
        if (HasLeftSon(node))
        {
            DHCPCountImple(GetSon(node, LEFT), height - 1, count);
        }

        if (HasRightSon(node))
        {
            DHCPCountImple(GetSon(node, RIGHT), height - 1, count);
        }
    }

    return;
}




/*******    <<<<<  API FUNCTIONS COMPLETE !!  >>>>>>   ************/
