#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>

#include "p1s.h"


static inline void emit_code(int64_t ** byte_code, size_t* len, int64_t code)
{
    *byte_code = realloc(*byte_code, (*len + 1) * sizeof(int64_t));
    (*byte_code)[*len] = code;
    ++*len;
}

static inline void print_error(size_t index, const char * msg, int64_t * byte_code)
{
    fprintf(stderr, "syntax error: %s at token %zu\n", msg, index);
    free(byte_code);
}

int64_t * compile_text_file(FILE* f)
{
    char token[255];
    char last_token[255];
    int64_t * byte_code = malloc(0);
    size_t len = 0;
    size_t index = 0;
    int64_t nested_loop_depth = 0;
    while ( fscanf(f, "%255s", token) != EOF )
    {
        ++index;
        if (strcmp("+1s", token) == 0)
        {
            //+1s
            emit_code(&byte_code, &len, 1);
        }
        else if (strcmp("people", token) == 0)
        {
            // repeat
            int64_t repeat_token = 0;
            sscanf(last_token, "%" SCNd64, &repeat_token);
            emit_code(&byte_code, &len, 2);
            emit_code(&byte_code, &len, repeat_token);

            ++nested_loop_depth;
        }
        else if (strcmp("do", token) == 0)
        {
            if (strcmp("people", last_token) != 0)
            {
                // no people before do
                print_error(index, "expect 'people' before 'do'", byte_code);
                return NULL;
            }
        }
        else if (strcmp("done", token) == 0)
        {
            // repeat end.
            --nested_loop_depth;
            if (nested_loop_depth < 0)
            {
                print_error(index, "no matching 'do' for 'done'", byte_code);
                return NULL;
            }
            emit_code(&byte_code, &len, 3);
            ++len;
        }
        strcpy(last_token, token);
    }
    if (nested_loop_depth != 0)
    {
        print_error(index, "no matching 'done' for 'do'", byte_code);
        return NULL;
    }
    emit_code(&byte_code, &len, -1);
    ++len;
    return byte_code;
}

