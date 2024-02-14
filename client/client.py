import socket

HOST = '127.0.0.1'  # Server IP address
PORT = 8080         # Server port

# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the server
client_socket.connect((HOST, PORT))
print(f"Connected to {HOST}:{PORT}")

# TODO: replace with request to retrieve data
message = "1"
client_socket.send(message.encode())

# Receive and print the echoed response
response = client_socket.recv(1024).decode()
print(f"Received from server: {response}")

# Close the connection
client_socket.close()

