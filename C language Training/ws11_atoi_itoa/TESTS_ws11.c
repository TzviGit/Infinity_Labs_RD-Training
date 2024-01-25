
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ws11_funcs.h"

int main()
{
    char arr1[] = " acefBDFG ";
    char arr2[] = "abcdegABCDEF", blocker[] = "bDgE";

    char result[100] = "1000";
    char *dest = (char *)malloc(100);

    int base = 2;

    int val = -8;


    PrintLetFirst2Arr(arr1,strlen(arr1), arr2, strlen(arr2), blocker, strlen(blocker));





    /*TEST ENDIANNES*/
   if (IsLittleEndian())
   {
       puts("SYSTEM IS LITTLE_ENDIAN");
   }
   else
   {
        puts("SYSTEM IS BIG_ENDIAN");
   }

   /*TEST MACRO*/
   if (IS_LITTLE_ENDIAN)
   {
       puts("SYSTEM IS LITTLE_ENDIAN");
   }
   else
   {
        puts("SYSTEM IS BIG_ENDIAN");
   }


   /*TEST ITOA*/

    TestItoa(val,dest, base,result);

    TestAtoi();

    free(dest);
    dest = NULL;

    return 0;
}
