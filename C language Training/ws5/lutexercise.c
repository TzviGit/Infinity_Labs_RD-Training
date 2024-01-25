


#include <stdio.h>
#include <stdlib.h>

/*NOPHAR*/
char pressed = 1;
void (*LUT[256]) () ;
void A()
{
	printf("A pressed.\n\n");
}

void T()
{
	printf("T pressed.\n\n");
}
void Nothing(){return;}

int ESC()
{
	system("stty icanon echo");
	return 0;
}


void IfMethod()
{
	while (27 != pressed)
	{
		scanf("%c",&pressed);
		if ('A' == pressed)
		{ A();}
		 
		if ('T' == pressed)
		{ T(); }
	}
	
	ESC();
}

void CaseMethod()
{
	while (27 != pressed)
	{
		scanf("%c",&pressed);
		switch (pressed)
		{
			case 'A':
			A();
			break;
			
			case 'T':
			T();
			break;
			
			default:
			Nothing();
		}
	}
	ESC();
}

void FillLUT()
{
	
	int i = 0;
	for (i = 0; i < 256;i++)
	{
		*(LUT + i) = Nothing;
	}

	*(LUT + 'A') = A;
	*(LUT +'T') = T;
	

}	

void LUTMethod()
{
	
	FillLUT();
	scanf("%c",&pressed);
	while (27 != pressed)
	{	
		LUT[(int)pressed]();
		scanf("%c",&pressed);
		
	}
	ESC();
}







int main()
{
	system("stty -icanon -echo");
	
	/* pick your method.
	CaseMethod();
	
	IfMethod();
	*/
	LUTMethod();
	
	
system("stty icanon echo");
	return 0;		
}		
