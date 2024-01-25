#include <assert.h> /*assert*/
#include <stddef.h> /*size_t*/

#include <stdlib.h>
#include <string.h>
#include "lib_memfuncs.h"


/* APPROVED_BY_FANIA*/
void *MemSet(void *s, int c, size_t n)
{
  unsigned char set = *(unsigned char *)&c;
  unsigned char *byteptr = (unsigned char *)s;
  size_t word_set = 0;
  size_t *wordptr = (size_t *)s;

  size_t w_set = (size_t)set;
  assert(s);

  word_set |= ((w_set | (w_set << 8)) | ((w_set | w_set << 8) << 16));
  if (8 == WORD_SIZE)
  {
      word_set |= word_set << 32; /*now hold 8 bytes worth of memory to set*/
  }

  while ( n && ((size_t)byteptr % WORD_SIZE))
  {
    *byteptr++ = set;
    --n;
  }

  wordptr = (size_t *)byteptr;
  while (n >= sizeof(size_t))
  {
      *wordptr++ = word_set;
      n -= WORD_SIZE;
  }

  byteptr = (unsigned char *)wordptr;

  while (n--)
  {
    *byteptr++ = set;
  }

  return s;
}

/*VADIM APROVED*/
void *MemCpy(void *dest, const void *src, size_t n) {
  size_t *wordptr_dest = (size_t *)dest, *wordptr_src = (size_t *)src;

  unsigned char *byteptr_dest = (unsigned char *)dest,
                *byteptr_src = (unsigned char *)src;


  assert(dest);
  assert(src);

  while ( n && ((size_t)byteptr_dest % WORD_SIZE))
  {
    *byteptr_dest++ = *byteptr_src++;
    --n;
  }

  wordptr_dest = (size_t *)byteptr_dest;
  wordptr_src = (size_t *)byteptr_src;

  while (n >= WORD_SIZE)
  {
    *wordptr_dest++ = *wordptr_src++;
    n -= sizeof(size_t);
  }

  byteptr_dest = (unsigned char *)wordptr_dest;
  byteptr_src = (unsigned char *)wordptr_src;

  while (n--)
  {
    *byteptr_dest++ = *byteptr_src++;
  }

  return dest;
}

/*APROVED BY VADIM   (and JOhny with caveat about not doing recursion*/
void *MemMove(void *dest, const void *src, size_t n)
{

    ptrdiff_t offset = ((char *)dest - (char *)src);

    if ((offset <= 0) || (offset > (ptrdiff_t)n))
    {
        return MemCpy(dest, src,n);
    }

    MemMove((void *)((char *)dest + offset), (void *)((char *)src + offset), (n - offset));

    MemCpy(dest, src,offset);

    return dest;


}
