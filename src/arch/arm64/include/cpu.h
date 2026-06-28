#pragma once

static inline void cpu_halt(void) {
    __asm__ __volatile__("hlt");
}

static inline void cpu_cli(void) {
    __asm__ __volatile__("cli");
}

static inline void cpu_sti(void) {
    __asm__ __volatile__("sti");
}