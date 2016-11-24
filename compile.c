#include <stdio.h>

#include "p1s.h"

int64_t * compile_text_file(FILE* f)
{
    char * token;
    while ( fscanf(f, "%ms", &token) != EOF ) {
        fprintf(stderr, "%s\n", token);
        free(token);
    }
    return NULL;
}

