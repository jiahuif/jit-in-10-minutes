#include "p1s.h"

#include <string.h>

void emit(struct code_block * block, uint8_t * code, size_t length)
{
    while (block->capacity < block->length + length)
    {
        block->capacity *= 2;
        block->code = (uint8_t *) realloc(block->code, block->capacity);
    }
    uint8_t * head = block->code + block->length;
    memcpy(head, code, length);
    block->length += length;
}
