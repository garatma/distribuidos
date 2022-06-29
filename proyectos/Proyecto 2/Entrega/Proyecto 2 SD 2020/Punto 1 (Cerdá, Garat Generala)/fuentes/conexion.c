#include <stdlib.h>
#include "constantes.h"
#include "conexion.h"
#include "generala.h"
#include "logica.h"

static CLIENT * clnt;
static int * id;
int categorias_remotas[CANTIDAD_CATEGORIAS];
int offset = 0;

void inicializar_conexion(int argc, char ** argv)
{
	// Localhost
	char * host = "localhost";
	if(argc!=1) host=argv[1];	

	//Creo el cliente
	clnt=clnt_create(host,GENERALA,VERSGENERALA,"udp");
	if (clnt==NULL){
		clnt_pcreateerror(host);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < CANTIDAD_CATEGORIAS; ++i) {
		categorias_remotas[i] = 0;
	}
}

void anotarse_conexion()
{
	// El cliente se anota en el servidor
	id = anotarse_1(NULL,clnt);
	if (id== (int*) NULL) {
		clnt_perror(clnt,"Call Failed-->Anotarse");
		exit(EXIT_FAILURE);
	}
	if (*id == 0) offset = CANTIDAD_CATEGORIAS;
}

void empezar_conexion()
{
	int * resultado=empezar_1(NULL, clnt);
	if (resultado==(int *)NULL) {
		clnt_perror(clnt,"Call Failed-->Empezar");
		exit(EXIT_FAILURE);
	}
	if (*resultado==0)
		printf("No puede empezar el juego porque solo hay un jugador. Cuando otro jugador inicie, podrá empezar a jugar.\n");
}

void esperar_otro_jugador_conexion() {
	int * jugando, hay_2_jugadores = 0;
	while (!hay_2_jugadores) {
		jugando=jugadores_1(NULL, clnt);
		hay_2_jugadores = *jugando != -1;
		if (jugando == (int*) NULL){
			clnt_perror(clnt,"Call Failed---> Jugando");
			exit(EXIT_FAILURE);
		}
	}
}

int permiso_conexion()
{
	int * resultado;
	resultado=permiso_1(id,clnt);
	if (resultado == (int*) NULL){
		clnt_perror(clnt,"Call Failed---> Permiso1");
		exit(EXIT_FAILURE);
	}
	return *resultado;
}

void jugadaanterior_conexion(int dados_remotos[DADOS])
{
	struct jugada * jugada_anterior;
	jugada_anterior = jugadaanterior_1(NULL,clnt);
	if (jugada_anterior == (struct jugada *)NULL) {
		clnt_perror(clnt,"Call Failed -->Jugada Anterior");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < DADOS; ++i) {
		dados_remotos[i] = jugada_anterior->dados[i];
	}
}

void resultadosparciales_conexion(int * indice, int * puntaje)
{
	struct tabla * resultado = resultadosparciales_1(id,clnt);
	if (resultado == (struct tabla *)NULL) {
		clnt_perror(clnt,"Call Failed -->Resultados Parciales");
		exit(EXIT_FAILURE);
	}

	int encontre_diferencia = 0, i = 0;
	while (!encontre_diferencia && i < CANTIDAD_CATEGORIAS) {
		encontre_diferencia = categorias_remotas[i] != resultado->valores[i];
		i++;
	}

	if (encontre_diferencia) {
		categorias_remotas[i-1] = resultado->valores[i-1];
		*indice = i-1;
		*puntaje = resultado->puntaje[i-1];
	}
}

void jugar_conexion(int dados[DADOS])
{
	struct dados * resultado;
	resultado = jugar_1(NULL, clnt);
	if(resultado== (struct dados *)NULL) {
		clnt_perror(clnt,"Call Failed--> Jugar");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < DADOS; ++i) {
		dados[i] = resultado->numeros[i];
	}
}

int tirar_conexion(int dado)
{
	int * resultado = tirar_1(&dado,clnt);
	if(resultado==(int*) NULL) {
		clnt_perror(clnt,"Call Failed--> VolverTirarDados");
		exit(EXIT_FAILURE);
	}
	return *resultado;
}

int anotarresultados_conexion(int indice, int puntaje)
{
	struct tabla argumento;
	for (int i = 0; i < CANTIDAD_CATEGORIAS; ++i) {
		argumento.valores[i] = (i == indice);
		if (i == indice) {
			argumento.valores[i] = 1;
			argumento.puntaje[i] = puntaje;
		} else {
			argumento.valores[i] = 0;
		}
	}
	int * resultado=anotarresultados_1(&argumento,clnt);
	if (resultado==(int*) NULL) {
		clnt_perror(clnt,"Call Failed-->AnotarCategoria");
		exit(EXIT_FAILURE);
	}
	return *resultado;
}

void tachar_conexion(int indice)
{
	struct tabla argumento;
	for (int i = 0; i < CANTIDAD_CATEGORIAS; ++i) {
		argumento.valores[i] = (i == indice);
	}
	int * resultado=tachar_1(&argumento, clnt);
	if(resultado == (int *) NULL){
		clnt_perror(clnt,"Call Error-->TacharCategoria");
		exit(EXIT_FAILURE);
	}
}

void terminar_conexion()
{
	struct tabla * resultado=terminar_1(id,clnt);
	if(resultado== (struct tabla*) NULL){
		clnt_perror(clnt,"Call Failed--> Terminar");
		exit(EXIT_FAILURE);
	}
}

void finalizarjugadores_conexion()
{
	int * resultado, terminaron_todos = 0;
	while (!terminaron_todos) {
		resultado=finalizarjugadores_1(id, clnt);
		if(resultado==(int*) NULL) {
			clnt_perror(clnt,"Call Failed--> Finalizar");
			exit(EXIT_FAILURE);
		}
		terminaron_todos = resultado != 0;
	}
}
