#ifndef LIBRARY_H
#define LIBRARY_H
#include <stddef.h>

#define WORD_SIZE sizeof(size_t)
void *MemCpy(void *, const void *, size_t);
void *MemSet(void *, int, size_t);
void *MemMove(void *, const void *, size_t);

 #endif
