#include "../include/cpu.h"
#include "../include/arch_init.h"

void kprint_char(char c);

void kprint(const char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    kprint_char(str[i]);
  }
}

char *vidptr = (char *)0xb8000;
unsigned int current_loc = 0;

void kmain(void) {
  kprint("UNIT KERNEL \n");

  arch_setup();

  while (1) {
    cpu_halt();
  }
}

// a comment to test out ssh signing