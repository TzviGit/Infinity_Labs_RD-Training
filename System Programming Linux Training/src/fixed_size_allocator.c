/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      SHACHAR SHOSHANI
* Description   Fixed SIze Allocator module
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/
#include <assert.h> /*assert*/

#include <stdio.h>
#include "fixed_size_allocator.h"


#define WORD_ALLIGNMENT sizeof(size_t)
#define UTILITY_BLOCK 8


size_t WordAllignedPadding(size_t block_size);

fsa_t FSAInit(void *mem_pool, size_t pool_size, size_t block_size)
{
    size_t actual_block_size = block_size + WordAllignedPadding(block_size);
    size_t next_block = UTILITY_BLOCK + actual_block_size, offset = 0;
    size_t *initializer = (size_t *)mem_pool + 1;


    assert(mem_pool);
    *(size_t *)mem_pool = 0;
    if (actual_block_size && pool_size >= (sizeof(size_t) + actual_block_size))
    {
        *(size_t *)mem_pool = sizeof(size_t);
        pool_size -= UTILITY_BLOCK;

        while (pool_size >= 2 * actual_block_size)
        {
            *(initializer + offset) = next_block;
            offset += actual_block_size / sizeof(size_t);
            next_block += actual_block_size;
            pool_size -= actual_block_size;
        }

        *(initializer + offset) = 0;
    }
    return (fsa_t)mem_pool;
}

void *FSAAlloc(fsa_t fsa)
{
    void *block = NULL;
    assert(fsa);

    if (*(size_t*)fsa) /* if pool isn't full */
	{
        size_t offset = *(size_t *)fsa;
        block = (void *)((char *)fsa + offset);
        *(size_t *)fsa = *(size_t *)block;
	}

	return block;
}

void FSAFree(fsa_t fsa, void *block)
{
    size_t temp = 0;
    assert(fsa);
    assert(block);

    temp = *(size_t *)fsa;
    *(size_t *)fsa = (size_t )((char *)block - (char *)fsa);
    *(size_t *)block = temp;
}


size_t FSASuggestSize(size_t num_of_blocks, size_t block_size)
{
    size_t actual_block_size = block_size + WordAllignedPadding(block_size);

    return (actual_block_size * num_of_blocks) + UTILITY_BLOCK;
}

size_t FSACountFree(const fsa_t fsa)
{
    size_t count = 0;
    size_t next_offset = 0;
    assert(fsa);

    next_offset = *(size_t *)fsa;

    while (next_offset)
    {
        ++count;
        next_offset = *(size_t *)((char *)fsa + next_offset);
    }

    return count;
}



size_t WordAllignedPadding(size_t block_size)
{
    size_t remainder = block_size % WORD_ALLIGNMENT;
    return remainder ? WORD_ALLIGNMENT - remainder : remainder;
}
