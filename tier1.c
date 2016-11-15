#include "tier1.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include "p1s.h"

static code_generator loop_processor, instruction_generator;

static int64_t * create_loop(struct code_block * block, int64_t * byte_code)
{
    int start_index = block->length;
    emit(block, loop_begin_code, sizeof(loop_begin_code));
    assert ( * (int32_t *) (block->code + start_index + 2) == 0);
    ++byte_code;
    * (int32_t *) (block->code + start_index + 2) = *byte_code;

    byte_code = instruction_generator(block, byte_code + 1);

    int end_index = block->length;
    emit(block, loop_end_code, sizeof(loop_end_code));
    assert( *((uint8_t *) block->code + start_index + 20) == 0x8c);
    assert( *((uint8_t *) block->code + end_index) == 0xe9);
    * (int32_t *) (block->code + start_index + 21) = (int32_t) ( (end_index + 5) - (start_index + 25));
    /* jmp offset = address of target instruction - address of next instruction */
    * (int32_t *) (block->code + end_index + 1) = (int32_t) ((start_index + 11) - (end_index + 5) );
    return byte_code;

}

static int64_t * generate_instructions(struct code_block * block, int64_t * byte_code)
{
    while (1)
    {
        if (*byte_code == 1)
        {
            // +1s
            emit(block, function_call_code, sizeof(function_call_code));
        }
        else if (*byte_code == 2)
        {
            // loop start
            byte_code = loop_processor(block, byte_code);
        }
        else if (*byte_code == 3)
        {
            // loop end
            return byte_code;
        }
        else if (*byte_code == -1)
        {
            // exit
            return byte_code;
        }
        ++byte_code;
    }
    return byte_code;
}

void tier1_generate_function(struct code_block * block, int64_t * byte_code)
{
    loop_processor = &create_loop;
    instruction_generator = &generate_instructions;
    *((void **)(function_call_code + 5)) = (void *) &incr;
    emit(block, init_code, sizeof(init_code));
    generate_instructions(block, byte_code);
    emit(block, return_code, sizeof(return_code));
}


