/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      Fania Volkof
* Description   varying size allocator (VSA) module
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#include <assert.h>
#include "variable_size_allocator.h"



#ifndef NDEBUG

#define UTIL_BLOCK 2
static const size_t VSA_CONFIRM = 0xCABBBBAB;
#define GET_VSA_CONFIRM(pointer) *((long *)(pointer) - 1)

#endif

#ifndef UTIL_BLOCK
#define UTIL_BLOCK 1
#endif

#define VSA_STRUCT_WORDS 1
#define WORD_SIZE 8
#define END_OF_POOL (long) 0xDEADBEEFDEADBEEF   /* < 0*/
#define END_BLOCK_WORDS 1

struct vsa
{
    long internal_pool[1];
};




static size_t ABS(long num);
static long *VSAUpdateUtilBlock(long *util_block, long val);
static void VSADefragmentPool(vsa_t *vsa);
static void VSADefragmentSegment(long *block);
static long *VSANextFreeBlock(const long *current_block);
static long *VSANextBlock(const long *current_block);



/* O(1) */
/* initiates a vsa; To avoid UB use a size of at least 24 step_bytes
*/
/* input: pool: receives allocated memory        */
/*        pool_size:  it's size in bytes         */
/* returns: an initialized and ready for use vsa */

vsa_t *VSAInit(void *pool, size_t pool_size)
{
    vsa_t *vsa = NULL;
    size_t reserved_block = ( END_BLOCK_WORDS + UTIL_BLOCK);
    size_t num_of_words = 0;
    long *init_ptr = NULL;
    long *end_block = NULL;

    if (pool_size < WORD_SIZE *reserved_block)
    {
        return NULL;
    }

    num_of_words = (pool_size / WORD_SIZE) - reserved_block;

    assert(pool);

    init_ptr = (long *)pool;
    end_block = (init_ptr + UTIL_BLOCK + num_of_words);

    init_ptr = VSAUpdateUtilBlock(init_ptr, num_of_words);
    *(end_block) = END_OF_POOL;

    vsa = (vsa_t *)pool;
    vsa->internal_pool[0] = *init_ptr;

    return vsa;

}

/* O(n) */
/* input: vsa:  receives a vsa                                  */
/*        size: a size of block to allocate                     */
/* returns: a pointer to an allocated block of the desired size */
/*            or NULL if doesn't exist                          */
void *VSAAlloc(vsa_t *vsa, size_t size)
{
    size_t size_in_words = (size / WORD_SIZE) + !!(size % WORD_SIZE);
    long *alloc_ptr = NULL;


    assert(vsa);

    alloc_ptr = (long *)vsa;
    for (; *alloc_ptr != END_OF_POOL; alloc_ptr = VSANextBlock(alloc_ptr))
    {
        long remainder = 0;
        alloc_ptr = VSANextFreeBlock(alloc_ptr);
        VSADefragmentSegment(alloc_ptr);

        remainder = *alloc_ptr - size_in_words;
        if (remainder >= 0)
        {
            if (remainder - UTIL_BLOCK > 0)
            {
                long *new_block = NULL;
                VSAUpdateUtilBlock(alloc_ptr, -size_in_words);
                new_block = VSANextBlock(alloc_ptr);
                VSAUpdateUtilBlock(new_block, (remainder - UTIL_BLOCK));
            }
            else
            {
                VSAUpdateUtilBlock(alloc_ptr, -(*alloc_ptr));
            }

            return (void *)(alloc_ptr + UTIL_BLOCK);
        }
    }

    return NULL;
}


void VSAFree(void *block)
{
    long *util = NULL;
    assert(block);
    assert(VSA_CONFIRM == GET_VSA_CONFIRM(block));

    util = ((long *)block - UTIL_BLOCK);
    VSAUpdateUtilBlock(util, -(*util));

}

/* O(n) */
/* input: receives a vsa */
/* returns: the largest contiguous block size that can be allocated */
size_t VSALargestBlockAvailable(vsa_t *vsa)
{
    size_t max_size = 0;
    long *block_ptr = NULL;
    assert(vsa);

    /**FIRST DEFRAGMENT THE MEMMORY**/
    VSADefragmentPool(vsa);

    block_ptr = (long *)vsa;

    while (*block_ptr != END_OF_POOL)
    {
        block_ptr = VSANextFreeBlock(block_ptr);

        if (*block_ptr > (long)max_size)
        {
            max_size = *block_ptr;
        }

        block_ptr = VSANextBlock(block_ptr);
    }

    return (WORD_SIZE * max_size);
}

static long *VSAUpdateUtilBlock(long *util_block, long val)
{
    *util_block = val;

#ifndef NDEBUG
*(util_block + 1) = VSA_CONFIRM;
#endif


    return util_block;
}

static long *VSANextBlock(const long *current_block)
{
    if(*current_block != END_OF_POOL)
    {
        return (long *)(current_block + UTIL_BLOCK + ABS(*(current_block)));
    }
    else
    {
        return (long *)current_block;
    }
}

static long *VSANextFreeBlock(const long *current_block)
{
    long *temp_block = (long *)current_block;
    while( *temp_block < 0 && *temp_block != END_OF_POOL)
    {
        temp_block = VSANextBlock(temp_block);
    }

    return temp_block;
}

void VSADefragmentSegment(long *block)
{
    long temp = *VSANextBlock(block);

    while (temp > 0)
    {
        *block += UTIL_BLOCK + temp;
        temp = *VSANextBlock(block);
    }

}

static void VSADefragmentPool(vsa_t *vsa)
{
    long *temp_block = VSANextFreeBlock((long *)vsa);

    for (; *temp_block != END_OF_POOL; temp_block = VSANextFreeBlock(temp_block))
    {
        VSADefragmentSegment(temp_block);
        temp_block = VSANextBlock(temp_block);
    }
}

static size_t ABS(long num)
{
    return (num > 0 ? num : -num);
}
