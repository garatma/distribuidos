/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°7 - RPC - Cliente
	- Cerdá Gianni Lucas - Garat Manuel
*/

// implementación del algoritmo de Cristian:

// medir el tiempo antes de la RPC: t_antes
// realizar RPC -> se obtiene el tiempo del servidor: t_servidor
// medir el tiempo luego de la RPC: t_despues

// t_cliente = t_antes + (t_despues - t_antes)/2
// diferencia: t_servidor - t_cliente

// si la diferencia es negativa: el cliente está adelantado
// si la diferencia es positiva: el cliente está atrasado

#include <errno.h>
#include "offset.h"

void
offset_programa_1(char *host)
{
	CLIENT *clnt;
	struct salida  *result_1;
	char *offset_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, OFFSET_PROGRAMA, OFFSET_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	struct timeval tiempo_antes, tiempo_despues;

	if(gettimeofday(&tiempo_antes, NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

	result_1 = offset_1((void*)&offset_1_arg, clnt);

	if(gettimeofday(&tiempo_despues, NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

	if (result_1 == (struct salida *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else {
		if (result_1->tipo == ERROR) {
			printf("El servidor no pudo calcular el tiempo.\n");
			exit(EXIT_FAILURE);
		}
		long int tiempo_servidor = result_1->salida_u.tiempo;

		// momento en el que el cliente calcula que el servidor mide su tiempo:
		// tiempo_antes + (tiempo_despues - tiempo_antes)/2
		long int tiempo_cliente = 1000000*tiempo_antes.tv_sec + tiempo_antes.tv_usec -
			(1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec) + 
			(tiempo_despues.tv_usec - tiempo_antes.tv_usec))/2;

		long int diferencia = tiempo_servidor - tiempo_cliente; 
		if (diferencia < 0) printf("El cliente está adelantado %li μs.\n", -diferencia);
		else printf("El cliente está atrasado %li μs.\n", diferencia);
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

	offset_programa_1 (host);

	return 0;
}
