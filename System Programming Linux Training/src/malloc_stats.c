#include <stdlib.h>
#include <stdio.h>
int main()
{
	char *s;
	malloc_stats();

	printf ("\n\n\n");
	s = malloc(20);
	malloc_stats();
	return 0;
}
