#include <stdio.h>  /* printf() */
#include <stdlib.h> /* malloc() */
#include <math.h>
static void StaticFunc(void)
{
    int a = 0;
    int b = 0;
    a += b;
}

extern int ExternFunc(void);


int non_const_global_var = 8;
const int const_global_var = 4;
static int static_non_const_global_var = 87;
static const int static_const_global_var = 47;

int main(int argc, char **argv, char **envp)
{
    void *heap_var1 = malloc(1);
    void *heap_var2 = malloc(1);
    void *heap_var3 = malloc(1);
    void *heap_var4 = malloc(1);
    void *heap_var5 = malloc(1);
    void *heap_var6 = malloc(1);
    sin(34);

    /*void *heap_var1 = NULL;
    void *heap_var2 = NULL;
    int non_const_local_var = 3;
    const int const_local_var = 9;
    static int static_non_const_local_var = 38;
    static const int static_const_local_var = 99;
    char *str_literal_ptr = "string_literal";

    StaticFunc();
    ExternFunc();

    heap_var1 = malloc(1);
    heap_var2 = malloc(1);

    (void)heap_var1;
    (void)heap_var2;
    (void)non_const_local_var;
    (void)const_local_var;
    (void)static_non_const_local_var;
    (void)static_const_local_var;
    (void)static_non_const_global_var;
    (void)static_const_global_var;
    (void)str_literal_ptr;
    (void)argc;*/

    printf("\n\nFirst_Environment_variable = \"%s\"\n\n", envp[0]);
    printf("\n\nFirst_Command-line_argument = \"%s\"\n\n", argv[0]);

    free(heap_var1);
    heap_var1 = NULL;
    free(heap_var2);
    heap_var2 = NULL;

    return 0;
}
