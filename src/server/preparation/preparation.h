#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "../../client/helpers/helpers.h"
#include "../server_structures/server_structures.h"

char** create_board();

void close_board(char** board);