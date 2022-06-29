#include "operacionesB.h"
#include "constantes.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

char lista_dominios [CANTIDAD_ENTRADAS][LONGITUD_CADENA];
int cantidad_dominios;
time_t tiempo;

int turno_patentar_auto() {
    // simula la obtención de un turno con un número aleatorio.
    int turno_concedido = NO;
    // traducir de número aleatorio a las constantes NO y SI.
    if (rand()%2) turno_concedido = SI;
    return turno_concedido;
}

int turno_transferencia_vehiculo() {
    // simula la obtención de un turno con un número aleatorio.
    int turno_concedido = NO;
    // traducir de número aleatorio a las constantes NO y SI.
    if (rand()%2) turno_concedido = SI;
    return turno_concedido;
}

void informacion_dominio(int numero_patente, char * resultado) {
    if (0 <= numero_patente && numero_patente < cantidad_dominios)
        // existe el dominio que se busca.
        strcpy(resultado, lista_dominios[numero_patente]);
    else
        // no existe el dominio. retornar un nombre vacío.
        strcpy(resultado, " ");
}

void inicializar_lista_dominios() {
    // inicializamos la semilla para los número aleatorios.
    srand((unsigned) time(&tiempo));
    // inicializamos la lista con algunos nombres.
    strcpy(lista_dominios[0], "Chevrolet");
    strcpy(lista_dominios[1], "Ford");
    strcpy(lista_dominios[2], "Fiat");
    strcpy(lista_dominios[3], "Peugeot");
    strcpy(lista_dominios[4], "Toyota");
    cantidad_dominios = 5;
}

