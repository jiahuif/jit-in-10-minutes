#include "tier0.h"

#include <stdint.h>


static int64_t * interpret(int64_t * result_ptr, int64_t * byte_code)
{
    int64_t * loop_end_ptr;
    while (1)
    {
        switch (*byte_code)
        {
        case 1:
            ++*result_ptr;
            break;
        case 2:
            ++byte_code;
            for (int i = 0 ; i < *byte_code ; ++i)
                loop_end_ptr = interpret(result_ptr, byte_code);
            byte_code = loop_end_ptr;
            break;
        case 3:
            return byte_code;
        case -1:
            return byte_code;
        }
        ++byte_code;
    }
    return byte_code;
}

int64_t tier0_interpret(int64_t * byte_code)
{
    int64_t ret = 0;
    interpret(&ret, byte_code);
    return ret;
}
