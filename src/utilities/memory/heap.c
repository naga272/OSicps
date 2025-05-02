#define CODE_FOR_HEAP // serve per accedere a funzione di stdlib
#include "kernel.h"
#include "utilities/stdlib/stdlib.h"
#include "heap.h"


/*
* NB: MAI USARE DIRETTAMENTE KMALLOC.
* ho creato una macro in skmalloc.h chiamata skmalloc(void *ptr, size_t size) (sta per secure kernel malloc)
* Questa chiama la kmalloc e in automatico esegue il controllo se il ptr e' nullo. 
* Questo consente di non dimenticarsi il controllo da fare a ogni data pool. 
*   Esempio:
    void *ptr;
    skmalloc(ptr, 12);  

    // do something with pointer, and than:

    kfree(ptr);
*/

static inline u32 heap_validate_table(void* ptr, void* end, struct heap_table* table)
{
    /*
    *   1) Calcolca la dimensione totale dell'area di memoria
    *   2) Calcola il numero di blocchi che ci sono
    *   3) Confronta col valore total della struct table
    *  Se il numero di blocchi calcolato non corrisponde a table->total, restituisce errore: -EINVARG
    **/
    u32 res = 0;

    size_t table_size = (size_t)(end - ptr);
    size_t total_blocks = table_size / OS_HEAP_BLOCK_SIZE;
    if (table->total != total_blocks)
    {
        res = -EINVARG;
        goto out;
    }

out:
    return res;
}


static u32 heap_validate_alignment(void* ptr)
{
    /* Verifica allineamento del blocco */
    return ((u32)ptr % OS_HEAP_BLOCK_SIZE) == 0;
}


u32 heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table)
{
    /* 
    *   1) Verifica allineamento degli indirizzi
    *   2) Azzera la struct heap
    *   3) Imposta l'indirizzo iniziale e la tabella
    *   4) Inizializza la tabella marcando tutti i blocchi liberi
    *   In caso !heap_validate_alignment() risulta 0, restituisco errore (-EINVARG)
    */
    u32 res = 0;

    if (!heap_validate_alignment(ptr) || !heap_validate_alignment(end))
    {
        res = -EINVARG;
        goto out;
    }

    memset(heap, 0, sizeof(struct heap));
    heap->saddr = ptr;
    heap->table = table;

    res = heap_validate_table(ptr, end, table);
    if (res < 0)
    {
        goto out;
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

out:
    return res;
}


static inline u32 heap_align_value_to_upper(u32 val)
{
    /*
    *   Allinea una dimensione multipla di OS_HEAP_BLOCK_SIZE 
    */
    if ((val % OS_HEAP_BLOCK_SIZE) == 0) {
        return val;
    }

    val = (val - ( val % OS_HEAP_BLOCK_SIZE));
    val += OS_HEAP_BLOCK_SIZE;
    return val;
}


static inline u32 heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry)
{
    /*
    *   estraggo il blocco dalle voci della tabella heap 
    */
    return entry & 0x0f;
}


u32 heap_get_start_block(struct heap* heap, u32 total_blocks)
{
    /*
    *   cerca total_blocks consecutivi liberi restituendo l'offset del primo blocco
    **/
    struct heap_table* table = heap->table;
    u32 bc = 0;
    u32 bs = -1;

    for (size_t i = 0; i < table->total; i++) {
        if (heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE) {
            bc = 0;
            bs = -1;
            continue;
        }

        // If this is the first block
        if (bs == -1) {
            bs = i;
        }
        bc++;
        if (bc == total_blocks) {
            break;
        }
    }

    if (bs == -1) {
        return -ENOMEM;
    }
    
    return bs;

}


void* heap_block_to_address(struct heap* heap, u32 block)
{
    
    /*
    *  converte un numero di blocco in un indirizzo di memoria
    **/
    return heap->saddr + (block * OS_HEAP_BLOCK_SIZE);
}


void heap_mark_blocks_taken(struct heap* heap, u32 start_block, u32 total_blocks) 
{
    /*
    * Consente di marcare i blocchi come occupati
    */
    u32 end_block = (start_block + total_blocks)-1;
    
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if (total_blocks > 1) {
        entry |= HEAP_BLOCK_HAS_NEXT;
    }

    for (u32 i = start_block; i <= end_block; i++) {
        heap->table->entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if (i != end_block -1) {
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}


void* heap_malloc_blocks(struct heap* heap, u32 total_blocks)
{
    /*
    * Trova i blocchi liberi tramite heap_get_start_block, ottiene l'offset iniziale in address e li marca come occupati
    */
    void* address = 0;

    u32 start_block = heap_get_start_block(heap, total_blocks);
    if (start_block < 0) {
        goto out;
    }

    address = heap_block_to_address(heap, start_block);

    // Mark the blocks as taken
    heap_mark_blocks_taken(heap, start_block, total_blocks);

out:
    return address;
}


void heap_mark_blocks_free(struct heap* heap, u32 starting_block)
{
    /*
    * Libera i blocchi partendo da starting_block 
    */
    struct heap_table* table = heap->table;
    
    for (u32 i = starting_block; i < (u32) table->total; i++) {
        
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        
        if (!(entry & HEAP_BLOCK_HAS_NEXT)) {
            break;
        }
    }
}


u32 heap_address_to_block(struct heap* heap, void* address)
{
    /* Converte l'indirizzo in un numero determinato di blocchi */
    return ((u32) (address - heap->saddr)) / OS_HEAP_BLOCK_SIZE;
}


void* heap_malloc(struct heap* heap, size_t size)
{
    /* Alloca dei blocchi di memoria, restituendo l'offset iniziale del primo blocco*/
    size_t aligned_size = heap_align_value_to_upper(size);
    u32 total_blocks = aligned_size / OS_HEAP_BLOCK_SIZE;
    return heap_malloc_blocks(heap, total_blocks);
}


void heap_free(struct heap* heap, void* ptr)
{
    /* Libera la memoria allocata */
    heap_mark_blocks_free(heap, heap_address_to_block(heap, ptr));
}