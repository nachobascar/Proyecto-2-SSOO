#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "../../client/helpers/helpers.h"
#include "../server_structures/server_structures.h"

void create_player_board(player player);

void close_board(char** board);