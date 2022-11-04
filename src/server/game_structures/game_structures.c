#include "game_structures.h"

void init_room(room* room) {
  room->status = 0;
  room->n_players = 0;
}

game init_game(int socket) {
  game game;
  game.socket = socket;
  
  game.lobby.first = NULL;
  game.lobby.last = NULL;

  game.lobby_size = 0;
  for (int i = 0; i < 10; i++) {
    init_room(&game.rooms[i]);
  }
  game.rooms_size = 10;
  return game;
}