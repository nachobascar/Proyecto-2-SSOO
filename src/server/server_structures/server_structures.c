#include "server_structures.h"

// Initialize the room
void init_room(room* room) {
  room->status = 0;
  room->n_players = 0;
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