#include <stdio.h>
#include <stdlib.h>
#include "string.h"




int main()
{
	char ptr1[100] = "hello World!!!";
	char ptr2[100] = "needle";
	char ptr3[100] = "find the needlneedle int the haystack needle";

	testStrDup("hello World!!!");
	testStrSpn(ptr1, "alofgjtjr",3);
	testPallindrome("hannah", 1);
	testPallindrome("hadnnah", 0);
	
	/*testStrCat(ptr1,ptr2,"hello World!!!needle");*/
	testStrStr(ptr3,ptr2,"needle int the haystack needle");
	printf("%s   %s\n","testttt",StrCat(ptr2,ptr1));

	/*puts ("whats up?\n");
	test2 = StrCpy (ptr2,ptr1);
	printf ("%s",ptr2);
	printf("StrCpy test: %s\n\n", StrCpy (test,ptr1));
	printf("StrnCpy and StrCat test: %s\n\n", StrCat (StrnCpy (test,ptr1,7),'\0'));
*/	return 0;
}

int StrCmp (const char *s1, const char *s2)
{

	while (*s1 && (*s1 == *s2))
		{
		    ++s1;
			++s2;
		}

	return (*s1 - *s2);
}

int StrnCmp(const char *s1, const char *s2, size_t n)
{
	char *ptr =(char *)s1;
	ptr = StrnCpy(ptr, ptr, n);
	*(ptr + n) = '\0';
	return  StrCmp(s1,s2);

}


/*jhonny*/
char *StrnCpy (char *dest, const char *src, size_t n)
{
	char *ptr_dest = dest;
	while (*(src++) && (n--))
	{
		*(ptr_dest++) = *(src - 1);
	}

	while (n--)
	{
		*(ptr_dest++) = '\0';
	}

    return dest;

}

char *StrCpy(char *dest, const char *src)
{
	return StrnCpy (dest,src,StrLen (src) );
}



char *StrChr (const char *s,char c)
{
	int i = 0;

	for (i =0; '\0' != s[i]; i++)
	{
		if (c == s[i])
		{
			return (char *)(s + i);
		}
	}


	return NULL;

}

char *StrDup(const char *s) /*jhonny */
{
	char *copy = (char *)malloc(sizeof(char) * (StrLen(s) + 1));

	return copy ? StrCpy(copy, s) : NULL;
}

void testStrDup(char *s)
{
	printf("%s\n\n", ( (StrCmp(StrDup(s), s) == 0) ? "StrDup works!" : "StrDup failed" ));
}

char *StrCat(char *dest, const char *s) /* Jhonny*/
{
	return StrnCat (dest, s, StrLen(s) + 1);
}

void testStrCat(char *dest, char *s, char *result)
{
	printf("%s\n\n",  StrCmp(StrCat(dest, s), result) == 0 ? "StrCat works!" : "StrCat failed" );
}


char *StrnCat(char *dest, const char *s,size_t n) /*jhonny*/
{
	size_t length = StrLen(dest);
	char *temp = dest + length;
  	StrnCpy(temp, s, n);
  	AddNull(dest);
	return dest;

}

void AddNull(char *s)
{
	size_t size = StrLen(s);
	*(s + size) = '\0';
}


char *StrStr(const char *haystack, const char *needle)
{
	size_t hay_len = StrLen(haystack);
	size_t needle_len =StrLen(needle);
	char *end_check =(char *)(haystack + hay_len - StrLen(needle));
		while (haystack <= end_check)
		{
			haystack = StrChr(haystack, *needle);
			if (0 == StrnCmp(haystack, needle, needle_len))
			{
				return (char *)haystack;
			}

			++haystack;
		}

		return NULL;
}




void testStrStr(char *haystack, char *needle, char * result)
 {
	printf("%s\n\n", (0 == StrCmp((StrStr(haystack, needle)), result)  ? "StrStr works!" : "StrStr failed" ));
}

size_t StrSpn (char *s, char *accept) /*aviad*/
{
	size_t span = 0;



	  while (*s)
	  {
	  	if ( StrChr (accept,*s))
	  	{
	  		++span;
	  	}
	  	else
	  	{
	  		return span;
	  	}
	  }



	  return span;
}

void testStrSpn(char *s, char *accept, size_t result)
 {
	printf("%s\n\n", ( (StrSpn(s, accept) == result) ? "StrSpn works!" : "StrSpn failed" ));
}

int IsPallindrome(const char *s1)
{
	char *last = (char *)(s1 + StrLen(s1) - 1);

	while ((last - s1) > 0 && (*s1 == *last))
	{
		++s1;
		--last;
	}

	return (last - s1) > 0 ? 0 : 1 ;
}

void testPallindrome(char *s,int a)
{

  printf("%s\n\n", (IsPallindrome(s) == a) ? "yay IsPallendrome works!" : "IsPallendrome failed");

}


void Swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

/* Assume src has at least len elements, dest can hold len elemnts */
void CopyArray(const int *src, int *dest, size_t len)
{
    size_t i = 0;

	for (; i < len; ++i)
	{
		dest[i] = src[i];
	}

}

size_t StrLen (const char *str) /*jhonny*/
{
	size_t size = 0;

	while (*str++)
	{
		size++;
	}

	return size;
}
