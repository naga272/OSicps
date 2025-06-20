#ifndef KHEAP_H
    #define KHEAP_H

#include <stddef.h>
#include "config.h"

// funzione che inizializza l'heap del kernel.
void kheap_init();
void* skcalloc(size_t);
void* kcalloc(size_t);
void kfree(void*);

#endif