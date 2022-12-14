#include "comunication.h"

int client_receive_id(int client_socket){
  // Se obtiene solamente el ID del mensaje
  int id = 0;
  recv(client_socket, &id, 1, 0);
  return id;
}

char * client_receive_payload(int client_socket, int* payload_size){
  // Se obtiene el largo del payload
  char payload_s[1];
  recv(client_socket, payload_s, 1, 0);
  *payload_size = payload_s[0];
  if (*payload_size == 0){
    return NULL;
  } else if (*payload_size < 0){
    *payload_size = 256 + *payload_size;
  }
  // Se obtiene el payload
  char * payload = malloc(*payload_size);
  int received = recv(client_socket, payload, *payload_size, 0);
  // Se retorna
  return payload;
}

void client_send_message(int client_socket, int pkg_id, char * message){
  int payloadSize = strlen(message) + 1; //+1 para considerar el caracter nulo. 
  //Esto solo es válido para strings, Ustedes cuando armen sus paquetes saben exactamente cuantos bytes tiene el payload.
  
  // Se arma el paquete
  char msg[1+1+payloadSize];
  msg[0] = pkg_id;
  msg[1] = payloadSize;
  memcpy(&msg[2], message, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 2+payloadSize, 0);
}