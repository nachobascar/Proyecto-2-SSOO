#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../server_structures/server_structures.h"
#include "../socket/socket.h"

// send text without expecting a response
const int SEND_TEXT_ID = 5;

// send text and expect a response
const int PENDING_INFO_ID = 6;

// Send the boards to the players
const int SEND_BOARD_ID = 7;

// game finished
const int GAME_FINISHED_ID = 8;

bool check_coordinates(char* coordinates, char** board);

void game_over(room* room, server* server, int winner_index);

void is_your_turn(player* player, server* server);

void opponent_turn(char* opponent_name, int socket, server* server);

void update_board(char** board, char* coordinates, room* room, int shotted_player_id, server* server);

bool check_if_game_is_over(char** board);

void send_boards(room* room, char** board_0, char** board_1, server* server);

void opponent_board(char** board, char* output_board);

void board_to_string(char** board, char* output_board);