#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../client/helpers/helpers.h"
#include "../server_structures/server_structures.h"

char** create_board();

void start_preparation(char** board);

void close_board(char** board);