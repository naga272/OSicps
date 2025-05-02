#ifndef HEAP_H
    #define HEAP_H

#include "config.h"
#include <stdint.h>
#include <stddef.h>


/*
*   L'heap si suddivide in blocchi di memoria fissa.
*   Questa suddivisione mi permette di tenere traccia della memoria con l'@heap_table
*   senza dover gestire ogni singolo byte.
*   es. Se la malloc() chiede 1kb e ogni blocco di memoria vale 4kb, io alla malloc restituisco
*   comunque 4 kb. I restanti, quindi, vengono sprecati
*/


// costanti che rappresentano lo stato di un blocco di memoria nella tabella degli heap. 
// Un blocco puo' essere occupato o libero
#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00

// flag che utilizzo per capire se un blocco ha un blocco successivo oppure e' il primo
#define HEAP_BLOCK_HAS_NEXT  0b100000000
#define HEAP_BLOCK_IS_FIRST  0b010000000

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY; // rappresenta una voce della tabella degli heap


/*
* struct heap_table rappresenta la tabella degli heap.
* Tiene traccia di come è messo ogni blocco di memoria nell'heap pool 
* (quindi se il blocco e' libero o occupato, se fa parte di una sequenza, etc...).
*   @entries:   ogni voce rappresenta un blocco di memoria nell'heap pool.
*   @total:     numero totali di voci
*/
struct heap_table {
    HEAP_BLOCK_TABLE_ENTRY* entries;
    size_t total;
};


/*
* struct heap Rappresenta tutto l'heap disponibile per il kernel.
* @table: ptr alla tabella degli heap
* @saddr: start address del pool di memoria
*/
struct heap {
    struct heap_table* table;
    void* saddr;                // start address del data pool
};


u32 heap_create(struct heap* heap, void *ptr, void *end, struct heap_table* table);
void* heap_malloc(struct heap* heap, size_t size);
void heap_free(struct heap* heap, void* ptr);


#endif