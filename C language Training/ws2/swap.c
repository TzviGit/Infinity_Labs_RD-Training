#include <stddef.h> /*syze_t*/
#include <stdio.h>

void SwapInt ( int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}


void SwapSize_t (size_t *a, size_t *b )
{
	size_t temp = *a;
	*a = *b;
	*b = temp;

}


void SwapSize_tPointer (size_t **p1, size_t **p2)
{
	size_t temp =(size_t) *p1;
	*p1 = *p2;
	*p2 =(size_t *) temp;

}


void TestSwapInt (int *t1, int *t2)
{

    int a = *t1, b = *t2;

	SwapInt (t1,t2);

	if ((*t1 == b) && (*t2 == a))
	{
		puts("successfull!");
	}else
	{
		puts ("Failed Test!!!!!!");
	}

}


void TestSwapSize_t (size_t *t1, size_t *t2)
{

    size_t a = *t1, b = *t2;

	SwapSize_t(t1,t2);

	if ((*t1 == b) && (*t2 == a))
	{
		puts("successfull!");
	}else
	{
		puts ("Failed Test!!!!!!");
	}

}


void TestSwapSize_tPointer (size_t **t1, size_t **t2)
{
	size_t *a = *t1;
	size_t *b = *t2;

	SwapSize_t((size_t *)t1,(size_t *)t2);

	if ((*t1 == b) && (*t2 == a))
	{
		puts("successfull!");
	}
	{
		puts ("Failed Test!!!!!!");
	}

}

int main()
{
	return 0;
}
