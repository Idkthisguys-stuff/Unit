#include <stdint.h>

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static struct gdt_entry gdt[3];
static struct gdt_ptr gdt_ptr;

void gdt_init() {
    // Null Entry
    gdt[0] = (struct gdt_entry){0, 0, 0, 0, 0, 0};

    // Kernel Code (Access: 0x9A, Granularity: 0xAF for 64-bit)
    gdt[1] = (struct gdt_entry){0, 0, 0, 0x9A, 0xAF, 0};
    
    // Kernel Data (Access: 0x92, Granularity: 0xCF)
    gdt[2] = (struct gdt_entry){0, 0, 0, 0x92, 0xCF, 0};

    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base = (uint64_t)&gdt;

    // Load

    __asm__ volatile("lgdt %0" : : "m"(gdt_ptr));
}