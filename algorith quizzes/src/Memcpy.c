/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      --------
* Description   Implementation Of Memcpy using only one variable.
* Group         OL110
* Company       ILRD Ramat Gan
* Date          October 17, 2021
* ****************************************************************************/

#include <string.h>  /*strlen */
#include "test_util.h"


#define WORD_SIZE 8


void *Memcpy(void *dest, const void *src, size_t n)
{
    char *origin = (char *)dest;

    while (((size_t)src % WORD_SIZE) && n)
    {
        --n;
        *(*(char **)&dest)++ = *(*(char **)&src)++;
    }

    while (n >= WORD_SIZE)
    {
        n -= WORD_SIZE;
        *(*(size_t **)&dest)++ = *(*(size_t **)&src)++;
    }

    while (n)
    {
        --n;
        *(*(char **)&dest)++ = *(*(char **)&src)++;
    }

    return origin;
}




/****************************************************************************
~                     $$$ TEST FUNCION  $$$
~
~*****************************************************************************/

void TestMemcopy()
{
    int flag = 0;

    char dest[300] = "this is comletely atrocias!?D";
    char src[250] = "123456789fkguyjfgyiukgkjs";

    VERIFY(strcmp(dest, src) != 0);

    Memcpy(dest, src, strlen(src) + 1);

    VERIFY(strcmp(dest, src) == 0);

    CHECK_SUCCESS(MEMCPY);

}

int main ()
{
    TestMemcopy();
    return 0;
}
