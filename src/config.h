#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include "./utilities/flags.h"

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#ifndef OS_TOTAL_INTERRUPTS
    #define OS_TOTAL_INTERRUPTS 512  // l'os ha 512 interrupts totali
#endif

#define OS_HEAP_SIZE_BYTES      104857600 // 100 MB
#define OS_HEAP_BLOCK_SIZE      4096
#define OS_HEAP_ADDRESS         0x01000000
#define OS_HEAP_TABLE_ADDRESS   0x07E00

// numero massimo caratteri per comando dalla shell
#define OS_SIZE_SHELL_COMMAND 2048 

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned char uchar;


/*
* schecker (secure checker) consente di eseguire dei controlli e di mostrare un messaggio di errore 
* nel caso in cui x valga piu' di 0 (cosa errata). 
*/

extern void panic(const uchar*);

#define schecker(x, msg_error)                  \
        if (x) {                                \
            panic((const uchar*) msg_error);    \
        }      

#endif