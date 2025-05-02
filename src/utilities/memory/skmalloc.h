#ifndef SKMALLOC_H
    #define SKMALLOC_H

#include "config.h"
#include "kheap.h"
#include "heap.h"

extern struct heap kernel_heap;

/* 
* Secure kernel malloc 
* Macro che consente di evitare di dover fare il controllo del ptr nullo
* Si trova all'interno di un do-while per evitare errori di sintassi in caso di annidamento if. 
* Se per esempio scriviamo il codice in questo modo senza do-while: 
    void* ptr;
    if(something)
        skmalloc(ptr, 12)
    else
        do_something
*
* Il compilatore dara' errore di sintassi. Per evitare questo lo metto all'interno di un do {} while (0)
*/

#define skmalloc(ptr, size)                                                     \
    do {                                                                        \
        ptr = heap_malloc(&kernel_heap, size);                                  \
        schecker(ptr == NULL, (const uchar*) "errore nell'allocare la memoria");\
    } while (0);


#endif