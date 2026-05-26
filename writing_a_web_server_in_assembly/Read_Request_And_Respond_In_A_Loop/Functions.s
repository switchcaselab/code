.intel_syntax noprefix
.global Write
.global Socket
.global Bind
.global Listen
.global Accept
.global Read
.global Close
.global Exit
.global StrLen

Write:
    mov rax, 1
    syscall
    ret

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

Read:
    mov rax, 0
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

StrLen:
    mov rax, 0
    1: 
        cmp BYTE PTR [rax + rdi], 0
        je  2f
        inc rax
        jmp 1b
    2:
        ret
