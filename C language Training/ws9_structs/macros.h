#ifndef MACROS_H
#define MACROS_H

/*#define APPROVED_BY_FANIA(this) (printf("%s\n", #this will work really well!))*/

#define MAX2(a,b) (((a) > (b)) ? (a) : (b))
#define MAX3(a, b, c) MAX2(MAX2(a, b), (c))
#define SIZE_OF_VAR(var) ((char *)(&var + 1) - (char *)(&var))
#define SIZE_OF_TYPE(type) ((char *)(((type *)1000) + 1) - (char *)(type *)1000)

int MYDEFINE = 0;

#endif /*define macros.h*/
