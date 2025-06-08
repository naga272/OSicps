#ifndef PAGING_H
#define PAGING_H

#include "config.h"

/*  FLAGS   */
#define PAGING_CACHE_DISABLED   0b00010000
#define PAGING_WRITE_THROUGH    0b00001000
#define PAGING_ACCESS_FROM_ALL  0b00000100
#define PAGING_IS_WRITEBLE      0b00000010
#define PAGING_IS_PRESENT       0b00000001


#define PAGING_TOTAL_ENTRIES_X_TABLES 1024
#define PAGE_SIZE 4096

struct paging_4gb_chunk {
    u32 *directory_entry;
};


struct paging_4gb_chunk* paging_new_4gb(u8);
void switch_page(u32*);
void enable_paging(void);
u32* paging_4gb_chunk_get_directory(struct paging_4gb_chunk*); 
u8 paging_is_aligned(void*);
int paging_set(u32*, void*, u32);

#endif
