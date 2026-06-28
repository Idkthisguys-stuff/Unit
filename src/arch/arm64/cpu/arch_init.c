#include "../include/arch_init.h"
#include "../include/memory.h"

void idt_init(void);

void arch_setup(void) {
    heap_init();
}

void kprint_char(char c) {
    volatile unsigned int *uart = (unsigned int *)0x09000000;
    *uart = c;
}