#include "stdlib.h"

void* memset(void* ptr, int c, size_t size)
{
    char *ptr_c = (char*) ptr;
    for (size_t i = 0; i < size; i++)
        ptr_c[i] = c;

    return ptr;
}