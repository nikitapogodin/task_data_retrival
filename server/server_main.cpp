#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <unistd.h>

#include "data_ret_server.hpp"

#include <getopt.h>

const int BUFFER_SIZE = 1024;

int process_request(char *buffer) {
  // TODO: process user request
  return 1;
}

void start_serve(int server_socket, DataRetServer &data_server) {
  // Accept connections and handle data
  while (true) {
    sockaddr_in client_addr{};
    socklen_t client_addr_l = sizeof(client_addr);

    int client_socket =
        accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_l);
    if (client_socket == -1) {
      std::cerr << "Error accepting connection\n";
      continue;
    }

    std::cout << "Client connected from " << inet_ntoa(client_addr.sin_addr)
              << ":" << ntohs(client_addr.sin_port) << std::endl;

    // Handle data from the client
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {

      buffer[bytes_read] = '\0';
      std::cout << "Received from client: " << buffer << std::endl;

      // TODO: implementa functionality
      data_server.getDataForSource(process_request(buffer));

      // Echo back to the client
      send(client_socket, buffer, bytes_read, 0);
    }

    // Close the client socket
    close(client_socket);
    std::cout << "Client disconnected\n";
  }
}

int main(int argc, char **argv) {

  int option;
  int option_index{0};
  int port{8080};
  int number_data_sources{2};
  int data_lifetime{50};

  static struct option long_options[] = {
      {"port", required_argument, nullptr, 'p'},
      {"data_sources_number", required_argument, nullptr, 'n'},
      {"data_lifetime", required_argument, nullptr, 't'},
      {nullptr, 0, nullptr, 0} // Required for the end of the options array
  };

  while ((option = getopt_long_only(argc, argv, "", long_options,
                                    &option_index)) != -1) {
    switch (option) {
    case 'p':
      port = std::stoi(optarg);
      break;
    case 'n':
      number_data_sources = std::stoi(optarg);
      break;
    case 't':
      data_lifetime = std::stoi(optarg);
      break;
    default:
      std::cout << "Unknown option" << std::endl;
      return 1;
    }
  }

  // TODO: replace with with you implementation
  DataRetServer data_server(number_data_sources, data_lifetime);

  // Create socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    std::cerr << "Error creating socket\n";
    return -1;
  }

  // Bind socket
  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) == -1) {
    std::cerr << "Error binding socket\n";
    close(server_socket);
    return -1;
  }

  // Listen for incoming connections
  if (listen(server_socket, 10) == -1) {
    std::cerr << "Error listening for connections\n";
    close(server_socket);
    return -1;
  }

  std::cout << "Server listening on port " << port << "...\n";

  start_serve(server_socket, data_server);

  // Close the server socket
  close(server_socket);

  return 0;
}
