#include "arch/x86/include/arch_init.h"
#include "arch/x86/include/cpu.h"

char *vidptr = (char *)0xb8000;
unsigned int current_loc = 0;

void kmain(void) {
  const char *str = "UNIT KERNEL";
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

  arch_setup();

  while (1) {
    cpu_halt();
  }
}