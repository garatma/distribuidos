/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°2 - Fork - RPC - Cliente
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include "proceso.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	// se inicializa el cliente - servidor
	char *server;
	CLIENT *clnt;
	int iteraciones;
	int  *result_1;

	if (argc < 3) {
		printf ("usage: %s server_host iteraciones \n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	server = argv[1];

	// control de entrada.
	if (strlen(argv[2]) > 10) {
		printf("Ingresó un número muy grande (mayor que diez dígitos).\n");
		exit(EXIT_FAILURE);
	}
	
	// convertir de string a entero las iteraciones. si el argumento no es
	// válido, se realizan 0 iteraciones.
	iteraciones = atoi(argv[2]);
	
	// declaramos los structs para almacenar lo que retorna gettimeofday().
	struct timeval tiempo_antes, tiempo_despues;
	
	clnt = clnt_create (server, CREARPROCESOPROG, CREARPROCESOVERS, "udp"); //Establesco conexión  con el servidor
	if (clnt == (CLIENT *) NULL) {
		clnt_pcreateerror (server);
		exit(EXIT_FAILURE);
	}
	
	// marcamos el tiempo justo antes de realizar la RPC.
	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}
	
	result_1 = tarea_1(&iteraciones, clnt);
	
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else
	{
		if (*result_1 == -1) {
			// servidor dio error en algo.
			printf("Error en el servidor. No se pudo medir el tiempo.\n");
			exit(EXIT_FAILURE);
		}
		if (gettimeofday(&tiempo_despues, NULL) == -1) {
			perror("Gettimeofday");
			exit(errno);
		}
		printf("La RPC tardó %ld μs en realizarse.\n", 
				1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
				( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
		);
	}	
	
	clnt_destroy (clnt);
	
	return EXIT_SUCCESS;
}
