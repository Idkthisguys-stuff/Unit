#include "../include/cpu.h"

#define IDT_SIZE 256

void write_port(unsigned short port, unsigned char data);
unsigned char read_port(unsigned short port);
void load_idt(void *ptr);
void keyboard_handler(void);

struct idt_ptr_struct {
  unsigned short limit;
  unsigned int base;
} __attribute__((packed));

struct IDT_entry {
  unsigned short int offset_lowerbits;
  unsigned short int selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short int offset_higherbits;
} __attribute__((packed));

struct IDT_entry IDT[IDT_SIZE];
struct idt_ptr_struct idt_ptr;

void idt_init(void) {
  unsigned long keyboard_address = (unsigned long)keyboard_handler;

  for (int i = 0; i < IDT_SIZE; i++) {
    IDT[i].type_attr = 0;
  }

  IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
  IDT[0x21].selector = 0x08;
  IDT[0x21].zero = 0;
  IDT[0x21].type_attr = 0x8e;
  IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

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
  idt_ptr.base = (unsigned int)&IDT;

  load_idt(&idt_ptr);
}