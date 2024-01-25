#include <stdio.h>
#include "g.h"

int main()
{
	printf("%d",g_s);
	Foo();
	printf("%d",g_s);
	
	return 0;
}

