#include <stdint.h>

#define PIC1_COMMAND 0x20
#define PIC_EOI 0x20

extern uint32_t *global_fb_ptr;

// This function receives the interrupt vector number
void handle_interrupt(uint64_t vector) { 
    if (vector == 33) {
        for (int i = 0; i < 1000; i++) {
            global_fb_ptr[i] = 0xFFFF0000;
        }
    }

    __asm__ volatile ("outb %0, %1" : : "a"((uint8_t)0x20), "Nd"((uint16_t)0x20));
}