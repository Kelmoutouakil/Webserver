#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_request(int client_socket) {
    // Read the HTTP request from the client
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    read(client_socket, buffer, sizeof(buffer) - 1);

    // Print the received request
    std::cout << "Received request:\n" << buffer << std::endl;

    // Send a simple response back to the client
    const char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 1\n\nHello, ajari!";
    write(client_socket, response, strlen(response));

    // Close the client socket
    close(client_socket);
}

int main() {
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }
    struct sockaddr t;

    // Bind the socket to an address and port
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error binding to address" << std::endl;
        close(server_socket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(server_socket);
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        // Accept a connection from a client
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        std::cout << "......>\n";
        if (client_socket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }
        // Handle the client request
        handle_request(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}
