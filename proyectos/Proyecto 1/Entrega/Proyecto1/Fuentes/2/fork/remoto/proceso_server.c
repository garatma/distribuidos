/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°2 - Fork - RPC - Servidor
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include "proceso.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int *
tarea_1_svc(int *argp, struct svc_req *rqstp)
{
	printf("Llamada de cliente recibida.\n");
	static int result=0;

	int pid = vfork();

	if (!pid) {
		// hijo

		// las iteraciones a ejecutar se pasan como argumento a la imagen.
		char iteraciones [20];
		
		if (sprintf(iteraciones,"%i",*argp)<0)
		{
			printf("Error del servidor\n");
			result=-1;
		}

		// cargar la imagen ejecutable.
		char * cmd [] = {"./tarea", iteraciones, NULL};
		int ret = execv("./tarea",cmd);

		if (ret == -1) {
			printf("Error al cargar la imagen ejecutable.\n");
			result = -1;
		}
	}
	else if (pid == -1)
	{
		printf("Error al crear el proceso.\n");
		result=-1;
	}
	else printf("Proceso creado con éxito.\n");

	return &result;
}
