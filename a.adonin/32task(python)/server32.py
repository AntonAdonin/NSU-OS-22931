import asyncio
import socket
import os
server_address = './uds_socket'


async def handle_client(client):
    loop = asyncio.get_event_loop()
    while True:
        try:
            data = await loop.sock_recv(client, 255)# Should be ready
        except ConnectionError:
            print(f"Client suddenly closed while receiving")
            return False
        print(f"Received {data.decode().upper()}")
        if not data:
            print("Disconnected")
            return False

async def run_server():
    try:
        os.unlink(server_address)
    except OSError:
        if os.path.exists(server_address):
            raise
    server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    server.bind(server_address)
    print(f'starting up on {server_address}')
    server.listen(5)
    server.setblocking(False)

    loop = asyncio.get_event_loop()
    while True:
        client, _ = await loop.sock_accept(server)
        print("client connected")
        loop.create_task(handle_client(client))

asyncio.run(run_server())