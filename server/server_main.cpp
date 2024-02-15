
#include "data_server.hpp"
#include "exceptions.hpp"

#include <arpa/inet.h>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <unistd.h>

const int BUFFER_SIZE = 1024;

namespace {

int processRequest(const char* buffer) {
    return std::stoi(buffer);
}

std::string makeOkResponse(int data) {
    return R"({"status": "OK", "data": )" + std::to_string(data) + "}";
}

std::string makeErrorResponse(const std::string& message) {
    return R"({"status": ")" + message + R"(", "data": null})";
}

void start_serve(int server_socket, server::DataServer& data_server) {
    // Accept connections and handle data
    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_addr_l = sizeof(client_addr);

        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_l);
        if (client_socket == -1) {
            std::cerr << "Error accepting connection\n";
            continue;
        }

        std::cout << "Client connected from " << inet_ntoa(client_addr.sin_addr) << ":"
                  << ntohs(client_addr.sin_port) << std::endl;

        // Handle data from the client
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
            buffer[bytes_read] = '\0';
            std::cout << "Received from client: " << buffer << std::endl;

            // Parse request
            auto index = processRequest(buffer);

            // Get data
            std::string response;
            try {
                auto data = data_server.getDataForSource(index);
                response = makeOkResponse(data);
            } catch (std::exception& ex) {
                std::cout << "Failed to process request: " << ex.what() << std::endl;
                response = makeErrorResponse(ex.what());
            }

            // Send response
            std::cout << "Sending to client: " << response << std::endl;
            send(client_socket, response.c_str(), response.size(), 0);
        }

        // Close the client socket
        close(client_socket);
        std::cout << "Client disconnected\n";
    }
}

} // namespace

int main(int argc, char** argv) {
    int option;
    int option_index{0};
    int port{8080};
    int number_data_sources{2};
    int data_lifetime{250};

    static struct option long_options[] = {
        {"port", required_argument, nullptr, 'p'},
        {"data_sources_number", required_argument, nullptr, 'n'},
        {"data_lifetime", required_argument, nullptr, 't'},
        {nullptr, 0, nullptr, 0} // Required for the end of the options array
    };

    while ((option = getopt_long_only(argc, argv, "", long_options, &option_index)) != -1) {
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

    // Create data sources & server
    constexpr int retrival_delay_ms = 500;
    constexpr int change_interval_ms = 1000;

    std::vector<std::unique_ptr<server::DataSource>> sources;
    for (int i = 0; i < number_data_sources; ++i) {
        sources.emplace_back(
            std::make_unique<server::DataSource>(change_interval_ms, retrival_delay_ms));
    }
    auto data_cache = std::make_unique<server::DataCache>(std::move(sources));
    server::DataServer data_server(std::move(data_cache), data_lifetime);

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

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
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
