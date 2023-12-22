import socket
import sys

# Create a UDS socket
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

server_address = './uds_socket'
print(f'connecting to {server_address}')
try:
    sock.connect(server_address)
except socket.error as msg:
    print(msg)
    sys.exit(1)

try:

    print("write 'q' to end connection")
    message = input("input message: ")
    while message.strip() != "q":
        print(f'sending {message}')
        sock.sendall(message.encode())
        message = input("input message: ")

except Exception as error:
    print(error)

finally:
    print('closing socket')
    sock.close()