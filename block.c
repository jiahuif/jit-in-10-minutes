#include "p1s.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>



void * make_block_executable(struct code_block * block)
{
    size_t size = ROUND_TO_PAGE(block->length); // round up to nearest page size
    void * ptr = alloc_page(size);
    memcpy(ptr, block->code, block->length);
    protect_page(ptr, size);
    block->executable_code = ptr;
    return ptr;
}

struct code_block * create_code_block()
{
    struct code_block * block = malloc(sizeof(struct code_block));
    block->capacity = 1;
    block->length = 0;
    block->code = malloc(1);
    block->executable_code = NULL;
    return block;
}

void destroy_code_block(struct code_block * block)
{
    if (block->code)
    {
        free(block->code);
        block->code = NULL;
    }
    if (block->executable_code)
    {
        free_page(block->executable_code, ROUND_TO_PAGE(block->length));
        block->executable_code = NULL;
    }
    free(block);
}

