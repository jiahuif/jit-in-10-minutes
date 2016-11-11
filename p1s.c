#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096

void * alloc_page(size_t size)
{
    void * ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap");
        return NULL;
    }
    return ptr;
}

void protect_page(void * ptr, size_t size)
{
    if (mprotect(ptr, size, PROT_READ | PROT_EXEC) == -1)
    {
        perror("mprotect");
        exit(EXIT_FAILURE);
    }
}

typedef long (*JittedFunc)(long);

long incr(long x)
{
    return x + 1;
}

struct code_block
{
    unsigned char * code;
    size_t capacity;
    size_t length;
};

static unsigned char init_code[] =
{
    0x48, 0xc7, 0xc2, 0x00, 0x00, 0x00, 0x00, // mov $0x0, %rdx
};

static unsigned char function_call_code[] =
{
    0x48, 0x89, 0xd7,                               // mov %rdx, %rdi
    0x48, 0xb8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // movabs $func, $rax
    0xff, 0xd0,                                     // callq *%rax
    0x48, 0x89, 0xc2,                               // mov %rax, %rdx
};

static unsigned char return_code[] =
{
    0x48, 0x89, 0xd0,                         // mov %rdx, %rax
    0xc3                                      // ret
};

static unsigned char loop_begin_code[] =
{
    0x48, 0xb9, 0x00, 0x00, 0x00,        // movabsq    $count,%rcx
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x51,                                // push %rcx
    0x59,                                // START: pop %rcx
    0x48, 0xff, 0xc9,                    // dec %rcx
    0x48, 0x83, 0xf9, 0x00,              // cmp %0x0, %rcx
    0x0f, 0x8c, 0x00, 0x00, 0x00, 0x00,  // jl <.END>
    0x51                                 // push %rcx
};

static unsigned char loop_end_code[] =
{
    0xe9, 0x00, 0x00, 0x00, 0x00, // jmp <.START>
    0x90        // END: nop
};

void emit(struct code_block * block, unsigned char * code, size_t length)
{
    while (block->capacity < block->length + length)
    {
        block->capacity *= 2;
        block->code = (unsigned char *) realloc(block->code, block->capacity);
    }
    unsigned char * head = block->code + block->length;
    memcpy(head, code, length);
    block->length += length;
}

long * generate_instructions(struct code_block * block, long * byte_code)
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
            int start_index = block->length;
            emit(block, loop_begin_code, sizeof(loop_begin_code));
            assert ( * (long *) (block->code + start_index + 2) == 0);
            ++byte_code;
            * (long *) (block->code + start_index + 2) = *byte_code;
            byte_code = generate_instructions(block, byte_code + 1);
            int end_index = block->length;
            emit(block, loop_end_code, sizeof(loop_end_code));
            assert( *((unsigned char *) block->code + start_index + 20) == 0x8c);
            assert( *((unsigned char *) block->code + end_index) == 0xe9);
            * (int *) (block->code + start_index + 21) = (int) ( (end_index + 5) - (start_index + 25)); // address of target instruction - address of next instruction
            * (int *) (block->code + end_index + 1) = (int) ((start_index + 11) - (end_index + 5) );
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

void generate_function(struct code_block * block, long * byte_code)
{
    emit(block, init_code, sizeof(init_code));
    generate_instructions(block, byte_code);
    emit(block, return_code, sizeof(return_code));
}

void * make_block_executable(struct code_block * block)
{
    size_t size = ( (block->length - 1)/ PAGE_SIZE + 1 ) * PAGE_SIZE;
    void * ptr = alloc_page(size);
    memcpy(ptr, block->code, block->length);
    protect_page(ptr, size);
    return ptr;
}

struct code_block * create_code_block()
{
    struct code_block * block = malloc(sizeof(struct code_block));
    block->capacity = 1;
    block->length = 0;
    block->code = malloc(1);
    return block;
}

int main()
{
    assert(*((void **)(function_call_code + 5)) == NULL);
    *((void **)(function_call_code + 5)) = (void *) &incr;
    struct code_block * block = create_code_block();
    long byte_code[] = {2, 10 , 2, 10, 1,  1, 3 , 3, -1};
    generate_function(block, byte_code);
    JittedFunc f = (JittedFunc) make_block_executable(block);
    printf("%d\n", f(0));
    return 0;
}

