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

int create_socket(char *ip_address, int port);
int accept_connections(int socket_fd, server server);
