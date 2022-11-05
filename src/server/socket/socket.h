#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "../server_structures/server_structures.h"

// Create the socket and open the connection of the server
int create_socket(char *ip_address, int port);

// Start listening for connections
int accept_connections(int socket_fd, server server);

// Send a package to the client
void send_package(int client_socket_fd, int id, int data_length, char *data);
