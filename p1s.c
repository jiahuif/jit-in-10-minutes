#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>

#include "p1s.h"

#include "tier1.h"

int64_t incr(int64_t x)
{
    return x + 1;
}

int main()
{
    struct code_block * block = create_code_block();
    int64_t byte_code[] = {2, 10 , 2, 10, 1,  1, 3 , 3, -1};
    tier1_generate_function(block, byte_code);
    GeneratedFunction f = (GeneratedFunction) make_block_executable(block);
    printf("%" PRId64 "\n", f(0));
    destroy_code_block(block);
    return 0;
}

