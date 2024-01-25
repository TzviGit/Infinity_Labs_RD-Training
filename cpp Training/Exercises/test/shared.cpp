
#include <stdio.h>
char name[] = "FOO";

#ifdef __cplusplus
extern "C" {
#endif

/* Function declaration */
char*GetFuncName();
int FOO(int);

void Bar();
int Biz();



#ifdef __cplusplus
}
#endif





char* GetFuncName()
{
    return name;
}

int FOO(int a)
{
    printf("HEY >> COOL << WERE << IN << THE << FUNCTON!!! <<<>>>\n\n");
    return 569;
}

void Bar(){}

int Biz()
{
    return 76;
}
