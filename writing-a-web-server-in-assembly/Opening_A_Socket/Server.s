.intel_syntax noprefix
.global _start

_start:
    # Allocate 4 bytes on the stack for the server socket
    sub rsp, 4
    
    mov rdi, 2 # socket domain = AF_INET (IPv4)
    mov rsi, 1 # socket type = SOCK_STREAM (TCP)
    mov rdx, 0 # socket protocol
    call Socket # call the socket syscall
    mov DWORD PTR [rsp], eax # error | socket is stored in eax
    
    cmp eax, 0 # check whether eax < 0
    jl  1f # jump to exit

    mov rdi, rax # prepare to close the socket stored in rax.
    call Close # call the close syscall

    xor rax, rax # rax = 0 # clear rax to close cleanly.

    1: mov rdi, rax # Exit with status code
    call Exit
