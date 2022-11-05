

# Documentación Mensajes de Cliente a Server

Ids

  - 0: Recibe el nombre del usuario, y lo agrega al loby
  - 1: Recibe el id de la sala a la que ingresar


# Documentación Mensajes de Server a Cliente

Ids

  - 0: Recibe la lista de salas desde el server. El buffer está ordenado en pares de bytes, donde los primeros 4 bytes son el id de la sala, y los segundos 4 bytes son la cantidad de jugadores en ella, luego se repite hasta completar el largo del buffer.
  - 1: Responde al intento de ingreso en una sala. Si el mensaje viene vacío la sala está llena, sino, viene con el id de la sala a la que se ingresó correctamente.