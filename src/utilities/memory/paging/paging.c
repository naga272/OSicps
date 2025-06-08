#include "paging.h"
#include "utilities/memory/skalloc.h"
#include "utilities/flags.h"


void paging_load_directory(u32*);


static u32* current_directory = 0x00;


struct paging_4gb_chunk* paging_new_4gb(u8 flags)
{
    u32 offset = 0;
    u32* directory;

    skcalloc(directory, sizeof(u32) * PAGING_TOTAL_ENTRIES_X_TABLES);

    for (u32 i = 0; i < PAGING_TOTAL_ENTRIES_X_TABLES; i++) {
        u32* entry; 
        
        skcalloc(entry, sizeof(u32) * PAGING_TOTAL_ENTRIES_X_TABLES);
        
        for (u32 x = 0; x < PAGING_TOTAL_ENTRIES_X_TABLES; x++) {
            entry[x] = (offset + (x * PAGE_SIZE)) | flags;
        }

        offset += (PAGING_TOTAL_ENTRIES_X_TABLES * PAGE_SIZE);
        directory[i] = (u32) entry | flags | PAGING_IS_WRITEBLE;
    }

    struct paging_4gb_chunk* chunk_4gb; 
    skcalloc(chunk_4gb, sizeof(struct paging_4gb_chunk));

    chunk_4gb->directory_entry = directory;
    return chunk_4gb;
}


void switch_page(u32* directory) 
{
    paging_load_directory(directory);
    current_directory = directory;
}


u32* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk) 
{
    return chunk->directory_entry;
}


u8 paging_is_aligned(void* virtual_addr) 
{
    // @virtual_addr: mi assicuro che la virtual address e' aligned
    return ((u32) virtual_addr % PAGE_SIZE == 0);
}


int paging_get_index(void* virtual_addr, u32* directory_index, u32* table_index)
{
    int res = 0;
    if (!paging_is_aligned(virtual_addr)) {
        res -= EINVARG;
        goto out;
    }


    *directory_index = (u32) virtual_addr / (PAGING_TOTAL_ENTRIES_X_TABLES * PAGE_SIZE);
    *table_index = (u32) virtual_addr / (PAGING_TOTAL_ENTRIES_X_TABLES * PAGE_SIZE) / PAGE_SIZE; 

out:
    return res;
}


int paging_set(u32* directory, void* virtual_address, u32 val)
{
    int res = 0;

    if (!paging_is_aligned(virtual_addr)) {
        res -= EINVARG;
        goto out;
    }

    u32 directory_index = 0;
    u32 table_index = 0;
    res = paging_get_index(virtual_address, &directory_index, &table_index);
    
    if (res < 0) {
        return res;
    }

    u32 entry = directory[directory_index];
    u32* table = (u32*) (entry & 0xfffff000);
    table[table_index] = val;

out:
    return res;
}
