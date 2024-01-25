#include <stdio.h>
#include <stdlib.h>

/* Fania */
void Kill(int *);
void TestLastStanding(int);
int LastStanding(int );
int *NextAlive(int *, int *, int *);

int main()
{
	TestLastStanding(4099);
	TestLastStanding(8);
	TestLastStanding(11);
	return (0);
}





int LastStanding(int num)
{
	int *men = (int *)malloc(sizeof(int) * num);
	int *p = men;
	int *end = (men +num -1);
	int final_man = 0;
	
	int i = 1;
	for (i = 1; i <= num; i++)
	{
		*p++ = i;
	}
	
	p = men;
	
	while (p != NextAlive(p, men, end))
	{
		
		p = NextAlive(p, men, end) ;
		
		Kill(p);
		
		p = NextAlive(p, men, end);
		
	}
	
	final_man = *p;
	free(men);
	
	return final_man;
}

int *NextAlive(int *p, int *start, int *end)
{
	do
	{
		if (p == end)
		{p = start;}
		else
		{ ++p;}
	}
	while (0 == *p);
	
		
	return p;
}
void Kill(int *p)
{	
	*p = 0;
}

void TestLastStanding(int num)
{
	printf("the last one standing out of %d is: %d\n\n", num, LastStanding(num));
}



