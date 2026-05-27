.intel_syntax noprefix
.global Exit

Exit:
    mov rax, 60
    syscall
