bits 32

section .multiboot
        align 4
        dd 0x1BADB002
        dd 0x00
        dd - (0x1BADB002 + 0x00)

section .text
global start
extern kmain

start:
    cli
    mov esp, stack_space

    mov eax, pdpt
    or eax, 0b11
    mov [pml4], eax

    mov eax, pd
    or eax, 0b11e
    mov [pdpt], eax

    mov eax, 0b10000011
    mov [pd], eax

    mov eax, pml4
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    lgdt [gdt64.pointer]
    jmp gdt64.code:start64

bits 64
start64:
    mov ax, gdt64.data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    call kmain
    hlt
global read_port
global write_port
global load_idt
global keyboard_handler
extern keyboard_handler_main

read_port:
    mov dx, di
    xor rax, rax
    in al, dx
    ret

write_port:
    mov dx, di
    mov ax, si
    out dx, al
    ret

load_idt:
    mov rdi, rdi
    lidt [rdi]
    sti
    ret

keyboard_handler:
    push rax
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11

    cld
    call keyboard_handler_main

    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax
    iretq

section .bss
align 4096
pml4:
    resb 4096
pdpt:
    resb 4096
pd:
    resb 4096

resb 8192
stack_space:

section .rodata
gdt64:
    dq 0
.code: equ $ - gdt64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.data: equ $ - gdt64
    dq (1 << 41) | (1 << 44) | (1 << 47)
.pointer:
    dw $ - gdt64 - 1
    dq gdt64