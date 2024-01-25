#ifndef __MY_TEST_UTIL_HEADER__
#define __MY_TEST_UTIL_HEADER__

#include <stdio.h>

#define _RED_    "\033[1;31m"
#define _GREEN_  "\033[1;32m"
#define _YELLOW_ "\033[1;33m"
#define _BLUE_   "\x1B[1;34m"
#define _PURPLE_ "\033[1;35m"
#define _CYAN_   "\033[1;36m"
#define _WHITE_  "\033[1;0m"
#define _RESET_   "\033[1;0m"


#define RESET_FLAG int flag = 0
#define VERIFY(bool) TestCondition( __LINE__,  (bool), &flag)
#define CHECK_SUCCESS(func_name) TestSuccess(flag , (#func_name))


void TestCondition( int line, int boolean, int *flag)
{
    if (!boolean)
    {
        *flag = 1;
        printf(_RED_"The Test on line _YELLOW_ %d " _RED_ "FAILED!!!\n\n" _WHITE_, line);
    }
}

void TestSuccess(int flag, char *func_name)
{
    if (!flag)
    {
        printf(_CYAN_"\n\n %s " _GREEN_ "passed without a hitch\n\n" _WHITE_,func_name);
    }
}

#endif   /* __MY_TEST_UTIL_HEADER__ defined */
