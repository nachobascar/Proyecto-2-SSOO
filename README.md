

# Documentación Mensajes de Cliente a Server

Ids

  - 0: Recibe el nombre del usuario, y lo agrega al loby


# Documentación Mensajes de Server a Cliente

Ids

  - 0: Recibe la lista de salas desde el server. El buffer está ordenado en pares de byte, donde el primer byte es el id de la sala, y el segundo byte es la cantidad de jugadores en ella.