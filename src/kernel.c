#include "include/cpu.h"
#include "include/gdt.h"
#include "include/memory.h"

#define IDT_SIZE 256
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

int shift_pressed = 0;

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

char *vidptr = (char *)0xb8000;
unsigned int current_loc = 0;

unsigned char keyboard_map[128] = {
    0,   27,  '1',  '2',  '3',  '4', '5', '6',  '7', '8', '9', '0',
    '-', '=', '\b', '\t', 'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
    'o', 'p', '[',  ']',  '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
    'j', 'k', 'l',  ';',  '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm',  ',',  '.',  '/', 0,   '*',  0,   ' '};

unsigned char keyboard_map_shifted[128] = {
    0,   27,  '!',  '@',  '#',  '$', '%', '^',  '&', '*', '(', ')',
    '_', '+', '\b', '\t', 'Q',  'W', 'E', 'R',  'T', 'Y', 'U', 'I',
    'O', 'P', '{',  '}',  '\n', 0,   'A', 'S',  'D', 'F', 'G', 'H',
    'J', 'K', 'L',  ':',  '"',  '~', 0,   '|',  'Z', 'X', 'C', 'V',
    'B', 'N', 'M',  '<',  '>',  '?', 0,   '*',  0,   ' '};

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

void keyboard_handler_main(void) {
  unsigned char status;
  char keycode;
  status = read_port(KEYBOARD_STATUS_PORT);

  if (status & 0x01) {
    unsigned char scancode = read_port(KEYBOARD_DATA_PORT);

    if (scancode == 0x2A || scancode == 0x36) {
      shift_pressed = 1;
    } else if (scancode == (0x2A + 0x80) || scancode == (0x36 + 0x80)) {
      shift_pressed = 0;
    } else if (scancode < 0x80) {
      keycode = shift_pressed ? keyboard_map_shifted[scancode] : keyboard_map[scancode];

      if (keycode >= 0) {
        vidptr[current_loc++] = keycode;
        vidptr[current_loc++] = 0x07;
      }
    }
  }

  write_port(0x20, 0x20);
}

void kmain(void) {
  const char *str = "Unit Kernel";
  unsigned int i = 0;

  while (i < 80 * 25 * 2) {
    vidptr[i] = ' ';
    vidptr[i + 1] = 0x07;
    i = i + 2;
  }

  i = 0;
  while (str[i] != '\0') {
    vidptr[current_loc++] = str[i];
    vidptr[current_loc++] = 0x07;
    ++i;
  }

  gdt_init();
  idt_init();
  heap_init();

  while (1) {
    cpu_halt();
  }
}