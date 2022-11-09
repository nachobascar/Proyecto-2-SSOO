
#include "message_handler.h"

// // Init player. The package is the player name
// void handle_id_0(player* player, server* server, int id, int data_length, char data[256]) {
// 	char player_name[256];
// 	strcpy(player_name, data);

// 	// Find if player is on active room
// 	int room_id, player_id;
// 	struct player* old_player = find_disconnected_player_on_room(player_name, &room_id, &player_id, server);
// 	if (old_player == NULL) {
// 		// Check if name is already taken
// 		int disponible = is_name_disponible(player_name, server);
// 		if (!disponible) {
// 			// Send error message
// 			char error_message[256];
// 			error_message[0] = 0;
// 			strcpy(error_message + 1, "Name already taken");
// 			send_package(player->socket, 0, strlen(error_message), NULL, server);
// 			return;
// 		}

// 		// If not, create a new player
// 		strncpy(player->name, player_name, 255);
// 		// Add the player to the lobby
// 		add_player_to_lobby(player, &server->lobby);
// 		printf("Player %s joined the lobby\n", player_name);

// 		// Send to the player the list of rooms
// 		char data[255];
// 		int data_length = 1;
// 		data[0] = 1;
// 		for (int i = 0; i < server->rooms_size; i++) {
// 			int room_id = i;
// 			int n_players = server->rooms[i].n_players;
// 			data[data_length++] = room_id;
// 			data[data_length++] = n_players;
// 		}
// 		send_package(player->socket, 0, data_length, data, server);
// 	}
// 	else {
// 		// If the player is on active room, update the socket fd
// 		strcpy(player->name, player_name);
// 		strcpy(player->status, "playing");
// 		player->room_id = room_id;
// 		player->player_id = player_id;
// 		// Update player on room
// 		server->rooms[room_id].players[player_id] = player;
// 		printf("Player %s rejoined the game\n", player_name);
// 		free(old_player);
// 	}
// }

// Enter user in the room
void handle_id_1(player* player, server* server, int id, int data_length, char* data) {
	int room_id = data[0];
	if (room_id >= server->rooms_size) {
		printf("Invalid room id: %d\n", room_id);
		return;
	}

	room* room = &server->rooms[room_id];
	printf("%d\n", room->n_players);
	if (room->n_players == 2) {
		// If the room is full, send error
		send_package(player->socket, 1, 0, NULL, server);
	}
	else {
		// If the room is not full, add the player to the room
		player->room_id = room_id;

		int player_id = 0;
		for (int i = 0; i < 2; i++) {
			if (room->players[i] == NULL) {
				player_id = i;
				break;
			}
		}
		player->player_id = player_id;
		room->players[player_id] = player;
		room->n_players++;
		strcpy(player->status, "waiting");

		// Remove the player from the lobby
		remove_player_from_lobby(player, &server->lobby);
		printf("Player %s joined the room %d\n", player->name, room_id);
		// Send to the player the room id
		send_package(player->socket, 1, 1, data, server);

		if (room->n_players == 2) {
			// If the room is full, start the game
			for (int i = 0; i < 2; i++) {
				strcpy(room->players[i]->status, "pending confirmation");
				// Tell the players that the game is starting
				send_package(room->players[i]->socket, 9, 0, NULL, server);
			}
		}
	}
}

// Request for updated rooms list
void handle_id_2(player* player, server* server, int id, int aux_data_length, char* aux_data) {
	// Send to the player the list of rooms
	char data[255];
	int data_length = 1;
	data[0] = 1;
	for (int i = 0; i < server->rooms_size; i++) {
		int room_id = i;
		int n_players = server->rooms[i].n_players;
		data[data_length++] = room_id;
		data[data_length++] = n_players;
	}
	send_package(player->socket, 0, data_length, data, server);
}

// Recieve confirmation of player to play
void handle_id_3(player* player, server* server, int id, int aux_data_length, char* aux_data) {
	int room_id = player->room_id;
	room* room = &server->rooms[room_id];
	strcpy(player->status, "confirmed");

	if (room->n_players < 2) {
		// If the other player left, send error
		strcpy(player->status, "waiting");
		char message[1];
		message[0] = 1;
		send_package(player->socket, 10, 1, message, server);
	}
	else if (room->players[!player->player_id] != NULL && strcmp(room->players[!player->player_id]->status, "confirmed") == 0) {
		// If the other player is confirmed, start the game
    char *message = "Seleccione la posición del barco de tamaño 2\n";
    char buffer[255];
    for (int i = 0; i < 25; i++) {
      buffer[i] = ' ';
    }
    buffer[25] = 1;
    strcpy(buffer + 26, message);
    for (int i = 0; i < 2; i++) {
      send_package(room->players[i]->socket, 2, 27 + strlen(message), buffer, server);
      strcpy(room->players[i]->status, "boat 2");
    }
	}
}

// Tell that a player went out of the room
void handle_id_4(player* player, server* server, int id, int aux_data_length, char* aux_data) {
	int room_id = player->room_id;
	int player_id = player->player_id;
	room* room = &server->rooms[room_id];
	// Remove the player from the room
	room->players[player_id] = NULL;
	room->n_players--;
	// Add the player to the lobby
	add_player_to_lobby(player, &server->lobby);
	strcpy(player->status, "lobby");
	printf("Player %s left the room %d\n", player->name, room_id);

	// Confirm abort to player
	char message[1];
	message[0] = 0;
	send_package(player->socket, 10, 1, message, server);

	if (room->players[!player_id] != NULL && strcmp(room->players[!player_id]->status, "confirmed") == 0) {
		// If the other player is still in the room, tell him that the other player left
		strcpy(room->players[!player_id]->status, "waiting");
		char message[1];
		message[0] = 1;
		send_package(room->players[!player_id]->socket, 10, 1, message, server);
	}
}

// Game phase handler. Player shoots opponent
void handle_id_5(player* player, server* server, int id, int data_length, char* data) {
	char coordinates[2];
	coordinates[0] = data[0];
	coordinates[1] = data[1];

	char message[100];

	int room_id = player->room_id;
	room* room = &server->rooms[room_id];

	struct player* opponent = room->players[!player->player_id];

	// get boards
	char** player_board = player->board;
	char** opponent_board = opponent->board;

	// Check the input
	if (!check_coordinates(coordinates, opponent_board)) {
		strcpy(message, "Coordenadas invalidas, intentalo nuevamente\n");
		strcat(message, "Formato: <letra mayúscula><numero> (ej: B5)\n");
		send_package(player->socket, PENDING_INFO_ID, strlen(message), message, server);
		return;
	}

	// Manage the valid shot done by the player
	strcpy(message, "Disparando a las coordenadas ");
	strcat(message, coordinates);
	strcat(message, "\n");
	send_package(player->socket, SEND_TEXT_ID, strlen(message), message, server);
	send_package(opponent->socket, SEND_TEXT_ID, strlen(message), message, server);

	update_board(opponent_board, coordinates, room, player->player_id, server);

	// If the game is over, send the signal to the players and the info
	if (check_if_game_is_over(opponent_board)) {
		game_over(room, server, player->player_id);
		return;
	}

	// If the game is not over, send the boards to the players and continue the game
	send_boards(room, player_board, opponent_board, server);

	// "Change turn" message
	is_your_turn(opponent, server);
	opponent_turn(opponent->name, player->socket, server);
}


void handle_id_0(player* player, server* server, int id, int data_length, char* data) {
	player->board = create_board();
}

void handle_id_6(player* player, server* server, int id, int data_length, char* data) {
	print_grid(player->board);
	char start[] = {data[0], data[1]};
	char end[] = {data[2], data[3]};
	char end_2[] = {data[4], data[5]};
	char char_1 = start[0];
	char char_2 = start[1];
	printf("Start: %c\n", char_1);
	printf("Start: %c\n", char_2);
	printf("Start: %s\n", start);
	printf("End: %s\n", end);
	printf("End: %s\n", end_2);

	// if (count_placed_ships(player->board) != 3) {
	// 	int status = place_ship(player->board, start, end);
	// 	if (status == 0) {
	// 		print_grid(player->board);
	// 	} else {
	// 		char* error_msg = "Las coordenadas ingresadas son inválidas. Recuerda lo siguiente\n"
	// 		"\t- Seguir el formato para ingresar coordenadas\n"
	// 		"\t- Los barcos solo pueden estar horizontal o verticalmente\n"
	// 		"\t- Primero debes ingresar un barco de largo 2, luego 3 y luego 4\n"
	// 		"\t- Un barco no puede chocar con otro\n\n";
	// 	  printf("%s", error_msg);
	// 	}
  // }
}