#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

	size_t StrLen(const char *);
	int StrCmp (const char *, const char *);
	void CopyArray(const int *, int *);
	void SwapSPointer (size_t *, size_t *);
	void SwapSize_t (size_t *, size_t *);
	void SwapInt ( int *, int *);
	void PrintAdresses ();
	void TestSwapSize_t (size_t *, size_t *);
	void TestSwapTestSize_t (size_t *, size_t *);

  int main ()
  {
     char s1[] = "hello";
     char s2[] = "hello!";


     int i = 1, j = 2;
     int *ptr1 = &i, *ptr2 = &j;

     printf ("%p   %p\n",(void *)ptr1,(void *)ptr2);
     printf ("%d  %d\n",*ptr1,*ptr2);
     SwapSize_t ((size_t *)&ptr1,(size_t *) &ptr2);


     printf ("%p   %p\n",(void *)ptr1, (void *)ptr2);
     printf ("%d  %d\n",*ptr1,*ptr2);


     printf("\n%d",StrCmp (s1,s2));

  	 PrintAdresses();


    return 0;

 }

 void SwapInt ( int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}


void SwapSize_t (size_t *a, size_t *b )
{
	size_t temp = (size_t)*a;
	*a = *b;
	*b = temp;

}


void SwapPointer (size_t * pp1, size_t * pp2)
{
	size_t temp = (size_t) *pp1;
	*pp1 = *pp2;
	*pp2 = temp;

}



void CopyArray(const int *arr, int *copyarr)
{
    int index = 0;

	while (arr)
	{
		copyarr[index] = arr[index];
		arr++;
		index++;
	}

}

 void PrintAdresses ()
 {
     static int s_i = 7;
  	 int i = 7;
  	 int *ptr = &i;
  	 int *ptr2 = (int *)malloc(sizeof(int));


   	 printf("\n%p",(void *)&s_i);
     printf("\n%p",(void *)&i);
     printf("\n%p",(void *)ptr);
     printf("\n%p",(void *)&ptr);
     printf("\n%p",(void *)ptr2);
     printf("\n%p",(void *)&ptr2);



    /* if (ptr)
     {
        &ptr;
     }
*/

     free (ptr2);
     ptr2 = NULL;

   }

   size_t StrLen (const char *s)
   {
   		size_t size = 0;

   		while ('\0' != *s)
   		{
   			size += sizeof(char);
   			s++;
   		}

   			return size;
   	}


int StrCmp (const char *s1, const char *s2)
{
	while (*s1)
   	{
   		if ((*s1++) == *(s2++))
   		{
   			continue;
   		}
   		else
   		{
   			return (*(s1 - 1) > *(s2 - 1)) ?  1 : -1;
		}
	}

	if ('\0' != *s2)
   	{
   		return -1;
   	}

	return 0;
}



  void TestSwapSize_t (size_t *t1, size_t *t2)
{
	size_t a = *t1, b = *t2;

	SwapSize_t (t1,t2);

	if ((*t1 == b) && (*t2 == a))
	{
		puts("successfull!");
	}else
	{
		puts ("Failed Test!!!!!!");
	}

}
