#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>

#include "p1s.h"

#include "tier0.h"
#include "tier1.h"

int64_t incr(int64_t x)
{
    return x + 1;
}

int main(int argc, char ** argv)
{
    struct code_block * block = create_code_block();
    FILE * text_source;
    if (argc == 1)
    {
        text_source = stdin;
    }
    else if (argc == 2)
    {
        text_source = fopen(argv[1], "r");
        if (!text_source)
        {
            perror("fopen");
            return EXIT_FAILURE;
        }
    } else {
    	fprintf(stderr, "Usage\n---------\nRead from stdin:\n\t%s\nRead from file:\n\t%s INFILE\n", argv[0], argv[0]);
    	return EXIT_FAILURE;
    }
    int64_t * byte_code = compile_text_file(text_source);
    if (!byte_code)
        return EXIT_FAILURE;
    tier1_generate_function(block, byte_code);
    GeneratedFunction f = (GeneratedFunction) make_block_executable(block);
    printf("%" PRId64 " " "%" PRId64 "\n", f(0), tier0_interpret(byte_code));
    destroy_code_block(block);
    return 0;
}

