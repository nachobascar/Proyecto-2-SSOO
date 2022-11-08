

# Documentación Mensajes de Cliente a Server

Ids

  - 0: Recibe el nombre del usuario, y lo agrega al loby
  - 1: Recibe el id de la sala a la que ingresar
  - 2: Petición del cliente para obtener lista de salas actualizadas
  - 3: Confirmación del cliente que está listo para jugar
  - 4: Petición del jugador de salir de la sala
  - 5: Lista de coordenadas de las posiciones de los barcos. El payload es un arreglo de char de largo 16 con las coordenadas de inicio y fin de cada barco: [<inicio_barco_largo_2><fin_barco_largo_2><inicio_barco_largo_3><fin_barco_largo_3><inicio_barco_largo_4><fin_barco_largo_4>].
  - 6: Confirmación de coordenadas seleccionadas.


# Documentación Mensajes de Server a Cliente

Ids

  - 0: Recibe la lista de salas desde el server. El primer byte del buffer es 1 si la petición fue exitosa y 0 en caso contrario. Luego, el buffer está ordenado en pares de bytes, donde los primeros 4 bytes son el id de la sala, y los segundos 4 bytes son la cantidad de jugadores en ella, luego se repite hasta completar el largo del buffer.
  - 1: Responde al intento de ingreso en una sala. Si el mensaje viene vacío la sala está llena, sino, viene con el id de la sala a la que se ingresó correctamente.
  - 2: Petición de ingresar las coordenadas de los barcos. Si se le pidió porque partió la fase de preparación del juego, el payload va vacío. Si es por algun error, hay un mensaje de error en el payload (ej: las coordenadas enviadas no son válidas, se deben enviar nuevamente).
 - 3: Confirmación de que las coordenadas son válidas. En el payload se entrega el tablero.
 - 4: 
