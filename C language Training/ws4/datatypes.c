#include <stdio.h>

int main()
{

char a = 'a';
unsigned char b = 'b';
short c = 1;
unsigned short d = 1;
int e = 2;
unsigned int f = 2;
long g = 100;
unsigned long h = 400;
float i = 3434.4;
double j = 43.4;
long double k = 34344.55;
size_t l = 5;
printf("size of char is %lu\n\n",sizeof(a));
printf("size of unsigned char is %lu\n\n",sizeof(b));
printf("size of short is %lu\n\n",sizeof(c));
printf("size of unsigned short is %lu\n\n",sizeof(d));
printf("size of int is %lu\n\n",sizeof(e));
printf("size of unsigned int is %lu\n\n",sizeof(f));
printf("size of long is %lu\n\n",sizeof(g));
printf("size of unsigned long is %lu\n\n",sizeof(h));
printf("size of float is %lu\n\n",sizeof(i));
printf("size of double is %lu\n\n",sizeof(j));
printf("size of long double is %lu\n\n",sizeof(k));
printf("size of size_t is %lu\n\n",sizeof(l));
return (0);
}
