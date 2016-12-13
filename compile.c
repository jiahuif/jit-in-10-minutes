#include <stdio.h>

#include "p1s.h"

int64_t * compile_text_file(FILE* f)
{
    char token[255];
    while ( fscanf(f, "%255s", token) != EOF )
    {
        fprintf(stderr, "%s\n", token);
    }
    return NULL;
}

