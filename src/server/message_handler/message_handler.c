
#include "message_handler.h"

// Init player. The package is the player name
void handle_id_0(player* player, server *server, int id, int data_length, char *data) {
  char player_name[256];
  strcpy(player_name, data);

  // Find if player is on active room
  int room_id, player_id;
  struct player* old_player = find_disconnected_player_on_room(player_name, &room_id, &player_id, server);
  if (old_player == NULL) {
    // If not, create a new player
    strcpy(player->name, player_name);
    // Add the player to the lobby
    add_player_to_lobby(player, &server->lobby);
    printf("Player %s joined the lobby\n", player_name);

    // Send to the player the list of rooms
    char data[255];
    int data_length = 0;
    for (int i = 0; i < server->rooms_size; i++) {
      char room_id[4];
      sprintf(room_id, "%d", i);
      char n_players[4];
      sprintf(n_players, "%d", server->rooms[i].n_players);
      
      strcpy(data + data_length, room_id);
      data_length += 4;
      strcpy(data + data_length, n_players);
      data_length += 4;   
    }
    send_package(player->socket, 0, data_length, data, server);
  } else {
    // If the player is on active room, update the socket fd
    strcpy(player->name, player_name);
    strcpy(player->status, "playing");
    // Update player on room
    server->rooms[room_id].players[player_id] = player;
    printf("Player %s rejoined the game\n", player_name);
    free(old_player);
  }
}

// Enter user in the room
void handle_id_1(player* player, server *server, int id, int data_length, char *data) {
  int room_id = atoi(data);
  room* room = &server->rooms[room_id];
  if (room->n_players == 2) {
    // If the room is full, send error
    send_package(player->socket, 1, 0, NULL, server);
  } else {
    // If the room is not full, add the player to the room
    room->players[room->n_players++] = player;
    strcpy(player->status, "waiting");

    if (room->n_players == 2) {
      // If the room is full, start the game
      for (int i = 0; i < 2; i++) {
        strcpy(room->players[i]->status, "playing");
      }
      // TODO: start_game(room);
    }

    // Remove the player from the lobby
    remove_player_from_lobby(player, &server->lobby);
    printf("Player %s joined the room %d\n", player->name, room_id);
    // Send to the player the room id
    send_package(player->socket, 1, 4, data, server);

    // Send updated room list to all players in the lobby
    char data[255];
    int data_length = 0;
    for (int i = 0; i < server->rooms_size; i++) {
      char room_id[4];
      sprintf(room_id, "%d", i);
      char n_players[4];
      sprintf(n_players, "%d", server->rooms[i].n_players);
      
      strcpy(data + data_length, room_id);
      data_length += 4;
      strcpy(data + data_length, n_players);
      data_length += 4;   
    }
    for (struct lobby_node* node = server->lobby.first; node != NULL; node = node->next) {
      send_package(node->player->socket, 0, data_length, data, server);
    }
  }
}