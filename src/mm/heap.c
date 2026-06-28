#include <stddef.h>
#include <stdint.h>

extern unsigned int kernel_end;
unsigned int heap_ptr = 0;

void heap_init(void) {
    heap_ptr = (uintptr_t)&kernel_end;
}

void* kmalloc (size_t size) {
    size = (size + (sizeof(void*) - 1)) & ~(sizeof(void*) - 1);

    void* addr = (void*)heap_ptr;
    heap_ptr += size;
    return addr;
}

void kfree (void* ptr)  {}