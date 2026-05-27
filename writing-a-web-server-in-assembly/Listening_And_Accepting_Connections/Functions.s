.intel_syntax noprefix
.global Socket
.global Bind
.global Listen
.global Accept
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

Listen:
    mov rax, 50 
    syscall
    ret

Accept:
    mov rax, 43
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
