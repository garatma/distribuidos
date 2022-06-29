#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include "logica.h"
#include "conexion.h"
#include "gui.h"

static const char * nombres_categorias[CANTIDAD_CATEGORIAS] = {
	"Número 1", "Número 2", "Número 3", "Número 4", "Número 5", "Número 6",
	"Escalera", "Full", "Poker", "Generala", "Generala Doble"
};

int puntaje_remoto[CANTIDAD_CATEGORIAS];

static struct puntaje puntaje, puntaje_tentativo;

static int dados[DADOS];

static int dados_ultima_consulta[DADOS];

pthread_t no_colgar_gui;
int turno;

void tirar_dados(int dados_gui[DADOS])
{
	jugar_conexion(dados);
	for (int i = 0; i < DADOS; ++i) { dados_gui[i] = dados[i]; }
}

int descartar(int dado_a_descartar)
{ 
	int nuevo_dado = tirar_conexion(dado_a_descartar);
	dados[dado_a_descartar] = nuevo_dado;
	return nuevo_dado;
}

void inicializar(struct puntaje * puntaje)
{
	for (int i = 0; i < CANTIDAD_CATEGORIAS; i++) {
		puntaje->puntos[i] = 0;
		puntaje_remoto[i] = 0;
	}
}

void crear_mapa(int dados[DADOS], int mapa[CARAS])
{
	for (int i = 0; i < DADOS; ++i) mapa[dados[i]-1]++;

	// resultado:
	//  en posición 0: cantidad de veces que aparece el dado 1
	//  en posición 1: cantidad de veces que aparece el dado 2
	// ...
}

int detectar_escalera(int mapa[CARAS])
{
	// escalera implica las siguiente posibilidades de ocurrencias:

	// 1 1 1 1 0 o
	// ^	 ^

	// 0 1 1 1 1
	//   ^	 ^

	int hubo_escalera = 1, i = 0, f = CARAS-2;

	// verificar en qué caso estamos.
	// si hay cero ocurrencias de uno, controlar a partir del 2
	if (mapa[0] != 1) {
		i = 1; // segundo caso.
		f = CARAS-1;
	}

	while (hubo_escalera && i <= f) hubo_escalera &= (mapa[i++] == 1);
	return hubo_escalera;
}

// detecta si en el mapa hay cierta cantidad de apariciones de un mismo dado.
int detectar_cantidad_ocurrencias(int mapa[CARAS], int cantidad)
{
	int hubo = 0;
	for (int i = 0; i < CARAS; ++i)
		if (mapa[i] == cantidad) hubo++;

	return hubo;
}

void detectar_puntaje(int dados[DADOS], int mapa[CARAS], struct puntaje * puntaje_tentativo, struct puntaje * puntaje)
{
	for (int i = 0; i < CARAS; ++i) mapa[i] = 0;
	crear_mapa(dados, mapa);

	// números
	for (int i = 0; i < CARAS; ++i)
		if (!puntaje->puntos[i] && puntaje->puntos[i] != -1 && mapa[i]) puntaje_tentativo->puntos[i] = mapa[i]*(i+1);

	// escalera
	if (!puntaje->puntos[INDICE_ESCALERA] && puntaje->puntos[INDICE_ESCALERA] != -1) {
		int hubo_escalera = detectar_escalera(mapa);
		if (hubo_escalera) puntaje_tentativo->puntos[INDICE_ESCALERA] = PUNTAJE_ESCALERA;
	}

	// full
	if (!puntaje->puntos[INDICE_FULL] && puntaje->puntos[INDICE_FULL] != -1) {
		int hubo_full = detectar_cantidad_ocurrencias(mapa, 2);
		hubo_full &= detectar_cantidad_ocurrencias(mapa, 3);
		if (hubo_full) puntaje_tentativo->puntos[INDICE_FULL] = PUNTAJE_FULL;
	}

	int hubo_generala = 0;

	if (!puntaje->puntos[INDICE_GENERALA] && puntaje->puntos[INDICE_GENERALA] != -1) {
		hubo_generala = detectar_cantidad_ocurrencias(mapa, 5);
		if (hubo_generala) puntaje_tentativo->puntos[INDICE_GENERALA] = PUNTAJE_GENERALA;
	}
	else if(!puntaje->puntos[INDICE_GENERALA_DOBLE] && puntaje->puntos[INDICE_GENERALA_DOBLE] != -1) {
		hubo_generala = detectar_cantidad_ocurrencias(mapa, 5);
		if (hubo_generala) puntaje_tentativo->puntos[INDICE_GENERALA_DOBLE] = PUNTAJE_GENERALA_DOBLE;
	}

	// poker
	if (!puntaje->puntos[INDICE_POKER] && puntaje->puntos[INDICE_POKER] != -1) {
		int hubo_poker = detectar_cantidad_ocurrencias(mapa, 4);
		if (hubo_poker) puntaje_tentativo->puntos[INDICE_POKER] = PUNTAJE_POKER;
		else if (!hubo_generala && puntaje->puntos[INDICE_GENERALA] && puntaje->puntos[INDICE_GENERALA_DOBLE]) {
			// si ya hubo generala y generala doble en otro turno, una generala es un póker
			hubo_poker = detectar_cantidad_ocurrencias(mapa, 5);
			if (hubo_poker) puntaje_tentativo->puntos[INDICE_POKER] = PUNTAJE_POKER;
		}
	}
}

void obtener_categorias(char categorias[CANTIDAD_CATEGORIAS][LONGITUD_CADENA])
{
	int mapa[CARAS];
	for (int i = 0; i < CANTIDAD_CATEGORIAS; ++i) puntaje_tentativo.puntos[i] = 0; 
	detectar_puntaje(dados, mapa, &puntaje_tentativo, &puntaje);
	for (int i = 0; i < CANTIDAD_CATEGORIAS; ++i) {
		if (puntaje_tentativo.puntos[i] > 0 && !puntaje.puntos[i] && puntaje.puntos[i] != -1) {
			if (puntaje_tentativo.puntos[i] == 1)
				sprintf(categorias[i], "%s: %i punto", nombres_categorias[i], puntaje_tentativo.puntos[i]);
			else 
				sprintf(categorias[i], "%s: %i puntos", nombres_categorias[i], puntaje_tentativo.puntos[i]);
		}
		else sprintf(categorias[i], " ");
	}
}

void obtener_lista_tachar(char lista[CANTIDAD_CATEGORIAS][LONGITUD_CADENA])
{
	for (int i = 0; i < CANTIDAD_CATEGORIAS; ++i)
		if (!puntaje.puntos[i] && puntaje.puntos[i] != -1) strcpy(lista[i], nombres_categorias[i]);
		else strcpy(lista[i], " ");
}

int igual_ultima_consulta(int dados_arg[DADOS])
{
	int iguales = 1;
	for (int i = 0; i < DADOS; ++i) {
		iguales = iguales && dados_arg[i] == dados_ultima_consulta[i];
		dados_ultima_consulta[i] = dados_arg[i];
	}
	return iguales;
}

void esperar_mi_turno()
{
	int es_mi_turno = 0;
	int dados_remotos[DADOS];
	while (!es_mi_turno) {
		es_mi_turno = permiso_conexion();
		if (!es_mi_turno) {
			// actualizar los dados con los del oponente
			jugadaanterior_conexion(dados_remotos);
			if (!igual_ultima_consulta(dados_remotos)) actualizar_dados(0, dados_remotos);
		}
	}
}

int termino_remoto()
{
	int seguir_evaluando = 1, i = 0;
	while (seguir_evaluando && i < CANTIDAD_CATEGORIAS) {
		seguir_evaluando &= (puntaje_remoto[i] != 0);
		i++;
	}
	return seguir_evaluando;
}

int total_remoto()
{
	int suma = 0;
	for (int i = 0; i < CANTIDAD_CATEGORIAS; ++i) {
		if (puntaje_remoto[i] > 0) suma += puntaje_remoto[i];
	}
	return suma;
}

void * manejo_nuevo_turno()
{
	// si no es el primer turno -> avisar que terminé el turno al servidor
	if (turno) { esperar_mi_turno(); }

	esperar_mi_turno();

	int puntaje = 0, indice = -1;
	resultadosparciales_conexion(&indice, &puntaje);

	if (!puntaje) puntaje = -1;

	if (indice > -1) puntaje_remoto[indice] = puntaje;

	if (termino_remoto()) puntaje_total_remoto(total_remoto());
	activar_gui(indice, puntaje);

	pthread_exit(EXIT_SUCCESS);
	return NULL;
}

void decidir_tachar(int indice)
{
	for (int i = 0; i < DADOS; ++i) { dados[i] = 0; }
	puntaje.puntos[indice] = -1;
	tachar_conexion(indice);
	turno = 1;
	pthread_create(&no_colgar_gui, NULL, manejo_nuevo_turno, NULL);
}

void decidir_puntaje(int indice)
{
	for (int i = 0; i < DADOS; ++i) { dados[i] = 0; }
	puntaje.puntos[indice] = puntaje_tentativo.puntos[indice];
	anotarresultados_conexion(indice, puntaje.puntos[indice]);
	turno = 1;
	pthread_create(&no_colgar_gui, NULL, manejo_nuevo_turno, NULL);
}

int puntaje_total()
{
	int suma = 0;
	for (int i = 0; i < CANTIDAD_CATEGORIAS; ++i)
		if (puntaje.puntos[i] > 0) suma += puntaje.puntos[i];
	return suma;
}

int partida_terminada()
{
	int seguir_evaluando = 1, i = 0;
	while (seguir_evaluando && i < CANTIDAD_CATEGORIAS) {
		seguir_evaluando &= (puntaje.puntos[i] != 0);
		i++;
	}
	return seguir_evaluando;
}

void inicializar_logica(int * argc, char *** argv)
{
	inicializar(&puntaje);

	struct timeval tiempo;

	/* medimos el tiempo */
	if (gettimeofday(&tiempo,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

	/* inicializamos la semilla de la función aleatoria con el tiempo medido */
	srandom(tiempo.tv_sec + 1000000*tiempo.tv_usec);

	/* nos conectamos con el servidor */
	inicializar_conexion(*argc, *argv);
	anotarse_conexion();
	empezar_conexion();
	esperar_otro_jugador_conexion();
	turno = 0;
	pthread_create(&no_colgar_gui, NULL, manejo_nuevo_turno, NULL);
}
