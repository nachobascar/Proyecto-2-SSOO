#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct player {
  int socket;
  char* name;
  char** board;
  char status[50];
} player;

typedef struct room {
  player *players[2];
  int n_players;
  int status;
} room;

struct lobby_node {
  player* player;
  struct lobby_node* next;
  struct lobby_node* prev;
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

//############################################################################################################
/*
  Player functions
*/

// Initialize the player
player *init_player(int socket, char* name);

// Check if name is disponible
int is_name_disponible(char* name, server* server);

//############################################################################################################
/*
  Lobby functions
*/

// Add the player to the lobby
void add_player_to_lobby(player* player, lobby* lobby);

// Remove the player from the lobby
void remove_player_from_lobby(player* player, lobby* lobby);

// Pop the first player from the lobby
player* pop_player_from_lobby(lobby* lobby);

// Find player on the lobby
player* find_player_on_lobby_by_socket(int socket, lobby* lobby);

//############################################################################################################
/*
  Rooms functions
*/

// Find a disconnected player inside room with name
player* find_disconnected_player_on_room(char* name, int* room_id, int* player_id, server* server);

// Find a player inside room with socket
player* find_player_on_room_by_socket(int client_socket_fd, server* server);

