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
    # Allocate 1024 bytes for the file path [rsp + 1068]
    # Allocate 4 bytes for the file descriptor [rsp + 2092]
    sub rsp, 2096
    
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
        # This code only supports requests smaller than 1024 bytes.
        mov rcx, 47

        mov ebx, GET
        cmp ebx, DWORD PTR [rsp + 44]
        je 5f
        inc rcx
        # Read the path
        5:
            mov rbx, 1068
            mov BYTE PTR [rsp + rbx], '.'
        6:
            inc rcx
            cmp rcx, 1024
            je 8f
            mov dl, BYTE PTR [rsp + rcx]
            cmp dl, ' '
            je 7f
            inc rbx
            mov BYTE PTR [rsp + rbx], dl
            jmp 6b
        7:
            inc rbx
            cmp BYTE PTR [rsp + rbx - 1], '/'
            jne 7f
            mov rdx, INDEX
            mov QWORD PTR [rsp + rbx], rdx
            add rbx, 8
            lea rdx, INDEX
            mov dx, [rdx + 8]
            mov WORD PTR [rsp + rbx], dx
            add rbx, 2
        7:
            mov BYTE PTR [rsp + rbx], 0

        
        # Open the requested file

        lea rdi, [rsp + 1068]
        mov esi, 0
        mov edx, 0
        call Open
        mov [rsp + 2092], eax

        cmp eax, 0
        jl 9f # Jump to file not found
        
        lea rdi, A_RESPONSE
        call StrLen # calculate length of A_RESPONSE

        mov edi, DWORD PTR [rsp + 40] # fd = client socket
        lea rsi, A_RESPONSE # buf = A_RESPONSE
        mov rdx, rax # count = length of A_RESPONSE
        call Write

        4:
            mov edi, DWORD PTR [rsp + 2092] # fd = file fd
            lea rsi, [rsp + 44]
            mov rdx, 1024
            call Read

            mov edi, DWORD PTR [rsp + 40]
            lea rsi, [rsp + 44]
            mov edx, eax
            call Write

            cmp eax, 1024
            je  4b

        jmp 8f

    9:
        lea rdi, NOT_FOUND_RESPONSE
        call StrLen # calculate length of NOT_FOUND_RESPONSE

        mov edi, DWORD PTR [rsp + 40] # fd = client socket
        lea rsi, NOT_FOUND_RESPONSE # buf = NOT_FOUND_RESPONSE
        mov rdx, rax # count = length of NOT_FOUND_RESPONSE
        call Write         
    8:
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
A_RESPONSE: .asciz "HTTP/1.1 200 OK\r\nServer: SwitchCase\r\nConnection: close\r\n\r\n"
NOT_FOUND_RESPONSE: .asciz "HTTP/1.1 404 Not Found\r\nServer: SwitchCase\r\nConnection: close\r\n\r\nFile not found."
GET: .ascii "GET "
POST: .ascii "POST"
INDEX: .ascii "index.html"
