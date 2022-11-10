

# Documentación Mensajes de Cliente a Server

Ids

  - 0: Recibe el nombre del usuario, y lo agrega al loby
  - 1: Recibe el id de la sala a la que ingresar
  - 2: Petición del cliente para obtener lista de salas actualizadas
  - 3: Confirmación del cliente que está listo para jugar
  - 4: Petición del jugador de salir de la sala
  - 5: Lista de coordenadas de las posiciones de los barcos. El payload es un arreglo de char de largo 5 con las coordenadas de inicio y fin de un barco: [<inicio_barco> <fin_barco>].
  - 6: Confirmación o no confirmación de coordenadas seleccionadas.
  - 7: Mensaje para la fase de juego.
  - 8: Respuesta a si quiere esperar o no al jugador que se desconectó. El payload será un 1 si desea esperarlo y un 0 si desea salir de la sala.
  - 9: Confirmación del cliente para volver al lobby una vez terminada la partida. 



# Documentación Mensajes de Server a Cliente

Ids

  - 0: Recibe la lista de salas desde el server. El primer byte del buffer es 1 si la petición fue exitosa y 0 en caso contrario. Luego, el buffer está ordenado en pares de bytes, donde el primer 1 bytes es el id de la sala, y el segundo 1 bytes es la cantidad de jugadores en ella, luego se repite hasta completar el largo del buffer.
  - 1: Responde al intento de ingreso en una sala. Si el mensaje viene vacío la sala está llena, sino, viene con el id de la sala a la que se ingresó correctamente.
  - 2: Petición de ingresar las coordenadas de los barcos. Los primeros 25 bytes viene el tablero a imprimir, el siguiente byte es un entero (1 si se tiene que imprimir el menu y 0 si no), y luego el mensaje para indicar la coordenada de cual barco mandar.
  - 3: Coordenadas no válidas, en el payload va el mensaje por error para imprimir.
  - 4: Petición de coonfirmación de coordenadas. Los primeros 25 bytes viene el tablero a imprimir.
  - 5: Recibe texto a imprimir.
  - 6: Recibe texto a imprimir y envia el input.
  - 7: Recibe 2 tableros a imprimir.
  - 8: Juego terminado
  - 9: La sala en la que el usuario ingresó se llenó, el servidor espera la confirmación de jugar o salir.
  - 10: Alguno de los dos jugadores se salió antes de empezar el juego. Payload: 0 si es el que se salió, 1 si es que te quedarás esperando.
  - 11: El otro jugador ha salido de la partida, el servidor espera si desea esperarlo o no.
