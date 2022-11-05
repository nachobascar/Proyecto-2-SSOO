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