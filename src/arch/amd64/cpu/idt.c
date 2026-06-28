#include "../include/cpu.h"
#include <stdint.h>

#define IDT_SIZE 256

void write_port(unsigned short port, unsigned char data);
unsigned char read_port(unsigned short port);
void load_idt(void *ptr);
void keyboard_handler(void);

struct idt_ptr_struct {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

struct IDT_entry {
    uint16_t offset_lowerbits;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attr;
    uint16_t offset_mid;
    uint32_t offset_higherbits;
    uint32_t reserved;
} __attribute__((packed));

struct IDT_entry IDT[IDT_SIZE];
struct idt_ptr_struct idt_ptr;

void idt_init(void) {
  uint64_t keyboard_address = (uint64_t)keyboard_handler;

  for (int i = 0; i < IDT_SIZE; i++) {
    IDT[i].type_attr = 0;
  }

  IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
  IDT[0x21].selector = 0x08;
  IDT[0x21].offset_mid = (keyboard_address >> 16) & 0xffff;
  IDT[0x21].type_attr = 0x8e;
  IDT[0x21].offset_higherbits = (keyboard_address >> 32) & 0xffffffff;

  /* ICW1 - begin initialization */
  write_port(0x20, 0x11);
  write_port(0xA0, 0x11);

  /* ICW2 - remap offset address of IDT */
  write_port(0x21, 0x20);
  write_port(0xA1, 0x28);

  /* ICW3 - setup cascading correctly */
  write_port(0x21, 0x04);
  write_port(0xA1, 0x02);

  /* ICW4 - environment info */
  write_port(0x21, 0x01);
  write_port(0xA1, 0x01);

  /* mask interrupts - enable only keyboard (IRQ1 -> bit 1 cleared) */
  write_port(0x21, 0xFD);
  write_port(0xA1, 0xFF);

  idt_ptr.limit = (sizeof(struct IDT_entry) * IDT_SIZE) - 1;
  idt_ptr.base = (uint64_t)&IDT;

  load_idt(&idt_ptr);
}