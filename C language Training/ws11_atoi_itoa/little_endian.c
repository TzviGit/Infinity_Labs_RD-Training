#include "ws11_funcs.h"



int IsLittleEndian()
{
    unsigned int d = 1;
    return (0 != *(char *)&d);
}
