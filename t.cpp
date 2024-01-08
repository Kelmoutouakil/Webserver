// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>

// #define MAX_CLIENTS 5

// int main() {
//     int serverSocket, clientSockets[MAX_CLIENTS];
//     fd_set readfds;
//     struct sockaddr_in serverAddr, clientAddr;
//     socklen_t clientAddrLen = sizeof(clientAddr);

//     // Create and set up the server socket
//     serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == -1) {
//         perror("Socket creation failed");
//         exit(EXIT_FAILURE);
//     }

//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_addr.s_addr = INADDR_ANY;
//     serverAddr.sin_port = htons(8888);

//     if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
//         perror("Bind failed");
//         exit(EXIT_FAILURE);
//     }

//     if (listen(serverSocket, 3) < 0) {
//         perror("Listen failed");
//         exit(EXIT_FAILURE);
//     }

//     printf("Server listening on port 8888...\n");

//     // Initialize the client sockets array
//     for (int i = 0; i < MAX_CLIENTS; ++i) {
//         clientSockets[i] = 0;
//     }

//     while (1) {
//         FD_ZERO(&readfds);
//         FD_SET(serverSocket, &readfds);
//         int maxSocket = serverSocket;
//         // Add all client sockets to the set
//         for (int i = 0; i < MAX_CLIENTS; ++i) {
//             int clientSocket = clientSockets[i];
//             if (clientSocket > 0) {
//                 FD_SET(clientSocket, &readfds);
//             }
//             if (clientSocket > maxSocket) {
//                 maxSocket = clientSocket;
//             }
//         }
//         // Use select to wait for activity on any of the sockets
//         int activity = select(maxSocket + 1, &readfds, NULL, NULL, NULL);
//         if (activity < 0) {
//             perror("Select error");
//             exit(EXIT_FAILURE);
//         }
//         // Check for incoming connection on the server socket
//         if (FD_ISSET(serverSocket, &readfds)) {
//             int newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
//             if (newSocket < 0) {
//                 perror("Accept error");
//                 exit(EXIT_FAILURE);
//             }
//             printf("New connection, socket fd is %d\n", newSocket);

//             // Add the new socket to the array of client sockets
//             for (int i = 0; i < MAX_CLIENTS; ++i) {
//                 if (clientSockets[i] == 0) {
//                     clientSockets[i] = newSocket;
//                     break;
//                 }
//             }
//         }
//         // Check for data on client sockets
//         for (int i = 0; i < MAX_CLIENTS; ++i) {
//             int clientSocket = clientSockets[i];

//             if (FD_ISSET(clientSocket, &readfds)) {
//                 char buffer[1024];
//                 ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

//                 if (bytesRead <= 0) {
//                     // Connection closed or error
//                     printf("Client disconnected, socket fd is %d\n", clientSocket);
//                     close(clientSocket);
//                     clientSockets[i] = 0;
//                 } else {
//                     // Process data received from the client
//                     buffer[bytesRead] = '\0';
//                     printf("Received data from client %d: %s\n", clientSocket, buffer);
//                 }
//             }
//         }
//     }

//     return 0;
// }
