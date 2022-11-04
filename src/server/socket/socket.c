#include "socket.h"

int create_socket(char *ip_address, int port) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("Error creating socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = inet_addr(ip_address);

  int bind_status = bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address));
  if (bind_status < 0) {
    perror("Error binding socket");
    exit(EXIT_FAILURE);
  }

  return socket_fd;
}

void handle_package(int client_socket_fd, char buffer[257], game game) {
  switch (buffer[0])
  {
  case 0: ;
    // Init player. The package is the player name
    char player_name[256];
    int i;
    for (i = 0; i < 255; i++) {
      player_name[i] = buffer[i + 2];
    }
    player_name[i] = '\0';

    // Create player and add to the lobby
    // init_player(client_socket_fd, player_name, game);
    printf("Player %s joined the lobby\n", player_name);

    break;
  
  default:
    break;
  }
}

struct connection_init_args {
  int client_socket_fd;
  game game;
};

void *handle_client(void *args) {
  int client_socket_fd = ((struct connection_init_args*) args)->client_socket_fd;
  game game = ((struct connection_init_args*) args)->game;

  char buffer[257];
  while (1) {
    bzero(buffer, 257);
    int read_status = read(client_socket_fd, buffer, 2);
    if (read_status < 0) {
      perror("Error reading from socket");
      exit(EXIT_FAILURE);
    }

    read_status = read(client_socket_fd, &(buffer[2]), buffer[1]);
    if (read_status < 0) {
      perror("Error reading package from socket");
      exit(EXIT_FAILURE);
    }

    handle_package(client_socket_fd, buffer, game);
  }
}

int accept_connections(int socket_fd, game game) {
  while (1) {
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);

    listen(socket_fd, 5);
    int client_socket_fd = accept(socket_fd, (struct sockaddr *) &client_address, &client_address_length);
    if (client_socket_fd < 0) {
      perror("Error accepting connection");
      exit(EXIT_FAILURE);
    }

    struct connection_init_args args;
    args.client_socket_fd = client_socket_fd;
    args.game = game;

    // Start thread for client
    pthread_t thread;
    pthread_create(&thread, NULL, handle_client, (void *) &args);
  }
}