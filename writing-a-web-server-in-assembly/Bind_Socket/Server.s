.intel_syntax noprefix
.global _start

_start:
    # Allocate 4 bytes for the server socket [rsp]
    # Allocate 16 bytes for the sockaddr_in struct [rsp + 4]
    sub rsp, 20
    
    # Create a server socket.

    mov rdi, 2 # socket domain = AF_INET (IPv4)
    mov rsi, 1 # socket type = SOCK_STREAM (TCP)
    mov rdx, 0 # socket protocol
    call Socket # call the socket syscall
    mov DWORD PTR [rsp], eax # error | socket is stored in eax
    
    cmp eax, 0 # check whether eax < 0
    jl  1f # jump to exit

    # Bind the server socket to 0.0.0.0:1337
    mov WORD PTR [rsp + 4], 2 # sin_family = AF_INET
    mov ax, 1337 # Server port number
    xchg ah, al # little-endian to big-endian
    mov WORD PTR [rsp + 6], ax # sin_port = htons(1337)
    mov DWORD PTR [rsp + 8], 0 # sin_addr = 0
    mov QWORD PTR [rsp + 12], 0 # sin_zero = 0

    mov edi, DWORD PTR [rsp] 
    lea rsi, [rsp + 4]
    mov rdx, 16
    call Bind

    cmp eax, 0 # check if bind failed.
    jl 1f

    # Close the server socket 
    mov edi, [rsp] # prepare to close the socket stored in [rsp].
    call Close # call the close syscall

    xor rax, rax # rax = 0 # clear rax to close cleanly.

    1: mov rdi, rax # Exit with status code
    call Exit
