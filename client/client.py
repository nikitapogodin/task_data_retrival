import socket
import time


HOST = '127.0.0.1'  # Server IP address
PORT = 8080         # Server port


def request_data(socket, source_index):
    # Request data
    message = f"{source_index}"
    socket.send(message.encode())
    # Receive response
    return socket.recv(1024).decode()


def request_one(socket, source_index):
    start = time.perf_counter()
    response = request_data(socket, source_index)
    elapsed_time = time.perf_counter() - start
    print(f"Received from server: {response}, time: {elapsed_time*1e3:.2f} ms")


def request_loop(socket, source_index, count, delay_ms):
    start = time.perf_counter()
    for i in range(0, count):
        response = request_data(socket, source_index)
        elapsed_time = time.perf_counter() - start
        print(f"[{int(elapsed_time*1e3)}] Received from server: {response}")
        time.sleep(delay_ms/1000)


if __name__ == '__main__':
    # Create a socket object
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connect to the server
    print(f"Connecting to {HOST}:{PORT}...")
    client_socket.connect((HOST, PORT))
    print(f"Connected")

    # request_one(socket, 1)
    request_loop(client_socket, 1, 100, 50)

    # Close the connection
    client_socket.close()
