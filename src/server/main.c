#include <stdio.h>

#include "socket/socket.h"
#include "preparation/preparation.h"
#include "server_structures/server_structures.h"

server server_;
int socket_fd;

// Signal handler for SIGINT
void signal_handler(int signal) {
    printf("Signal %d received, exiting...\n", signal);
    close(socket_fd);

    for (int i = 0; i < server_.rooms_size; i++) {
        room* room = &server_.rooms[i];
        for (int j = 0; j < 2; j++) {
            if (room->players[j]) {
                player* player = room->players[j];
                close(player->socket);
                close_board(player->board);
                free(player);
            }
        }
    }
    
    struct lobby_node* node = server_.lobby.first;
    while (node != NULL) {
        struct lobby_node* next = node->next;
        close(node->player->socket);
        free(node->player);
        free(node);
        node = next;
    }

    // terminate all threads
    for (int i = 0; i < server_.threads_size; i++) {
        pthread_cancel(server_.threads[i]);
        // Wait for cancel to conclude
        pthread_join(server_.threads[i], NULL);
    }

    free(server_.threads);

    printf("Socket closed\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler);
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

    // // Craete player
    // player player;
    // player.board = create_board();

    // // Menu for placing ships
    // start_preparation(player.board);

    // // Free board memory
    // close_board(player.board);

    // Initialize the server
    socket_fd = create_socket(ip_address, tcp_port);
    server_ = init_server(socket_fd);
    printf("Socket created\n");

    // Listen for connections
    accept_connections(socket_fd, &server_);

    // Close the socket
    close(socket_fd);
    free(server_.threads);
    printf("Socket closed\n");

    return 0;
}