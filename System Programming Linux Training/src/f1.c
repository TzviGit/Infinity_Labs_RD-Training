#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
  char i = 7;
  char j = 3;
  char k = 15;
char *p= &j;
  printf("%lu  %d\n\n", &i,i);
 printf("%lu  %d\n\n", &j, j);
 printf("%lu  %d\n\n", &k, k);
printf("%lu  %lu\n\n", &p, p);

 sleep (300);

 return 0;
}
