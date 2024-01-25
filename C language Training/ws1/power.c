long double power(int n)
{
	int i = 0;
	long double result =1;
	double base=10;

	if ( n < 0 )
	{
		n=-n;
		base=1/base;
	}

	for (i=0; i < n ; i++)
	{
		result *= base;
	}

	return result;

}

int main()
{return 0;}
