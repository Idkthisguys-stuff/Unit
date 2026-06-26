#include "../include/arch_init.h"
#include "../include/gdt.h"
#include "../include/memory.h"

void idt_init(void);

void arch_setup(void) {
    gdt_init();
    idt_init();
    heap_init();
}