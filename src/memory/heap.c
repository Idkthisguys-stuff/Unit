#include <stddef.h>

extern unsigned int kernel_end;
unsigned int heap_ptr = 0;

void heap_init(void) {
    heap_ptr = (unsigned int)&kernel_end;
}

void* kmalloc (size_t size) {
    size = (size + 3) & ~3;

    void* addr = (void*)heap_ptr;
    heap_ptr += size;
    return addr;
}

void kfree (void* ptr)  {}