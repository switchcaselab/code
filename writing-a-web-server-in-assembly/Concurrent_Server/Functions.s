.intel_syntax noprefix
.global Read
.global Write
.global Open
.global Socket
.global Bind
.global Listen
.global Accept
.global Close
.global Fork
.global Exit
.global RT_Signal
.global IgnoreSignal
.global RegisterSignalHandler
.global StrLen

Read:
    mov rax, 0
    syscall
    ret

Write:
    mov rax, 1
    syscall
    ret

Open:
    mov rax, 2
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

Close:
    mov rax, 3
    syscall
    ret

Fork:
    mov rax, 57
    syscall
    ret

Exit:
    mov rax, 60
    syscall
    # ret

RT_Signal:
    mov rax, 13
    syscall
    ret

IgnoreSignal:
    sub rsp, 32 # reserve stack
    mov QWORD PTR[rsp], 1 # handler = 1 (SIG_IGN)
    mov QWORD PTR[rsp + 8], 0 # sa_flags = 0
    mov QWORD PTR[rsp + 16], 0 # sa_restorer = 0
    mov QWORD PTR[rsp + 24], 0 # sa_mask = 0

    mov rsi, rsp
    mov rdx, 0
    mov r10, 8
    call RT_Signal
    add rsp, 32 # release stack
    ret

RT_SigReturn:
    mov rax, 15
    syscall
    ret

RegisterSignalHandler:
    sub rsp, 32 # reserve stack
    mov QWORD PTR[rsp], rsi # handler = 1 (SIG_IGN)
    mov QWORD PTR[rsp + 8], 0x04000000 # sa_flags = 0x04000000 (SA_RESTORER)
    lea rax, RT_SigReturn # sa_restorer = 0
    mov QWORD PTR[rsp + 16], rax
    mov QWORD PTR[rsp + 24], 0 # sa_mask = 0

    mov rsi, rsp
    mov rdx, 0
    mov r10, 8
    call RT_Signal
    add rsp, 32 # release stack
    ret


StrLen:
    mov rax, 0
    1: 
        cmp BYTE PTR [rax + rdi], 0
        je  2f
        inc rax
        jmp 1b
    2:
        ret
