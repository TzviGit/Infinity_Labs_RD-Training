/*****************************************************************************

Author:         Amatzia Safra
Company:		Infinity
Reviewer:       Fani
Date:           19.12.21
Group:          OL110
Description:    Reference counting string (using "copy on right" way)

*****************************************************************************/

#include <cstdio>       // printf
#include <iostream>     // ostream

#include "rcstring.hpp"

#define G "\33[32;1m"
#define R "\33[1;31m"
#define N "\33[0m"

#define FAIL(func) do { TestsFailed(1);                                                     \
                       printf("\n\33[1;31m%s\33[0m failed at line %d\n", #func, __LINE__);  \
                   } while(0);

#define TEST(test) do { TestsRun(1); if (!(test)){ is_successful = 0; FAIL(test); }	        \
                   } while(0);

#define FUNCTEST(func) do {                                                                 \
                           if (func){ printf("\n%s %s %s succeeded.", G, #func, N); }       \
                           else { printf("\n%s %s failed.%s", R, #func, N); }               \
                       } while(0);


static int Test1(void);

static size_t TestsRun(int add);
static size_t TestsFailed(int add);
static void SummarizeTests(void);



int main()
{
    FUNCTEST(Test1())

    SummarizeTests();

    return 0;
}

using namespace ilrd;

static int Test1(void)
{
    int is_successful = 1;

    RCString s("Hello");
    TEST(s.Length() == 5)
    TEST(s == "Hello")

    RCString s2(s);
    TEST(s2 == s)
    TEST(s2.Cstr() == s.Cstr())
    s2 = "World";
    TEST(s2 != s)
    TEST(s2 > s)
    TEST(s < s2)

    s = "Help";
    s2 = s;
    TEST(s2 == s)
    TEST(s2.Cstr() == s.Cstr())

    TEST(s[1] == 'e');
    TEST(s2 == s)
    
    s[1] = 'o';
    s[3] = 'e';
    TEST(s2 != s)
    std::cout << "s: " << s << "\n";
    std::cout << "s2: " << s2 << "\n";

    std::cout << "Please insert a string: \n";
    std::cin >> s2;
    std::cout << "new s2: " << s2 << "\n";

    return is_successful;
}



static size_t TestsRun(int add)
{
    static size_t tests = 0;

    tests += add;

    return tests;
}

static size_t TestsFailed(int add)
{
    static size_t failures = 0;

    failures += add;

    return failures;
}

static void SummarizeTests(void)
{
    size_t tests = TestsRun(0);
    size_t failures = TestsFailed(0);

    printf("\n\n\n\n-------------------------------------------------------");

    if (0 == failures)
    {
        printf("\n\n%s %ld tests done succeessfully. %s", G, tests, N);
    }
    else
    {
        printf("\n\n%ld tests,%s %ld failures %s", tests, R, failures, N);
    }

    printf("\n\n");
}

