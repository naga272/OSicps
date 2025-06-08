
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include "utilities/stdlib/stdlib.h"
// #include "utilities/string/string.h"
// #include "utilities/memory/kheap.h"
#include "utilities/memory/skalloc.h"
#include "utilities/memory/paging/paging.h"
#include "utilities/idt/idt.h"
#include "utilities/io/io.h"

#define prototype_fun_video_h
#include "utilities/video/print/video.h"
// #include "utilities/user_mode/gdt.h" ancora in fase di test

#include "kernel.h"


// Dichiarazione della funzione Rust (programma main ICPS)
extern int icps_init();

static struct paging_4gb_chunk* kernel_chunk;

void kernel_main()
{
    terminal_initialize(get_actual_color_terminal());

    printk(KERN_DEBUG "inizializzazione dell'heap\n");
    kheap_init();


    printk(KERN_DEBUG "inizializzazione della idt\n");
    idt_init();


    printk(KERN_DEBUG "setup del paging\n");
    kernel_chunk = paging_new_4gb(PAGING_WRITE_THROUGH | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEBLE | PAGING_IS_PRESENT);
    switch_page(paging_4gb_chunk_get_directory(kernel_chunk));
    enable_paging();

    char* ptr;
    ptr = (char*) skcalloc(ptr, 4096);

    paging_set(
        paging_4gb_chunk_get_directory(kernel_chunk), 
        (void*) 0x1000, 
        (u32) ptr | PAGING_WRITE_THROUGH | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEBLE | PAGING_IS_PRESENT
    );


    enable_interrupts();
    printk(KERN_DEBUG "abilitati gli interrupts\n");


    printk(KERN_DEBUG "shell avviata\n");
    print((const uchar*) ">>>");
    // panic((const uchar*) "Error! per ora la BSoD si attiva di default, il kernel e' ancora in fase di progettazione\n");
}
