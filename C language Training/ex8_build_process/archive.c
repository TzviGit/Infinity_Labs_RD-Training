!<arch>
a.c/            0           0     0     644     58        `
#include "g.h"
#include <stdio.h>

void Foo()
{
	++g_s;
}
g.h/            0           0     0     644     14        `
int g_s = 3;

main.c/         0           0     0     644     110       `
#include <stdio.h>
#include "g.h"

int main()
{
	printf("%d",g_s);
	Foo();
	printf("%d",g_s);
	
	return 0;
}

