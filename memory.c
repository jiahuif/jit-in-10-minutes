#include "p1s.h"

#include <stdio.h>

#if defined (unix)
#include <unistd.h>
#include <sys/mman.h>
#elif defined (WINDOWS)
#include <windows.h>
#endif

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

void free_page(void * ptr, size_t size)  {
	if (munmap(ptr, size) == -1)
        {
            perror("munmap");
            exit(EXIT_FAILURE);
        }
}

void protect_page(void * ptr, size_t size)
{
    if (mprotect(ptr, size, PROT_READ | PROT_EXEC) == -1)
    {
        perror("mprotect");
        exit(EXIT_FAILURE);
    }
}


