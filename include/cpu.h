#pragma once

static inline void cpu_halt(void) {
    #if defined(__x86_64__) || defined(__i386__)
    __asm__ __volatile__("hlt");
    #elif defined(__aarch64__)
    __asm__ __volatile__("wfi");
    #endif
}

static inline void cpu_cli(void) {
    #if defined(__x86_64__) || defined(__i386__)
        __asm__ __volatile__("cli");
    #elif defined(__aarch64__)
        __asm__ __volatile__("msr daifset, #2");
    #endif
}

static inline void cpu_sti(void) {
    #if defined(__x86_64__) || defined(__i386__)
        __asm__ __volatile__("sti");
    #elif defined(__aarch64__)
        __asm__ __volatile__("msr daifclr, #2");
    #endif
}