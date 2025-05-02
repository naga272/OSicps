
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include "utilities/stdlib/stdlib.h"
// #include "utilities/string/string.h"
// #include "utilities/memory/kheap.h"
#include "utilities/memory/skmalloc.h"
#include "utilities/idt/idt.h"
#include "utilities/io/io.h"

#include "kernel.h"

#define prototype_fun_video_h
#include "utilities/video/print/video.h"


// var per la shell
extern unsigned char keyboard_buffer[OS_SIZE_SHELL_COMMAND];
extern u32 counter_buffer;


// Dichiarazione della funzione Rust (programma main ICPS)
extern int icps_init();


void kernel_main()
{
    terminal_initialize(get_actual_color_terminal());

    kheap_init();
    idt_init();

    enable_interrupts();

    void *ptr;
    skmalloc(ptr, 12);  // restituisco un blocco (4095 bytes)
    kfree(ptr);         // libero i 4096 bytes

    // print((const uchar*) ">>>");
    panic((const uchar*) "Error! per ora la BSoD si attiva di default, il kernel e' ancora in fase di progettazione\n");
}
