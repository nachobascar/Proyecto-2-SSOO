#include "socket.h"

// Create the socket and open the connection of the server
int create_socket(char *ip_address, int port) {
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}

	// Set the socket options
	int opt = 1;
	int ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	if (ret < 0) {
		perror("Error setting socket options");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = inet_addr(ip_address);

	int bind_status = bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
	if (bind_status < 0) {
		perror("Error binding socket");
		exit(EXIT_FAILURE);
	}

	return socket_fd;
}

// Handle the message sent from client
void handle_package(player *player, char buffer[257], server *server) {
	int id = buffer[0];
	int data_length = (int)buffer[1];
	char data[256];
	memcpy(data, &buffer[2], data_length);
	data[data_length] = '\0';

	printf("Received package from %s with id %d and data %s\n", player->name, id, data);

	switch (id) {
	case 0:;
		// Init player. The package is the player name
		handle_id_0(player, server, id, data_length, data);
		break;

	case 1:;
		// Enter user in the room
		handle_id_1(player, server, id, data_length, data);
		break;
	case 2:;
		// Request for updated rooms list
		handle_id_2(player, server, id, data_length, data);
		break;
	case 3:;
		// Confirmation of user to play
		handle_id_3(player, server, id, data_length, data);
		break;
	case 4:;
		// Request for user to not play
		handle_id_4(player, server, id, data_length, data);
		break;
	case 5:;
		// Receive coordinates of the shot
		handle_id_5(player, server, id, data_length, data);
		break;
	case 6:;
		// Receive coordinates of the shot
		handle_id_6(player, server, id, data_length, data);
		break;
	case 7:;
		// Receive coordinates of the shot
		handle_id_7(player, server, id, data_length, data);
		break;
	case 8:;
		// Receive coordinates of the shot
		handle_id_8(player, server, id, data_length, data);
		break;
	case 9:;
		// Receive coordinates of the shot
		handle_id_9(player, server, id, data_length, data);
		break;

	default:
		break;
	}
}

// Data structure to pass multiple arguments to the thread
struct connection_init_args {
	int client_socket_fd;
	server *server;
};

// Handle the logic of a client disconnection
void handle_client_disconnection(int client_socket_fd, server *server) {
	// Find the player on the lobby
	player *player = find_player_on_lobby_by_socket(client_socket_fd, &(server->lobby));
	if (player != NULL) {
		// If the player is on the lobby, remove it
		remove_player_from_lobby(player, &server->lobby);
		printf("Player %s left the lobby\n", player->name);
		free(player);
	} else {
		// If the player is on active room, remove it
		player = find_player_on_room_by_socket(client_socket_fd, server);
		if (player == NULL) {
			return;
		}

		if (player->disconnected) {
			return;
		}

		room* room = &server->rooms[player->room_id];

		if (strcmp(room->status, "waiting") == 0) {
			// If the room is waiting, remove the player
			room->players[0] = room->players[!player->player_id];
			if (room->players[0] != NULL) {
				room->players[0]->player_id = 0;
			}
			room->players[1] = NULL;
			room->n_players--;

			printf("Player %s left the room %d\n", player->name, player->room_id);
			close(player->socket);
			free(player);
			return;
		} 
		
		if (strcmp(room->status, "pending confirmation") == 0) {
			// If the room is pending confirmation, remove player and tell the other player
			strcpy(room->status, "waiting");

			room->players[0] = room->players[!player->player_id];
			room->players[0]->player_id = 0;

			room->players[1] = NULL;
			room->n_players--;

			printf("Player %s left the room %d\n", player->name, player->room_id);

			char message[1];
			message[0] = 1;
			send_package(room->players[0]->socket, 10, 1, message, server);
			strcpy(room->players[0]->status, "waiting");
			close(player->socket);
			free(player);
			return;
		} 
		
		// If the room is playing, set player as disconnected
		player->disconnected = 1;
		printf("Player %s got disconnected\n", player->name);

		if (!(strcmp(room->status, "preparation") == 0 && !strcmp(player->status, "ready") == 0)) {
			send_package(room->players[!player->player_id]->socket, 11, 0, NULL, server);
		}
		close(player->socket);
	}
}

// Handle the connection with the client
void *handle_client(void *args) {
	signal(SIGPIPE, SIG_IGN);
	int client_socket_fd = ((struct connection_init_args *)args)->client_socket_fd;
	server *server = ((struct connection_init_args *)args)->server;

	player *player = init_player(client_socket_fd, "");

	char buffer[257];
	while (1) {
		bzero(buffer, 257);
		int read_status = read(client_socket_fd, buffer, 2);
		if (read_status <= 0) {
			perror("Error reading from socket");
			handle_client_disconnection(client_socket_fd, server);
			// Close the socket and end the thread
			close(client_socket_fd);
			pthread_exit(NULL);
		}
		read_status = read(client_socket_fd, buffer + 2, buffer[1]);
		if (read_status < 0) {
			perror("Error reading package from socket");
			handle_client_disconnection(client_socket_fd, server);
			close(client_socket_fd);
			pthread_exit(NULL);
		}

		handle_package(player, buffer, server);
	}
}

// Start listening for connections
int accept_connections(int socket_fd, server *server) {
	while (1) {
		struct sockaddr_in client_address;
		socklen_t client_address_length = sizeof(client_address);

		listen(socket_fd, 5);
		int client_socket_fd = accept(socket_fd, (struct sockaddr *)&client_address, &client_address_length);
		if (client_socket_fd < 0) {
			perror("Error accepting connection");
			exit(EXIT_FAILURE);
		}

		struct connection_init_args args;
		args.client_socket_fd = client_socket_fd;
		args.server = server;

		// Start thread for client
		server->threads = realloc(server->threads, (server->threads_size + 1) * sizeof(pthread_t));
		pthread_create(&server->threads[server->threads_size++], NULL, handle_client, (void *)&args);
	}
}

// Send a package to the client
void send_package(int client_socket_fd, int id, int data_length, char *data, server *server) {
	printf("Sending package to client %d, id: %d\n", client_socket_fd, id);
	char buffer[257];
	buffer[0] = id;
	buffer[1] = data_length;
	for (int i = 0; i < data_length; i++) {
		buffer[i + 2] = data[i];
	}

	int write_status = write(client_socket_fd, buffer, data_length + 2);
	if (write_status < 0) {
		perror("Error writing to socket");
		handle_client_disconnection(client_socket_fd, server);
		close(client_socket_fd);
		// pthread_exit(NULL);
	}
}