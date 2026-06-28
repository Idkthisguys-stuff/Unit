#pragma once
#include <stdint.h>

struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char  base_middle;
    unsigned char  access;
    unsigned char  granularity;
    unsigned char  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint64_t base;
}__attribute((packed));

extern struct gdt_entry gdt[3];
extern struct gdt_ptr gp;

void gdt_init(void);

extern void gdt_flush(unsigned int);