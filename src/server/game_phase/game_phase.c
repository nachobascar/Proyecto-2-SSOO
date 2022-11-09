#include "game_phase.h"

/*
  check if the given coordinates are valid ( non used && in the board )
  input:
    coordinates: A1
    board: grid of chars
*/
bool check_coordinates(char* coordinates, char** board) {
	if (strlen(coordinates) != 2) {
		return false;
	}

	if (coordinates[0] < 'A' || coordinates[0] > 'E') {
		return false;
	}

	if (coordinates[1] < '1' || coordinates[1] > '5') {
		return false;
	}

	int column = coordinates[0] - 'A';
	int row = coordinates[1] - '1';

	if (board[row][column] == 'X' || board[row][column] == '-') {
		return false;
	}

	return true;
}

/*
  manage the end of the game
*/
void game_over(room* room, server* server, int winner_index) {
	send_boards(room, room->players[0]->board, room->players[1]->board, server);

	char message[100];
	strcpy(message, "Ha ganado el jugador ");
	strcat(message, room->players[winner_index]->name);
	strcat(message, "!\n");
	strcat(message, "Qué quieres hacer ahora??\n");
	strcat(message, "\t1. Volver al lobby\n");
	strcat(message, "\t2. Salir del juego\n");

	send_package(room->players[0]->socket, GAME_FINISHED_ID, strlen(message), message, server);
	send_package(room->players[1]->socket, GAME_FINISHED_ID, strlen(message), message, server);
	return;
}

/*
  send to the client the signal to start his turn
*/
void is_your_turn(player* player, server* server) {
	char message[100];

	strcpy(message, "Es el turno del jugador ");
	strcat(message, player->name);
	strcat(message, "\n");
	strcat(message, "Elige las coordenadas donde quieres disparar (ej: A1)\n");

	send_package(player->socket, PENDING_INFO_ID, strlen(message), message, server);
}

/*
  send to the client the signal to wait for his turn
*/
void opponent_turn(char* opponent_name, int socket, server* server) {
	char message[100];

	strcpy(message, "Es el turno del jugador ");
	strcat(message, opponent_name);
	strcat(message, "\n");
	strcat(message, "Esperando a que el otro jugador elija coordenadas...\n");

	send_package(socket, SEND_TEXT_ID, strlen(message), message, server);
}

/*
  update the board of the player that was shot
  input:
    board: grid of chars of the player that was shot
    shotted_player_id: id of the player that was shot
    coordinates: A1
    room: room where the game is being played
    server: server struct
*/
void update_board(char** board, char* coordinates, room* room, int shotted_player_id, server* server) {
	char message[100];

	player* players[2];
	players[0] = room->players[0];
	players[1] = room->players[1];

	int column = coordinates[0] - 'A';
	int row = coordinates[1] - '1';

	char value = board[row][column];

	if (value == ' ') {
		board[row][column] = '-';
		send_package(players[0]->socket, SEND_TEXT_ID, 6, "Agua\n", server);
		send_package(players[1]->socket, SEND_TEXT_ID, 6, "Agua\n", server);
		return;
	}

	players[shotted_player_id]->board[row][column] = 'X';

	// check if in the board there is another boat with the same value
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (players[shotted_player_id]->board[row][column] == value) {
				strcpy(message, "Han impactado uno de tus barcos\n");
				send_package(players[shotted_player_id]->socket, SEND_TEXT_ID, strlen(message), message, server);

				strcpy(message, "Has impactado un barco\n");
				send_package(players[!shotted_player_id]->socket, SEND_TEXT_ID, strlen(message), message, server);
				return;
			}
		}
	}

	// if there is not another boat with the same value, the boat is sunk
	strcpy(message, "Has hundido un barco\n");
	send_package(players[shotted_player_id]->socket, SEND_TEXT_ID, strlen(message), message, server);

	strcpy(message, "Han hundido uno de tus barcos\n");
	send_package(players[!shotted_player_id]->socket, SEND_TEXT_ID, strlen(message), message, server);
}

/*
    check if there are 2, 3 or 4's in the board
*/
bool check_if_game_is_over(char** board) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (board[i][j] == '2' || board[i][j] == '3' || board[i][j] == '4') {
				return false;
			}
		}
	}

	return true;
}

/*
    send to both players their own board and the board of the opponent hidden
    Format: "OO  X- O O- XX  O     X |OO  X-    - XX   --   X"
     El separador "|" es ficticio. El cliente tomará los primeros 25 caracteres como el tablero del
     jugador y los siguientes 25 como el tablero del oponente
*/
void send_boards(room* room, char** board_0, char** board_1, server* server) {
	player* players[2];
	players[0] = room->players[0];
	players[1] = room->players[1];

	char board_0_hide[25];
	char board_1_hide[25];

	// build opponent boards
	opponent_board(board_0, board_0_hide);
	opponent_board(board_1, board_1_hide);

	char board_0_string[25];
	char board_1_string[25];

	// build player boards
	board_to_string(board_0, board_0_string);
	board_to_string(board_1, board_1_string);

	char package_0[100];
	char package_1[100];

	strcpy(package_0, board_0_string);
	strcat(package_0, board_1_hide);

	strcpy(package_1, board_1_string);
	strcat(package_1, board_0_hide);

	send_package(players[0]->socket, SEND_BOARD_ID, strlen(package_0), package_0, server);
	send_package(players[1]->socket, SEND_BOARD_ID, strlen(package_1), package_1, server);
}

/*
    build the string of the opponent board hidden
*/
void opponent_board(char** board, char* output_board) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (board[i][j] == '2' || board[i][j] == '3' || board[i][j] == '4') {
				output_board[i * 5 + j] = ' ';
			}
			else {
				output_board[i * 5 + j] = board[i][j];
			}
		}
	}
}

/*
    build the string of the player board
*/
void board_to_string(char** board, char* output_board) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			output_board[i * 5 + j] = board[i][j];
		}
	}
}
