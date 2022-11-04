#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct player {
  int socket;
  char* name;
  int status;
} player;

typedef struct room {
  player players[2];
  int n_players;
  int status;
} room;

struct lobby_node {
  player* player;
  struct lobby_node* next;
};
typedef struct lobby {
  struct lobby_node* first;
  struct lobby_node* last;
} lobby;

typedef struct server {
  int socket;

  lobby lobby;
  int lobby_size;

  room rooms[10];
  int rooms_size;
} server;

// Create and initialize the server
server init_server(int socket);

// Initialize the player
void init_player(player* player, int socket, char* name);

// Add the player to the lobby
void add_player_to_lobby(player* player, lobby* lobby);
