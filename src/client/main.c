#include <unistd.h>
#include <stdio.h>
#include "helpers/conection.h"
#include "helpers/comunication.h"
#include "helpers/helpers.h"

char * get_input(){
  char * response = malloc(20);
  int pos=0;
  while (1){
    char c = getchar();
    if (c == '\n') break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}


int main (int argc, char *argv[]){
  if (argc != 5) {
    printf("Usage: %s  -i <ip_address> -p <tcp_port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  char IP[16];
  int PORT;
  for (int i = 1; i < argc; i+=2) {
    if (strcmp(argv[i], "-i") == 0) {
      strcpy(IP, argv[i + 1]);
    } else if (strcmp(argv[i], "-p") == 0) {
      PORT = atoi(argv[i + 1]);
    } else {
      printf("Usage: %s  -i <IP> -p <tcp_port>\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  printf("IP address: %s\n", IP);
  printf("TCP port: %d\n\n", PORT);

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);

  printf("Bienvenido! Ingrese su nombre de usuario: \n");
  char * username = get_input();

  // Prueba fase de preparacion
  client_send_message(server_socket, 0, username);
  while (1) {
    char response[4];
    printf("Ingrese la coordenada de inicio del barco de largo 2\n");
    response[0] = get_input();
    printf("Ingrese la coordenada de fin del barco de largo 2\n");
    response[2] = get_input();
    client_send_message(server_socket, 6, response);
  }

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1){
    int msg_code = client_receive_id(server_socket);
    
    if (msg_code == 0) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      printf("Existen %i salas en el servidor\n", (payload_size)/8);

      if ((char) message == '0') {
        printf("El nombre de usuario ya existe, por favor ingrese otro\n");
        username = get_input();
        client_send_message(server_socket, 0, username);
      } else {
        for (int i = 1; i < payload_size; i+=8){
          printf("Sala %i: %i/%i jugadores  \n", (i/8) + 1, message[i], message[i+4]);
        }

        printf("¿A qué sala desea entrar?\n");
        
        char * response = get_input();

        client_send_message(server_socket, 1, response);
      }
      free(message);
    }
    if (msg_code == 1) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);

      if (payload_size == 0){
        printf("La sala está llena, intente con otra sala\n"); 
        client_send_message(server_socket, 2, "");

      } else {
        printf("Se ha unido a la sala %i\n", message[0]);
        printf("Que desea hacer?\n 1) Jugar\n 2) Salir de la sala\n");
        int option = getchar() - '0';
        getchar();
        client_send_message(server_socket, option+2, "");

      }

      free(message);
    }
    if (msg_code == 2) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);

      if (payload_size == 0) {
        printf("Fase de preparación iniciada\n");
      } else {
        printf(message);
      }
      
      char * grid = malloc(5*5*sizeof(char));
      print_grid(&grid);

      char response[12];
      
      printf("Ingrese la coordenada de inicio del barco de largo 2\n");
      response[0] = get_input();
      printf("Ingrese la coordenada de fin del barco de largo 2\n");
      response[2] = get_input();
      printf("Ingrese la coordenada de inicio del barco de largo 3\n");
      response[4] = get_input();
      printf("Ingrese la coordenada de fin del barco de largo 3\n");
      response[6] = get_input();
      printf("Ingrese la coordenada de inicio del barco de largo 4\n");
      response[8] = get_input();
      printf("Ingrese la coordenada de fin del barco de largo 4\n");
      response[10] = get_input();

      client_send_message(server_socket, 5, response);
      
      free(grid);
      free(message);
    }
    if (msg_code == 3) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      printf("Su tablero es:\n");
      print_grid(&message);
      printf("¿Está seguro que desea continuar?\n 1) Sí\n 2) No\n");
      int option = getchar() - '0';
      getchar();
      if (option == 2) {
        char * grid = malloc(5*5*sizeof(char));
        print_grid(&grid);

        char response[12];
        
        printf("Ingrese la coordenada de inicio del barco de largo 2\n");
        response[0] = get_input();
        printf("Ingrese la coordenada de fin del barco de largo 2\n");
        response[2] = get_input();
        printf("Ingrese la coordenada de inicio del barco de largo 3\n");
        response[4] = get_input();
        printf("Ingrese la coordenada de fin del barco de largo 3\n");
        response[6] = get_input();
        printf("Ingrese la coordenada de inicio del barco de largo 4\n");
        response[8] = get_input();
        printf("Ingrese la coordenada de fin del barco de largo 4\n");
        response[10] = get_input();

        client_send_message(server_socket, 5, response);
        
        free(grid);
      } else {
        client_send_message(server_socket, 6, "");
      }
      free(message);
    }
    if (msg_code == 4) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      printf("Esperando a que el otro jugador coloque sus barcos\n");
      free(message);
    }
    printf("------------------\n");
  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
