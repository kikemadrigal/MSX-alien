# instructions

Eres un mecenas de la tierra y debes de cumplir una serie de misiones para que tu empresa pueda progesar y descrubrir nuevos mundos

# Play online

https://github.com/kikemadrigal/captainkik

# Structure
El juego tiene la siguiente estructura:
    -archivo main.c que llama al archivo man/game.c el cual contiene 2 métodos: 1. inicialización juego y 2. jugar
    -Una carpeta man (managers):
        -entity.c: se encarga de inicializar las entidades en espacios de memoria RAM y de asignarles un valor
        -game.c: se encarga de inicializar(Crea los mapas y los screens,crea los sprites e inicializa los valores de los sistemas de renderizado, etc)
        -graphics.c: se encarga de dibujar en la pantalla lo que le digan

