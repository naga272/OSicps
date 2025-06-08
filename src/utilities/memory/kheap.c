#include "kernel.h"
#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "../stdlib/stdlib.h"


struct heap         kernel_heap;
struct heap_table   kernel_heap_table;


void kheap_init()
{
    /*
    *   Funzione che definisce il numero totale di blocchi di heap disponibili per il kernel
    * */
    int total_table_entries     = OS_HEAP_SIZE_BYTES / OS_HEAP_BLOCK_SIZE;  // restituisce 256000
    kernel_heap_table.entries   = (HEAP_BLOCK_TABLE_ENTRY*) (OS_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total     = total_table_entries;

    void *end = (void*) (OS_HEAP_ADDRESS + OS_HEAP_SIZE_BYTES); // fine heap
    
    u32 response = heap_create(&kernel_heap, (void*) (OS_HEAP_ADDRESS), end, &kernel_heap_table);

    schecker(response != 0, "critical error! Impossibile allocare la memoria per il kernel");
}


void* kmalloc(size_t size) 
{
    return heap_malloc(&kernel_heap, size);
}


void kfree(void* ptr)
{
    heap_free(&kernel_heap, ptr);
}

