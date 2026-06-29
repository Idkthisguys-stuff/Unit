.section .text
.global _start

_start:
    ldr x0, =stack_top
    mov sp, x0

    bl kmain

_halt:
    wfi
    b _halt

.section .bss
.align 4
stack_space:
    .skip 8192
stack_top:

