#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/
#include <string.h>/*memmove*/
#include "circular_buffer.h" /*func prototypes*/


#define BUFFER_BASE(cbuff_pointer) (char *)((char *)(cbuff_pointer) + (sizeof(cbuff_t) - sizeof(size_t)))
#define READ_IND(cbuff_p) ((cbuff_p)->head)
#define WRITE_POINTER(cbuff_p) (char *)(BUFFER_BASE(cbuff_p) + WRITE_IND(cbuff_p))
#define READ_POINTER(cbuff_p) (char *)(BUFFER_BASE((cbuff_p) + READ_IND((cbuff_p))))

#define INCREMENT_HEAD(cbuff_p, amount) ((cbuff_p)->head =  ((cbuff_p)->head + (size_t)(amount)) % (cbuff_p)->capacity)
#define MIN2(a,b) ((a) * ((a) <= (b)) + ((b) * ((b) < (a))))
#define MIN3(a, b, c) MIN2(MIN2(a, b), (c))
#define WRITE_IND(cbuff_p) ((cbuff_p)->head + (cbuff_p)->size) % (cbuff_p)->capacity

struct cb {
    size_t head;
    size_t size;
    size_t capacity;
    char buf[1];
};


/********APROVED__BY __VADIM*******/

cbuff_t *CBCreate(size_t capacity)
{
    cbuff_t *cbuff = (cbuff_t *)malloc(capacity + (sizeof(cbuff_t) - sizeof(size_t)));
    if (!cbuff)
    {
        return NULL;
    }
    cbuff->head =  0;
    cbuff->size = 0;
    cbuff->capacity = capacity;

    return cbuff;
}

void CBDestroy(cbuff_t *cbuff)
{
    free(cbuff);
}

size_t CBFreeSpace(const cbuff_t *cbuff)
{
    assert(cbuff);
    return (cbuff->capacity - cbuff->size);
}

size_t CBBufsiz(const cbuff_t *cbuff)
{
    assert(cbuff);
    return cbuff->capacity;
}

ssize_t CBRead(cbuff_t *cbuff, void *buf, size_t count)
{
    ssize_t result = 0;
    size_t chunk = 0;
    void *src = NULL, *dest = buf;
    assert(cbuff && buf);

    src = READ_POINTER(cbuff);
    chunk = MIN3(count,(cbuff->capacity - READ_IND(cbuff)) , cbuff->size);
    memmove(dest, src, chunk);

    cbuff->size -= chunk;
    INCREMENT_HEAD(cbuff, chunk);
    count -= chunk;
    result += chunk;
    dest = (void *)((char *)buf + chunk);
    src = READ_POINTER(cbuff);
    chunk = MIN2(count, cbuff->size);
    memmove(dest, src, chunk);

    cbuff->size -= chunk;
    INCREMENT_HEAD(cbuff, chunk);
    result += chunk;

    return result;
}

ssize_t CBWrite(cbuff_t *cbuff, const void *buf, size_t count)
{
    ssize_t result = 0;
    size_t chunk = 0;
    const void *src = buf;
    void *dest = NULL;
    assert(cbuff && buf);

    dest = (void *)WRITE_POINTER(cbuff);
    chunk = MIN3(count,(cbuff->capacity - WRITE_IND(cbuff)) , CBFreeSpace(cbuff));
    memmove(dest, src, chunk);

    cbuff->size += chunk;
    count -= chunk;
    result += chunk;
    src = (void *)((char *)buf + chunk);

    dest = (void *)WRITE_POINTER(cbuff);
    chunk = MIN2(count, CBFreeSpace(cbuff));
    memmove(dest, src, chunk);
    cbuff->size += chunk;
    result += chunk;

    return result;
}


int CBIsEmpty(const cbuff_t *cbuff)
{
    assert(cbuff);
    return !cbuff->size;
}


char *CBToString(cbuff_t *cbuff, char *buffer)
{
    size_t len = 0, index = 0;
    char *ptr = buffer, *read_ptr = NULL;
    assert(cbuff && buffer);

    len = cbuff->size;

    while (index < len)
    {
        read_ptr = BUFFER_BASE(cbuff) + ((cbuff->head + index) % CBBufsiz(cbuff));
        *ptr++ = *read_ptr;
        ++index;
    }

    *ptr = '\0';
    return buffer;
}
