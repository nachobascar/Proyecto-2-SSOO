#include <stdio.h>

#include "socket/socket.h"
#include "preparation/preparation.h"
#include "server_structures/server_structures.h"

int main(int argc, char *argv[]) {
    printf("Hello, World! Im the server\n\n");

    // Check for arguments
    if (argc != 5) {
        printf("Usage: %s  -i <ip_address> -p <tcp_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char ip_address[16];
    int tcp_port;
    for (int i = 1; i < argc; i+=2) {
        if (strcmp(argv[i], "-i") == 0) {
            strcpy(ip_address, argv[i + 1]);
        } else if (strcmp(argv[i], "-p") == 0) {
            tcp_port = atoi(argv[i + 1]);
        } else {
            printf("Usage: %s  -i <ip_address> -p <tcp_port>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    printf("IP address: %s\n", ip_address);
    printf("TCP port: %d\n\n", tcp_port);

    // Craete player
    player player;
    player.board = create_board();

    // Menu for placing ships
    start_preparation(player.board);

    // Free board memory
    close_board(player.board);

    // // Initialize the server
    // int socket_fd = create_socket(ip_address, tcp_port);
    // server server = init_server(socket_fd);
    // printf("Socket created\n");


    // // Listen for connections
    // accept_connections(socket_fd, server);

    // // Close the socket
    // close(socket_fd);
    // printf("Socket closed\n");

    return 0;
}