#pragma once

#include "../../client/helpers/helpers.h"

#include "../game_phase/game_phase.h"
#include "../preparation/preparation.h"
#include "../server_structures/server_structures.h"
#include "../socket/socket.h"

void handle_id_0(player *player, server *server, int id, int data_length, char *data);
void handle_id_1(player *player, server *server, int id, int data_length, char *data);
void handle_id_2(player *player, server *server, int id, int data_length, char *data);
void handle_id_3(player *player, server *server, int id, int data_length, char *data);
void handle_id_4(player *player, server *server, int id, int data_length, char *data);
void handle_id_5(player *player, server *server, int id, int data_length, char *data);
void handle_id_6(player *player, server *server, int id, int data_length, char *data);
void handle_id_7(player *player, server *server, int id, int data_length, char *data);
void handle_id_8(player *player, server *server, int id, int data_length, char *data);
void handle_id_9(player *player, server *server, int id, int data_length, char *data);
char** grid_to_send(char** grid);