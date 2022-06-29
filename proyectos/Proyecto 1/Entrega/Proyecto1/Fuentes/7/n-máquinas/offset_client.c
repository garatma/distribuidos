/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°7 - RPC - Cliente
	- Cerdá Gianni Lucas - Garat Manuel
*/

// implementación del algoritmo de Cristian:

#include <errno.h>
#include "offset.h"

#define LONGITUD_HOST 31

int offset_programa_1(long int * diferencia, char *host)
{
	CLIENT *clnt;
	struct salida  *result_1;
	char *offset_1_arg;

	clnt = clnt_create (host, OFFSET_PROGRAMA, OFFSET_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		return ERROR;
	}

	struct timeval tiempo_antes, tiempo_despues;

	if(gettimeofday(&tiempo_antes, NULL) == -1) {
		perror("Gettimeofday");
		return ERROR;
	}

	result_1 = offset_1((void*)&offset_1_arg, clnt);

	if(gettimeofday(&tiempo_despues, NULL) == -1) {
		perror("Gettimeofday");
		return ERROR;
	}

	if (result_1 == (struct salida *) NULL) {
		clnt_perror (clnt, "call failed");
		return ERROR;
	}
	else {
		if (result_1->tipo == ERROR) {
			printf("El servidor no pudo calcular el tiempo.\n");
			return ERROR;
		}
		long int tiempo_servidor = result_1->salida_u.tiempo;

		// momento en el que el cliente calcula que el servidor mide su tiempo:
		// tiempo_antes + (tiempo_despues - tiempo_antes)/2
		long int tiempo_cliente = 1000000*tiempo_antes.tv_sec + tiempo_antes.tv_usec - (1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec) + 
			(tiempo_despues.tv_usec - tiempo_antes.tv_usec))/2;

		*diferencia = tiempo_servidor - tiempo_cliente; 
	}

	clnt_destroy (clnt);

	return TIEMPO;
}

void borrar_nueva_linea(char * arreglo) {
	for (int i = 0; i < LONGITUD_HOST; ++i)
		if (arreglo[i] == '\n') {
			arreglo[i] = '\0';
			break;
		}
}

int
main (int argc, char *argv[])
{
	char host [LONGITUD_HOST];
	int seguir_testeando = 1, cantidad_tests = 0;
	long int suma_difs = 0, diferencia_actual; 

	while (seguir_testeando) {
		printf("Ingrese un host: ");
		if (fgets(host, sizeof(host)-1, stdin) == NULL) {
			printf("Host mal ingresado.\n");
		}
		else if (host[0] != '\n') {

			// fgets copia un fin de línea. hay que sacarlo para que sea lea 
			// correctamente el host.
			borrar_nueva_linea(host);
			if (offset_programa_1(&diferencia_actual, host) != ERROR) {
				printf("Diferencia de tiempo actual: %li\n", diferencia_actual);
				suma_difs += diferencia_actual;
				cantidad_tests++;
			}
		}
		else seguir_testeando = 0;
	}

	if (cantidad_tests != 0) {
		printf("%i test[s] indica[n] que el cliente está ", cantidad_tests);

		if (suma_difs < 0) printf("atrasado %li μs.\n", -suma_difs/cantidad_tests);
		else printf("adelantado %li μs.\n", suma_difs/cantidad_tests);
	}

	return 0;
}
