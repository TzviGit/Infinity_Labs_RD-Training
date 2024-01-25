#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc*/
/*
  Approved by Fania
*/
size_t StrLen (const char *);
char *StrCpy(char *, const char *);
char *StrnCpy (char *, const char *, size_t n);
char *StrCat (char *, const char *);
char *StrnCat (char *, const char *, size_t);
char *StrDup (const char *);
char *StrStr (const char *, const char *);
char *StrChr (const char *,char);
void testStrDup(char *);
void testStrCat(char *, char *, char *);
void testStrnCat(char *, char *, size_t, char *result);
void testStrLen(char *, size_t );
void testStrStr(char *, char *, char* );
void testStrSpn(char *, char *, size_t );
void testPallindrome(char *,int );
size_t HowManyArgs(char **);
void CopyStrArr(char **, char **);
void PrintStrArr(char **);
void FreeStrArr(char **);
void ToLower(char *);
void PrepareBuffer(char **, char **);
void ArrayToLower(char **);

int main (int argc, char **argv, char **envp)
{

	size_t size = HowManyArgs(envp);
	char **buffer = (char **)malloc((size + 1)* sizeof(*buffer));

	PrepareBuffer(buffer,envp);
	PrintStrArr(buffer);
	FreeStrArr(buffer);
	(void)argc;
	(void)argv;
	return 0;
}

/*returns string_count in arry of strings*/
size_t HowManyArgs(char **arr)
{
	size_t size = 0;
	while (*arr++)
	{
		++size;
	}

	return size;
}
void PrepareBuffer(char **buffer, char **arr)
{
		CopyStrArr(buffer, arr);
		ArrayToLower(buffer);
}

void CopyStrArr(char **buffer, char **arr)
{
	while (*arr)
	{
		*buffer = StrDup(*arr);
		++arr;
		++buffer;
	}

	*(buffer) = NULL;
}

void PrintStrArr(char **arr)
{
	while (*arr)
	{
		printf("%s\n",*arr);
		++arr;
	}

}

void FreeStrArr(char **arr)
{
	char **ptr = arr;

	while (*arr)
	{
		free(*arr);
		++arr;
	}

	free(ptr);
}

void ArrayToLower(char **arr)
{
	while (*arr)
	{
		ToLower(*arr);
		++arr;
	}
}

 void ToLower(char *str)
 {
 	while (*str)
 	{
 		if (*str >= 'A' && *str <= 'Z' )
 		{
 			*str += 32;
 		}

 		++str;
 	}
 }




 /*OTHER FUNC USED HERE*/

int StrCmp (char *s1, char *s2)
{

	while (*s1 && (*s1 == *s2))
		{	++s1;
			++s2;
		}

	return (*s1 - *s2) ;
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
  	StrnCpy((dest + StrLen(dest)), s, n);
	return dest;

}

void testStrnCat(char *dest, char *s, size_t n, char *result)
{
	printf("%s\n\n", ( (StrCmp(StrnCat(dest, s, n), result) == 0) ? "StrnCat works!" : "StrnCat failed" ));
}

size_t StrLen (const char * str) /*jhonny*/
{
	size_t size = 0;

	while (*str++)
	{
		size++;
	}

	return size;
}

void testStrLen(char *s, size_t result)
{
	printf("%s\n\n", ( (StrLen(s) == result) ? "StrLen works!" : "StrLen failed" ));
}


char *StrStr (const char * haystack, const char * needle) /*jhonny*/
{
	char *tmp_ndle = NULL;
	char *tmp = NULL;

	while (StrLen(haystack) >= StrLen(needle))
	{
		haystack = StrChr (haystack, *needle);
		tmp_ndle = (char *)(needle + 1);
		tmp = (char *)(haystack + 1);

		while (*tmp_ndle && (*tmp== *tmp_ndle))
		{
			++tmp;
			++tmp_ndle;
		}

	   printf("%c  %d\n\n",*tmp_ndle,56);
	    if (! *tmp_ndle)
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
	size_t span = 1;

	while ( *s && (! StrChr (accept,*s)))
	{
		s++;
	}

	if (! *s)
	{
	  	return 0;
    }

	 ++s;

	  while ( (*s) && StrChr (accept,*s))
	  {
	  	++s;
	    span++;
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
