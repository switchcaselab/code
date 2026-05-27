.intel_syntax noprefix
.global Socket
.global Close
.global Exit

Socket:
    mov rax, 41
    syscall
    ret

Close:
    mov rax, 3
    syscall
    ret

Exit:
    mov rax, 60
    syscall
    # ret
