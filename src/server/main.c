#include <stdio.h>

#include "socket/socket.h"

int main(int argc, char *argv[]) {
    printf("Hello, World! Im the server\n\n");

    if (argc < 5) {
        printf("Usage: %s  -i <ip_address> -p <tcp_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char ip_address[16];
    int tcp_port;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            strcpy(ip_address, argv[i + 1]);
        } else if (strcmp(argv[i], "-p") == 0) {
            tcp_port = atoi(argv[i + 1]);
        }
    }

    printf("IP address: %s\n", ip_address);
    printf("TCP port: %d\n\n", tcp_port);

    int socket_fd = create_socket(ip_address, tcp_port);
    printf("Socket created\n");

    server server = init_server(socket_fd);

    // Listen for connections
    accept_connections(socket_fd, server);

    close(socket_fd);
    printf("Socket closed\n");

    return 0;
}