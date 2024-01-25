#ifndef __MY_STRING_H__
#define __MY_STRING_H__
#include <stddef.h>

size_t StrLen (const char *);
char *StrCpy(char *, const char *);
char *StrnCpy (char *, const char *, size_t n);
char *StrCat (char *, const char *);
char *StrnCat (char *, const char *, size_t);
char *StrDup (const char *);
char *StrStr (const char *, const char *);
char *StrChr (const char *,char);


void CopyStrArr(char **, char **);
void PrintStrArr(char **);
void FreeStrArr(char **);
 void ToLower(char *);
 void swap(int *, int *);
 void CopyArray(const int *, int *, size_t );
 void AddNull(char *);

 char *GetNextWord(char *word);
 void StrReverse(char *str);
 void ReverseWord(char *word);
 void ReverseChars(char *start, size_t how_many);
 size_t WordLen(char *word);
 void CharSwap(char *c1, char *c2);

#endif
