#include <stdio.h>
#include <string.h>
#include "fooBar.h"
#include <dlfcn.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int (*func)(int) = NULL;
    void *lib_handle = dlopen("./libFoo.so", RTLD_LAZY);
    *(void **)&func = dlsym(lib_handle, argv[1]);
    
    printf("The number is: %d\n", func(atoi(argv[2])));

    dlclose(lib_handle);
    return 0;
}