#include <stdint.h>

extern void interrupt_handler_33(void);

struct idt_gate {
    uint16_t pointer_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  attributes;
    uint16_t pointer_middle;
    uint32_t pointer_high;
    uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static struct idt_gate idt[256];
static struct idt_ptr idt_ptr;

void idt_set_gate(int vector, void *handler, uint8_t attributes) {
    uint64_t addr = (uint64_t)handler;
    idt[vector].pointer_low = (uint16_t)(addr & 0xFFFF);
    idt[vector].selector = 0x08; // Kernel Code segment from GDT
    idt[vector].ist = 0;
    idt[vector].attributes = attributes;
    idt[vector].pointer_middle = (uint16_t)((addr >> 16) & 0xFFFF);
    idt[vector].pointer_high = (uint32_t)((addr >> 32) & 0xFFFFFFFF);
    idt[vector].zero = 0;
}

void idt_init() {
    idt_set_gate(33, interrupt_handler_33, 0x8E);
    
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint64_t)&idt;

    __asm__ volatile("lidt %0" : : "m"(idt_ptr));
}