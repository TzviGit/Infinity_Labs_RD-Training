#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
  char i = 34;
  char j = 11;
  char k = 345;

  printf("%lu  %d\n\n", &i,i);
 printf("%lu  %d\n\n", &j, j);
 printf("%lu  %d\n\n", &k, k);


sleep(300);
 return 0;
}
