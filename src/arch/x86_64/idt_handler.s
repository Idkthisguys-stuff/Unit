.intel_syntax noprefix

.global interrupt_handler_33

interrupt_handler_33:
    # Save state
    push rax
    push rbx
    push rcx
    push rdx

    # Pass the vector number (33) to the C function
    mov rdi, 33
    call handle_interrupt

    # Restore state
    pop rdx
    pop rcx
    pop rbx
    pop rax
    
    # Return from interrupt
    iretq