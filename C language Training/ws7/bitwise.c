#include <stdio.h>

#define TEST_ONE_PARAM(func,num,expected)  if (func((num)) == (expected)) \
    																			printf("%s works\n",#func );\
																					else \
																					printf("%s failed\n",#func )

#define TEST_TWO_PARAM(func,var1, var2,expected)  if (func((var1),(var2)) == (expected))\
 																									printf("%s works\n",#func );\
																									else \
																									printf("%s failed\n",#func )
int XTimesPowerBase2 (unsigned int, int );
int AddOne(int );
int Add(unsigned int , unsigned int );
int Subtract(unsigned int , unsigned int );
int BitIsSet(unsigned int , unsigned int );
int CountSetBits(unsigned int );
int IsPowerOf2(unsigned int );
int IsPowerOf2_NO_LOOP(unsigned int);
void Print3Bit(int *arr, size_t);
int MirrorBits(unsigned char );
int ThreeAndSixBit(unsigned char );
int ThreeORSixBit(unsigned char );
int Swap3And6Bit(unsigned char );
void SwitchBit(unsigned int *, unsigned int );
int ByteMirror_NoLoop(unsigned char );
void PrintBinary ( int,int);
void PrintFloatBits(float fl);
int CountSetBits_NO_LOOP(int );

int XTimesPowerBase2 (unsigned int x, int y)
{
	return (x << y);
}
int AddOne( int num)
{
	int i = 1;
	while ((num & i) )
	{
		num ^= i;
		i <<= 1;
	}
	return  (num | i) ;
}

int Add(unsigned int num1, unsigned int num2)
{
	unsigned int carry = ((num1 & num2) << 1), sum = (num1 ^ num2) ;
	while (carry & sum)
	{
		carry = sum & carry;
		sum = sum ^ carry;
		carry <<= 1;
}
sum =sum ^ carry;
	return sum;
}
int Subtract(unsigned int num1, unsigned int num2)
{
		num1 = ~num1;

	  num1 = Add(num1,num2);
		return ~num1;
}


int BitIsSet(unsigned int num, unsigned int bit)
{
	return ((num >> (bit-1)) & 1);
}



int CountSetBits(unsigned int num)
{
	int set_bits = 0;
	while (num)
	{
		set_bits += (num & 1);
		num >>= 1;
	}
	return set_bits;
}
int CountSetBits_NO_LOOP(int n)
{
    int count = 0;

 /*each iteration closes a bit so it will only run for as many bits are set*/
    while (n)
    {
        n = n & (n - 1);

        count++;
    }

    return count;
}

int IsPowerOf2(unsigned int num)
{
	return (1 == CountSetBits(num));
}

int IsPowerOf2_NO_LOOP(unsigned int num)
{
	return (0 == (num & (num - 1))) && num;
}

void Print3Bit(int *arr, size_t size)
{
	size_t i = 0;
	for (i = 0; i < size; i++)
	{
		if (3 == CountSetBits(arr[i]))
		{ printf("%u",arr[i]);}
	}
}

int ByteMirror_NoLoop(unsigned char num)
{
	num = (((num & 0xf0) >> 4) | ((num & 0x0f) << 4));
	num = (((num & 0xcc) >> 2) | ((num & 0x33) << 2));
	return ((num & 0x1010) >> 1) | ((num & 0x55) << 1);
}

int MirrorBits(unsigned char num)
{
	unsigned char msb = (8 * sizeof(num)) -1;
	unsigned char result = 0;
	unsigned char i = 0;

	while (msb  >= i)
	{
		result |= (((num >> i) & 1 ) << ( msb - i));
		++i;

	}


	 return result;
}

int TwoAndSixBit(unsigned char ch)
{
	return (BitIsSet(ch, 2) && BitIsSet(ch, 6));
}


int TwoORSixBit(unsigned char ch)
{
	return (BitIsSet(ch, 2) || BitIsSet(ch, 6));
}

void SwitchBit(unsigned int *num, unsigned int bit)
{
	*num = (*num ^ (1 << (bit - 1)));
}

int Swap3And6Bit(unsigned char ch)
{
	if (BitIsSet(ch, 3) ^ BitIsSet(ch, 5))
	{
		SwitchBit((unsigned int *)&ch, 3);
		SwitchBit((unsigned int *)&ch,5);
	}
	return ch;
}


int ClosestDevisable16(unsigned int num)
{
	return (~15 & num);
}
 void SwapNoTemp(unsigned int *num1, unsigned int *num2)
 {
 	 if (*num1 ==*num2)
	 {
		 return;
	 }

	 *num1 = *num1 ^ *num2;
 	 *num2 = *num1 ^ *num2;
 	 *num1 = *num1 ^ *num2;
 }

void PrintBinary (int num,int bit)
{
	if (0 == bit)
	{
		printf("\n");
		return;
	}
	PrintBinary(num,bit << 1);
	printf("%d",((num & bit) / bit));
}

void PrintFloatBits(float fl)
{
	unsigned int *p = (unsigned int *) &fl;

	PrintBinary(*p,1);
}

int main()
{
	unsigned int a = 0x90, b = 0x09;
	TEST_TWO_PARAM(Add,3,6,9);
	TEST_ONE_PARAM(ClosestDevisable16,38,32);
	TEST_ONE_PARAM(Swap3And6Bit,'E','Q');
	TEST_ONE_PARAM(TwoORSixBit,71,1);
	TEST_ONE_PARAM(TwoAndSixBit,39,1);
	TEST_TWO_PARAM(Subtract,38,30,8);
	TEST_ONE_PARAM(MirrorBits,0x0c,0x30);
	TEST_ONE_PARAM(AddOne,38,39);
	TEST_ONE_PARAM(AddOne,38,39);
	printf ("%d",Add(a,b));
	PrintBinary(0x0c,1);  PrintBinary(MirrorBits(0x0c),1);
	return (0);
}
