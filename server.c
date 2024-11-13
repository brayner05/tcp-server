#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define BUFFER_SIZE (1024)
#define FAILURE_CODE (-1)
#define PRINT_ERROR(msg) perror((msg))
#define PRINT_ERROR_FATAL(code) \
    do { \
        PRINT_ERROR("A fatal error occured"); \
        exit((code));\
    } while(0)


int main(void) {
    puts("Starting server...");

    // Create a socket which can accept data
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == FAILURE_CODE) {
        PRINT_ERROR_FATAL(1);
    }

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_length = sizeof(client_addr);

    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    server_addr.sin_port = htons(8080); // Port 8080

    // Bind the socket to the server address
    int bind_result = bind(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (bind_result == FAILURE_CODE) {
        PRINT_ERROR_FATAL(1);
    }

    // Mark the socket to be used to accept incoming requests
    int listen_result = listen(socket_fd, 3);
    if (listen_result == FAILURE_CODE) {
        PRINT_ERROR_FATAL(1);
    }

    while (1) {
        puts("Waiting for connection...");

        // Block until a connection is made
        int client_fd = accept(socket_fd, (struct sockaddr*) &client_addr, &client_addr_length);
        if (client_fd == FAILURE_CODE) {
            PRINT_ERROR("Failed to accept connection");
        }

        // Block until data is sent from the client, and then read it
        char buffer[BUFFER_SIZE];
        int bytes_read = read(client_fd, buffer, sizeof(buffer));
        if (bytes_read > 0) {
            for (size_t i = 0; i < bytes_read; i++) {
                putchar(buffer[i]);
            }
        }

        // Close the connection to the client
        close(client_fd);
    }

    puts("Shutting down...");
    close(socket_fd);
}