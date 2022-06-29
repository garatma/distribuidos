#include "operacionesA.h"
#include "constantes.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

char lista_partidas [CANTIDAD_ENTRADAS][LONGITUD_CADENA];
int cantidad_partidas;
time_t tiempo;

int turno_licencia_matrimonio() {
    // simula la obtención de un turno con un número aleatorio.
    int turno_concedido = NO;
    // traducir de número aleatorio a las constantes NO y SI.
    if (rand()%2) turno_concedido = SI;
    return turno_concedido;
}

void informacion_partida_nacimiento(int numero_partida, char * resultado) {
    if (0 <= numero_partida && numero_partida < cantidad_partidas)
        // existe la partida que se busca.
        strcpy(resultado, lista_partidas[numero_partida]);
    else
        // no existe la partida. retornar un nombre vacío.
        strcpy(resultado, " ");
}

int turno_inscripcion_recien_nacido() {
    // simula la obtención de un turno con un número aleatorio.
    int turno_concedido = NO;
    // traducir de número aleatorio a las constantes NO y SI.
    if (rand()%2) turno_concedido = SI;
    return turno_concedido;
}

void inicializar_lista_partidas_nacimiento() {
    // inicializamos la semilla para los número aleatorios.
    srand((unsigned) time(&tiempo));
    // inicializamos la lista con algunos nombres.
    strcpy(lista_partidas[0], "Marcos");
    strcpy(lista_partidas[1], "Mario");
    strcpy(lista_partidas[2], "Daphne");
    strcpy(lista_partidas[3], "Liliana");
    strcpy(lista_partidas[4], "Jean");
    cantidad_partidas = 5;
}

