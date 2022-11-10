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
    char * username = get_input();
    client_send_message(server_socket, 6, username);
  }

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1){
    int msg_code = client_receive_id(server_socket);
    
    if (msg_code == 0) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      printf("Existen %i salas en el servidor\n", (payload_size-1)/2);

      if ( message == 0) {
        printf("El nombre de usuario ya existe, por favor ingrese otro\n");
        username = get_input();
        client_send_message(server_socket, 0, username);
      } else {
        for (int i = 1; i < payload_size; i+=2){
          printf("Sala %i: %i/%i jugadores  \n", message[i], message[i+1], 2);
        }

        printf("¿A qué sala desea entrar?\n");
        
        char * response = get_input();
        int room = atoi(response);
        char room_id[1];
        room_id[0] = room;

        client_send_message(server_socket, 1, room_id);
      }
      free(message);
    }
    if (msg_code == 1) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);

      if (payload_size == 0){
        printf("No se pudo ingresar a la sala, puede que está este llena, intente con otra sala\n"); 
        client_send_message(server_socket, 2, "");
      } else {
        printf("Se ha ingresado a la sala %i\n", message[0]);
        printf("Esperando a que se conecte el otro jugador...\n");
      }
      free(message);
    }
    if (msg_code == 2) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      
      char ** grid = malloc(5*sizeof(char*));
      for (int i = 0; i < 5; i++){
        grid[i] = malloc(5*sizeof(char));
      }
      for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
          grid[i][j] = message[i*5 + j];
        }
      }

      print_grid(grid);
      for (int i = 0; i < 5; i++){
        free(grid[i]);
      }
      free(grid);

      if (message[25] == 1 ) {
        printf("----- MENÚ DE PREPARACIÓN -----\n\n Para ingresar las coordenadas considera lo siguiente\n \t- Pueden estar separadas por un espacio o por Enter\n \t- Deben ser de la forma Letra Número (por ejemplo A1)\n \t- La letra puede estar en mayúsculas o minúsculas\n\n");
      }
      for (int i = 26; i < payload_size; i ++ ) {
        printf("%c", message[i]);
      }

      char * response = get_input();
      client_send_message(server_socket, 5, response);

      free(message);
    }
    if (msg_code == 3) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      printf("%s", message);
      char* response = get_input();
      client_send_message(server_socket, 5, response);
    }
    if (msg_code == 4) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      printf("Esperando a que el otro jugador coloque sus barcos\n");
      free(message);
    }
    if (msg_code == 5) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      printf("%s\n", message);
      free(message);
    }
    if (msg_code == 6) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      printf("%s\n", message);
      char* response = get_input();
      // Con que id mandar esto?
      client_send_message(server_socket, 7, response);
    }
    if (msg_code == 7) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);

      // Split message in two
      char * message1 = malloc(25 * sizeof(char));
      char * message2 = malloc(25 * sizeof(char));
      for (int i = 0; i < 25; i++){
        message1[i] = message[i];
      }
      for (int i = 25; i < payload_size; i++){
        message2[i-25] = message[i];
      }

      // Print first grid
      printf("Su tablero:\n");

      char ** grid1 = malloc(5*sizeof(char*));
      for (int i = 0; i < 5; i++){
        grid1[i] = malloc(5*sizeof(char));
      }
      for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
          grid1[i][j] = message1[i*5 + j];
        }
      }

      print_grid(grid1);
      for (int i = 0; i < 5; i++){
        free(grid1[i]);
      }
      free(grid1);

      // Print second grid
      printf("Tablero del oponente:\n");

      char ** grid2 = malloc(5*sizeof(char*));
      for (int i = 0; i < 5; i++){
        grid2[i] = malloc(5*sizeof(char));
      }
      for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
          grid2[i][j] = message2[i*5 + j];
        }
      }

      print_grid(grid2);
      for (int i = 0; i < 5; i++){
        free(grid2[i]);
      }

      free(message);
      free(message1);
      free(message2);
    }
    if (msg_code == 8) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      printf("%s\n",message);
      free(message);
    }
    if (msg_code == 9) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      
      printf("La sala está llena, podemos comenzar el juego\n ¿Qué desea hacer?\n 1) Jugar 2) Salir de la sala\n");
      int option = getchar() - '0';
      getchar();
      char* response = "";

      client_send_message(server_socket, option + 2, response);
      free(message);
    }
    if (msg_code == 10) { 
      int payload_size;
      char * message = client_receive_payload(server_socket, &payload_size);
      if (message[0] == 0){
        printf("Has abandonado la sala, espera mientras te enviamos al lobby\n");
        char * response = "";	
        client_send_message(server_socket, 2, response);
      } else {
        printf("El otro jugador ha abandonado la partida, espera mientras llega otro jugador\n");
      }

      free(message);
    }
    printf("------------------\n");
  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
