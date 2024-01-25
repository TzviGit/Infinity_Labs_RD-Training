#include <cstdio>       // printf


#define _B_ "\33[1;34m"
#define _G_ "\33[1;32m"
#define _R_ "\33[1;31m"
#define _N_ "\33[0m"

#define FAIL(func) do { TestsFailed(1);                                                     \
                       printf("\33[1;31m" #func "\33[0m failed at line %d\n", __LINE__);    \
                   } while(0);

#define TEST(test) do { TestsRun(1); if (!(test)){ is_successful = 0; FAIL(test); }	        \
                   } while(0);

#define FUNCTEST(func) do {                                                                 \
                           printf(_B_ "\n" #func ":\n\n" _N_);                              \
                           if (func){ printf("\n" _G_ #func " succeeded.\n" _N_); }         \
                           else { printf(_R_ #func " failed.\n" _N_); }                     \
                           printf("--------\n");                                            \
                       } while(0);

size_t TestsRun(int add);
size_t TestsFailed(int add);
void SummarizeTests(void);

