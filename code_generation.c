#include "p1s.h"

#include <string.h>

uint8_t init_code[7] =
{
    0x48, 0xc7, 0xc2, 0x00, 0x00, 0x00, 0x00, // mov $0x0, %rdx
};

uint8_t function_call_code[18] =
{
    0x48, 0x89, 0xd7,                               // mov %rdx, %rdi

    0x48, 0xb8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // movabs $func, $rax
    // actual address of $func (8 bytes unsigned)
    // should be filled at offset +5

    0xff, 0xd0,                                     // callq *%rax
    0x48, 0x89, 0xc2,                               // mov %rax, %rdx
};

uint8_t return_code[4] =
{
    0x48, 0x89, 0xd0, // mov %rdx, %rax
    0xc3              // ret
};

uint8_t loop_begin_code[26] =
{
    0x48, 0xb9, 0x00, 0x00, 0x00,       // movabsq    $count,%rcx
    0x00, 0x00, 0x00, 0x00, 0x00,
    // actual value of $count (8 bytes signed)
    // should be filled at offset +2

    0x51,                               // push %rcx
    0x59,                               // START: pop %rcx
    0x48, 0xff, 0xc9,                   // dec %rcx
    0x48, 0x83, 0xf9, 0x00,             // cmp %0x0, %rcx

    0x0f, 0x8c, 0x00, 0x00, 0x00, 0x00, // jl <.END>
    // actual offset of .END (4 bytes signed)
    // should be filled at offset +21

    0x51                                // push %rcx
};

uint8_t loop_end_code[6] =
{
    0xe9, 0x00, 0x00, 0x00, 0x00, // jmp <.START>
    // actual offset of .START (4 bytes signed)
    // should be filled at offset + 1

    0x90                          // END: nop
};


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
