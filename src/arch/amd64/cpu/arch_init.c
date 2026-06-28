#include "../include/arch_init.h"
#include "../include/gdt.h"
#include "../include/memory.h"

#include "../../../../include/vga.h"

void idt_init(void);

void arch_setup(void) {
    //gdt_init();
    idt_init();
    heap_init();
}

void kprint_char(char c) {
    vidptr[current_loc++] = c;
    vidptr[current_loc++] = 0x07;
}