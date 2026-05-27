.intel_syntax noprefix
.global Socket
.global Bind
.global Close
.global Exit

Socket:
    mov rax, 41
    syscall
    ret    

Bind:
    mov rax, 49
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
