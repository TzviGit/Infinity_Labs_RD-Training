#include <stdio.h> /*for printf*/

long double power(int n)
{
	int i = 0;
	long double result = 1;
	double base = 10;
	
	if ( n < 0 )
	{	
		n = -n;
		base= 1 / base;
	}
	
	for (i = 0; i < n ; ++i)
	{
		result *= base;
	}
	
	return result;
	
}

void HelloWorld(void)
{
	const char hello[] = {0x22, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x22};
	
    printf("%s", hello);
}

int main()
{
    
		HelloWorld();
		
		printf("\n%Lf",power(10));
		
		
    
	return (0);
}	
	
	
	
