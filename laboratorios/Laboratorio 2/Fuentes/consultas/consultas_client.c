/* 
 * Ejercicio 8 - Laboratorio 2
 * Alumnos: Cerdá Gianni Lucas - Garat Manuel
 * 
*/

#include "consultas.h"
#include <stdlib.h>

void
tiempo_prog_1(char *host)
{
	CLIENT *clnt;
	struct salida *result_1;
	char *tiempo_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, TIEMPO_PROG, TIEMPO_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = tiempo_1((void*)&tiempo_1_arg, clnt);
	if (result_1 == (struct salida *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else
		printf("La hora local en el servidor es: %s\n", result_1->salida_u.cadena);
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


void
maquina_prog_1(char *host)
{
	CLIENT *clnt;
	struct salida *result_1;
	char *maquina_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, MAQUINA_PROG, MAQUINA_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = maquina_1((void*)&maquina_1_arg, clnt);
	if (result_1 == (struct salida *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else
		printf("El nombre del servidor es: %s\n", result_1->salida_u.cadena);
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


void
usuarios_prog_1(char *host)
{
	CLIENT *clnt;
	struct salida  *result_1;
	char *usuarios_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, USUARIOS_PROG, USUARIOS_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = usuarios_1((void*)&usuarios_1_arg, clnt);
	if (result_1 == (struct salida *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else
		printf("La cantidad de usuarios logueados en el servidor es: %i\n", result_1->salida_u.entero);
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 3) {
		printf ("uso: %s host operacion\n", argv[0]);
		printf("Operacion:\n0: Tiempo Local\n1: Nombre de la máquina\n2: Cantidad de usuarios logueados\n");
		exit (1);
	}

	host = argv[1];

	int op = atoi(argv[2]);

	if (op < 0 || op > 2) {
		printf("No existe esa operación.\n");
		exit(1);
	}

	switch (op) {
		case 0:
			tiempo_prog_1(host);
			break;
		case 1:
			maquina_prog_1(host);
			break;
		case 2:
			usuarios_prog_1(host);
			break;
		default:
			printf("No existe esa operación.\n");
			exit(1);
			break;
	}

	return 0;
}
