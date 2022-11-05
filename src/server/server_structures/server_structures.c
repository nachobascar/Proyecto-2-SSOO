#include "server_structures.h"

// Initialize the room
void init_room(room* room) {
  room->status = 0;
  room->n_players = 0;
  for (int i = 0; i < 2; i++) {
    room->players[i] = NULL;
  }
}

// Create and initialize the server
server init_server(int socket) {
  server server;
  server.socket = socket;
  
  server.lobby.first = NULL;
  server.lobby.last = NULL;

  server.lobby_size = 0;
  for (int i = 0; i < 10; i++) {
    init_room(&server.rooms[i]);
  }
  server.rooms_size = 10;
  return server;
}

// Initialize the player
player *init_player(int socket, char* name) {
  player* player = malloc(sizeof(player));
  player->socket = socket;
  player->name = name;
  strcpy(player->status, "lobby");
  return player;
}

// Add the player to the lobby
void add_player_to_lobby(player* player, lobby* lobby) {
  struct lobby_node* node = malloc(sizeof(struct lobby_node));
  node->player = player;
  node->next = NULL;
  node->prev = lobby->last;

  if (lobby->first == NULL) {
    lobby->first = node;
    lobby->last = node;
  } else {
    lobby->last->next = node;
    node->prev = lobby->last;
    lobby->last = node;
  }
}

// Remove the player from the lobby
void remove_player_from_lobby(player* player, lobby* lobby) {
  struct lobby_node* node = lobby->first;
  while (node != NULL) {
    if (node->player == player) {
      if (node->prev != NULL) {
        node->prev->next = node->next;
      } else {
        lobby->first = node->next;
      }
      if (node->next != NULL) {
        node->next->prev = node->prev;
      } else {
        lobby->last = node->prev;
      }
      free(node);
      return;
    }
    node = node->next;
  }
}

// Pop the first player from the lobby
player* pop_player_from_lobby(lobby* lobby) {
  if (lobby->first == NULL) {
    return NULL;
  }
  struct lobby_node* node = lobby->first;
  lobby->first = node->next;
  if (lobby->first == NULL) {
    lobby->last = NULL;
  } else {
    lobby->first->prev = NULL;
  }
  player* player = node->player;
  free(node);
  return player;
}

// Find player inside lobby
player* find_player_on_lobby_by_socket(int socket, lobby* lobby) {
  struct lobby_node* node = lobby->first;
  while (node != NULL) {
    if (node->player->socket == socket) {
      return node->player;
    }
    node = node->next;
  }
  return NULL;
}

// Find a disconnected player inside room with name
player* find_disconnected_player_on_room(char* name, int* room_id, int* player_id, server* server) {
  for (int i = 0; i < server->rooms_size; i++) {
    room* room = &server->rooms[i];
    if (room->n_players == 2) {
      if (strcmp(room->players[0]->name, name) == 0 && strcmp(room->players[0]->status, "disconnected") == 0) {
        *room_id = i;
        *player_id = 0;
        return room->players[0];
      }
      if (strcmp(room->players[1]->name, name) == 0 && strcmp(room->players[0]->status, "disconnected") == 0) {
        *room_id = i;
        *player_id = 1;
        return room->players[1];
      }
    }
  }
  return NULL;
}

// Find player inside room with socket
player* find_player_on_room_by_socket(int client_socket_fd, server* server) {
  for (int i = 0; i < server->rooms_size; i++) {
    room* room = &server->rooms[i];
    if (room->n_players == 2) {
      if (room->players[0]->socket == client_socket_fd) { 
        return room->players[0];
      }
      if (room->players[1]->socket == client_socket_fd) {
        return room->players[1];
      }
    }
  }
  return NULL;
}

// Check if name is disponible
int is_name_disponible(char* name, server* server) {
  for (int i = 0; i < server->rooms_size; i++) {
    room* room = &server->rooms[i];
    for (int j = 0; j < room->n_players; j++) {
      if (room->players[j] && strcmp(room->players[j]->name, name) == 0) {
        return 0;
      }
    }
  }
  struct lobby_node* node = server->lobby.first;
  while (node != NULL) {
    if (strcmp(node->player->name, name) == 0) {
      return 0;
    }
    node = node->next;
  }
  return 1;
}