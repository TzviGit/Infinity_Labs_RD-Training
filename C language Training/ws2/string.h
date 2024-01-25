#ifndef STRING_H
#define STRING_H
#include <stddef.h>

size_t StrLen (const char *);
char *StrCpy(char *, const char *);
char *StrnCpy (char *, const char *, size_t n);
char *StrCat (char *, const char *);
char *StrnCat (char *, const char *, size_t);
char *StrDup (const char *);
char *StrStr (const char *, const char *);
char *StrChr (const char *,char);
void testStrDup(char *);
void testStrCat(char *, char *, char *);
void testStrnCat(char *, char *, size_t, char *result);
void testStrLen(char *, size_t );
void testStrStr(char *, char *, char* );
void testStrSpn(char *, char *, size_t );
void testPallindrome(char *,int );
size_t HowManyArgs(char **);
void CopyStrArr(char **, char **);
void PrintStrArr(char **);
void FreeStrArr(char **);
 void ToLower(char *);
 void swap(int *, int *);
 void CopyArray(const int *, int *, size_t );
 void AddNull(char *);
#endif
