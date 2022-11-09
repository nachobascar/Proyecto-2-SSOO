#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../client/helpers/helpers.h"
#include "../server_structures/server_structures.h"

// Create an empty board
char** create_board();

// Menu for placing ships
void start_preparation(char** board);

// Free board memory
void close_board(char** board);

// Return the number of placed ships in a board
int count_placed_ships(char** board);

// Place a ship in the board given start and end position
// Return 0 if placed correctly, if not return -1
int place_ship(char** board, char* start, char* end);

// Empty board positions
void restart_board(char** board);