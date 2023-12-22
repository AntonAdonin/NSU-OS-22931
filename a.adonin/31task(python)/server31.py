import multiprocessing
import select
import socket
import sys
import os
import threading

server_address = './uds_socket'

def handle_connection(sock):
    try:
        data = sock.recv(1024)  # Should be ready
    except ConnectionError:
        print(f"Client suddenly closed while receiving")
        return False
    print(f"Received {data.decode().upper()} from: {addr}")
    if not data:
        print("Disconnected by", addr)
        return False
    return True


if __name__ == "__main__":
    try:
        os.unlink(server_address)
    except OSError:
        if os.path.exists(server_address):
            raise
    with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as serv_sock:
        serv_sock.bind(server_address)
        print(f'starting up on {server_address}')
        serv_sock.listen(5)
        serv_sock.setblocking(False)
        inputs = [serv_sock]
        outputs = []
        while True:
            print("Waiting for connections or data...")
            readable, writeable, exceptional = select.select(inputs, outputs, inputs)
            for sock in readable:
                if sock == serv_sock:
                    sock, addr = serv_sock.accept()  # Should be ready
                    print("Client connected")
                    sock.setblocking(False)
                    inputs.append(sock)
                else:
                    if not handle_connection(sock):
                        # Disconnected
                        inputs.remove(sock)
                        if sock in outputs:
                            outputs.remove(sock)
                        sock.close()