#include "fecha_so.h"
#include <string.h>

void ejercicio9_program_1(char *host)
{
	CLIENT *clnt;
	char * *result_1;
	char *so_1_arg;
	char * *result_2;
	char *fecha_hora_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, EJERCICIO9_PROGRAM, EJERCICIO9_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = so_1((void*)&so_1_arg, clnt);
	if (result_1 == (char **) NULL)
		clnt_perror (clnt, "call failed");
	else
		printf("El sistema operativo del servidor es: %s.\n", *result_1);

	result_2 = fecha_hora_1((void*)&fecha_hora_1_arg, clnt);
	if (result_2 == (char **) NULL)
		clnt_perror (clnt, "call failed");
    else
		printf("La hora del servidor es: %s.\n", *result_2);

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
	ejercicio9_program_1 (host);
    return 0;
}
