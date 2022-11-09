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
	printf("Client disconnected\n");
	player *player = find_player_on_lobby_by_socket(client_socket_fd, &(server->lobby));
	if (player != NULL) {
		// If the player is on the lobby, remove it
		remove_player_from_lobby(player, &server->lobby);
		printf("Player %s left the lobby\n", player->name);
		free(player);
	}
	else {
		// If the player is on active room, remove it
		player = find_player_on_room_by_socket(client_socket_fd, server);
		if (player != NULL) {
			strcpy(player->status, "disconnected");
			printf("Player %s got disconnected\n", player->name);
		}
	}
}

// Handle the connection with the client
void *handle_client(void *args) {
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
		pthread_t thread;
		pthread_create(&thread, NULL, handle_client, (void *)&args);
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
		pthread_exit(NULL);
	}
}