import socket

s = socket.socket()
s.bind(("127.0.0.1", 1337))
s.listen(1)

while True:
    conn, addr = s.accept()
    conn.recv(1024)  # read the HTTP request, but do not parse it
    conn.sendall(
        b"HTTP/1.1 200 OK\r\n"
        b"Server: SwitchCase\r\n"
        b"Connection: close\r\n"
        b"\r\n"
        b"This is our first response."
    )
    conn.close()
