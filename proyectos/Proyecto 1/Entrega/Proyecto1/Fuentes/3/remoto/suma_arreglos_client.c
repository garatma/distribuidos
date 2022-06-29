/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°3 - RPC - Cliente
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include "suma_arreglos.h"
#include <errno.h>
#include <stdio.h>

void
suma_arreglos_program_1(char *host)
{
	CLIENT *clnt;
	argumento  *result_1;
	argumento  suma_arreglos_1_arg;

	// los primeros 100 elementos de suma_arreglos_1_arg es el primer arreglo
	//   0,  1,  2, ..., 99
	for (int i = 0; i < 100; ++i) {
		suma_arreglos_1_arg.arr[i] = i;
	}

	// los primeros 100 elementos de suma_arreglos_1_arg es el segundo arreglo
	// 100, 99, 98, ...,  1
	for (int i = 100; i < 200; ++i) {
		suma_arreglos_1_arg.arr[i] = 200-i;
	}

	// suma esperada: 100, 100, 100, ..., 100

#ifndef	DEBUG
	clnt = clnt_create (host, SUMA_ARREGLOS_PROGRAM, SUMA_ARREGLOS_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	// declaramos los structs para almacenar lo que retorna gettimeofday().
	struct timeval tiempo_antes, tiempo_despues;

	// marcamos el tiempo justo antes de realizar la RPC.
	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

	// ejecutar la RPC.
	result_1 = suma_arreglos_1(&suma_arreglos_1_arg, clnt);
	if (result_1 == (argumento *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else {
		// tomamos el tiempo justo después de la RPC y realizamos la resta de
		// estos para obtener cuánto duró.
		if (gettimeofday(&tiempo_despues, NULL) == -1) {
			perror("Gettimeofday");
			exit(errno);
		}
		printf("La RPC tardó %ld μs en realizarse.\n", 
				1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
				( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
		);

		// imprimimos lo que devolvió el servidor.
		printf("La suma dió:\n");
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				printf("%i ", result_1->arr[10*i+j]);
			}
			printf("\n");
		}
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	suma_arreglos_program_1 (host);
exit (0);
}
