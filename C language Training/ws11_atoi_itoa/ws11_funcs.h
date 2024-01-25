#ifndef WS11FUNCS_H
#define WS11FUNCS_H
#include <stddef.h>

int CharToDigit(char , int );
int Atoi( const char *, int );
char DigToChar(int );
unsigned int SizeInBase(int , int );
char *Itoa(int value, char *, int  );
int CompareAtoi( const char *);
void TestAtoi();
void TestItoa(int , char *, int , char *);
void PrintLetFirst2Arr(char *, size_t, char *, size_t , char *, size_t );
int IsLittleEndian();
#define IS_LITTLE_ENDIAN ((*(unsigned int *)"\xFF\0\0\0" == 0xff ? 1 : 0))

#endif
