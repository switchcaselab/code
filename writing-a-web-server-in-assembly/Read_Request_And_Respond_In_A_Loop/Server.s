.intel_syntax noprefix
.global _start

.text
_start:
    # Allocate 4 bytes for the server socket [rsp]
    # Allocate 16 bytes for the server sockaddr_in struct [rsp + 4]
    # Allocate 16 bytes for the client sockaddr_in struct [rsp + 20]
    # Allocate 4 bytes for the client addrlen [rsp + 36]
    # Allocate 4 bytes for the client socket [rsp + 40]
    # Allocate 1024 bytes for the read buffer [rsp + 44]
    sub rsp, 1068
    
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

    mov edi, DWORD PTR [rsp] # sockfd = server socket
    lea rsi, [rsp + 4] # addr = server sockaddr
    mov rdx, 16 # addrlen = 16
    call Bind

    cmp eax, 0 # check if bind failed.
    jl 1f

    # Listen on the server socket
    mov edi, DWORD PTR [rsp] # sockfd = server socket
    mov esi, 0 # backlog = 0
    call Listen

    cmp eax, 0 # check if listen failed.
    jl 1f
    
    3:
        # Accept the first connection
        mov edi, DWORD PTR [rsp]  # sockfd = server socket
        lea rsi, [rsp + 20] # addr = client addr
        lea rdx, [rsp + 36] # addrlen = client addrlen
        call Accept   
        mov DWORD PTR [rsp + 40], eax # error | socket is stored in eax

        cmp eax, 0 # check if accept failed.
        jl 2f
        
        4: 
            # Read the client request
            mov edi, DWORD PTR [rsp + 40] # fd = client socket
            lea rsi, [rsp + 44]
            mov rdx, 1024        
            call Read
            cmp eax, 1024
            je  4b
        
        lea rdi, A_RESPONSE
        call StrLen # calculate length of A_RESPONSE

        mov edi, DWORD PTR [rsp + 40] # fd = client socket
        lea rsi, A_RESPONSE # buf = A_RESPONSE
        mov rdx, rax # count = length of A_RESPONSE
        call Write 

        # Close the client socket 
        mov edi, [rsp + 40] # prepare to close the socket stored in [rsp].
        call Close # call the close syscall
        jmp 3b
    2: 
        # Close the server socket 
        mov edi, [rsp] # prepare to close the socket stored in [rsp].
        call Close # call the close syscall     
    1:
        mov rdi, rax # Exit with status code
        call Exit

.data
A_RESPONSE: .asciz "HTTP/1.1 200 OK\r\nServer: SwitchCase\r\nConnection: close\r\n\r\nThis is our first response."
