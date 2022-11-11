

# Documentación Mensajes de Cliente a Server:

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



# Documentación Mensajes de Server a Cliente:

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
  - 20: Recibir imagen por haber ganado.


## Consideraciones adicionales:

  - La opción para salir del juego se presenta solamente al momento de llenarse la sala y pedirse la confirmación para partir jugando. Una salida en cualquier otro punto se considera una desconexión del jugador.
  - Para el manejo de sesión se pide un nombre inicial, que no debe estar ya utilizado. Esto permite una futura reconexión al juego (Se implementó el bonus de reconexión de jugador).
  - Se maneja la desconexión del jugador de la siguiente forma:
    - Si se desconecta estando en el lobby, o en una sala que aún no comienza la fase de preparación, entonces se elimina al jugador del servidor.
    - Si la fase de preparación ya comenzó, entonces se marca al jugador como desconectado, y se pregunta al oponente si desea esperar o ganar la partida. Si era el turno del oponente para disparar, se espera a realizar el disparo y luego se avisa de la desconexión, en cambio si el oponente estaba colocaldo sus barcos, se espera a que termine de colocar los barcos y de confirmarlos, y luego se le avisa de la desconexión.
    - El jugador puede volver a conectarse con su nombre de usuario, si estaba en fase de preparación se retoma desde donde la dejó, si yá tenía el tablero confirmado se le pide una reconfirmación de los barcos, y si entra en etapa de juego entonces retoma su turno de forma normal.
    - El oponente tiene la opción de seguir jugando o ganar la partida, y esta opción la puede tomar incluso si el jugador ya se reconectó, lo que le otorga al oponente un tiempo de 1 turno para tomar esta desición.
  - El manejo de control de input es limitado, por lo que se espera que el jugador cumpla con los formatos informados en los mensajes. Es por esto que asumimos (por la información del enunciado) que un manejo exhaustivo sobre los inputs otorgados va más allá de los objetivos de esta tarea. Es decir, que si el programa se cae por un input mal entregado por parte del cliente no se considera un error ya que queda el alcance limitado en ese ámbito.
  - Se incluyó también el bonus de la imagen. Al ganar, se le descarga al jugador una imagen de recompensa en su directorio de ejecución.
  - Se agrega el comando secreto ```bomb``` que puede ser ingresado en vez de una coordenada durante la etapa de juego y permite ganar la partida inmediatamente (fue utilizado para debuguear más rápido).

