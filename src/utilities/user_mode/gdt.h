#ifndef GDT_H
#define GDT_H

#include "config.h"
#include <stdint.h>


struct gdt_entry {
    uint16_t limit_low;      // Bits 0-15 del limite
    uint16_t base_low;       // Bits 0-15 del base
    uint8_t  base_middle;    // Bits 16-23 del base
    uint8_t  access;         // Access byte
    uint8_t  granularity;    // Flags + bits 16-19 del limite
    uint8_t  base_high;      // Bits 24-31 del base
} __attribute__((packed));


struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void gdt_set_gate(int num, u32 base, u32 limit, u8 access, u8 gran);
void gdt_install();

#endif