
#ifndef STDLIB_H
    #define STDLIB_H

#include <stddef.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#if defined(CODE_FOR_HEAP)
    void* current_address = (void*) (0x1000000);
#endif


void* memset(void* ptr, int c, size_t size);

#endif