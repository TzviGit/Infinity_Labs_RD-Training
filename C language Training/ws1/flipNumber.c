#include <stdio.h>

#define TEST_FLIP(a,b) printf("%s\n", ((b) == Flip((a)))? "flip function works": "flip function failed!")
#define TEST_FLIP_POINTER(a,b) FlipPointer((a)); \
                                printf("%s\n", ((b) == (*a))? "flip function works": "flip function failed!" )

/*This func recieves pointer and flips the number in the memory flipped version of int parameter*/
void FlipPointer (int *a)
{
	int result = 0, sign = 1;
	if (*a < 0)
	{
		sign = -1;
		*a *= -1;
	}  /*to account for different systems which may not do / and % the same for negative nums*/

	while (*a )
	{
		result = (result * 10) + ( *a % 10);
		*a = *a / 10;
	}

	*a = result * sign;

}

/*this func returns flipped version of int parameter*/

int Flip (int a)
{
	int result = 0, sign = 1;
	if (a < 0)
	{
		sign = -1;
		a *= -1;
	}   /*to account for different systems which may not do / and % the same for negative nums*/

	while (a)
	{
		result = (result * 10) + (a % 10);
		a = a / 10;
	}

	return  result * sign;

}

int main()
{
	int  b = 1234500, *f = &b;
	TEST_FLIP(12345, 54321); /*pass*/
	TEST_FLIP(1234500, 54321); /*pass*/
	TEST_FLIP(-12345, -54321);/*pass*/
	TEST_FLIP(-1234500, -54321);/*pass*/
	TEST_FLIP_POINTER(f, b);
	return (0);

}
