#include <math.h>
#include "dhcp.h"
#include "test_util.h"


void DHCPPrintOccupied(const dhcp_t *dhcp);
static void PrintAddress(ipv4_t id)
{
	printf(_CYAN_);
	printf("subnet_id:  %d.%d.%d.%d\n", id.address[0],
		id.address[1], id.address[2], id.address[3]);
	printf(_WHITE_);
}

int main(void)
{
	RESET_FLAG;

	size_t subnet_length = 0;

    ipv4_t test1 = {{0,0,0,0}};
    ipv4_t test2 = {{1,1,1,2}};
	ipv4_t test3 = {{0,0,0,0}};
	ipv4_t test4 = {{0,0,0,0}};
	ipv4_t test5 = {{255,255,255,253}};
	ipv4_t test6 = {{255,255,255,253}};

	ipv4_t test7 = {{255,255,255,255}};
    ipv4_t subnet_id = {{1,1,1,0}};

    dhcp_t *dhcp = DHCPCreate(subnet_id, subnet_length);


	long height = 32 - subnet_length;
	long max_cap = pow(2, height) - 3;




   PrintAddress(DHCPAllocateIP(dhcp, test2));



    PrintAddress(DHCPAllocateIP(dhcp, test2));


	PrintAddress(DHCPAllocateIP(dhcp, test2));


	PrintAddress(DHCPAllocateIP(dhcp, test4));

	VERIFY(max_cap - 4 == DHCPCountFree(dhcp));
	PrintAddress(DHCPAllocateIP(dhcp, test5));


	printf("\n\n" );
	VERIFY(max_cap - 5 == DHCPCountFree(dhcp));

	DHCPFreeIP(dhcp, test5);

	VERIFY(max_cap - 4 == DHCPCountFree(dhcp));

	PrintAddress(DHCPAllocateIP(dhcp, test7));

	VERIFY(max_cap - 4 == DHCPCountFree(dhcp));
	PrintAddress(DHCPAllocateIP(dhcp, test6));

	VERIFY(max_cap - 5 == DHCPCountFree(dhcp));


    DHCPDestroy(dhcp);

	CHECK_SUCCESS(DHCP);

    return 0;
}
