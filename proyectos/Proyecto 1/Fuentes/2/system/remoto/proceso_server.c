/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°2 - System - RPC - Servidor
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include "proceso.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int *
tarea_1_svc(int *argp, struct svc_req *rqstp)
{
	printf("Llamada de cliente recibida.\n");
	static int  result;

	int pid = vfork();
	char command [20];
	
	if(sprintf(command,"./tarea %i",*argp)<0){
		printf("Error del servidor\n");
		result=-1;
	}
	
	if (!pid){
		printf ("La tarea ejecutó %i iteraciones \n",*argp);
		system(command);	
	}
	else if (pid == -1){
		printf("Error al crear el proceso.\n");
		result=-1;
	}
	else printf("Proceso creado con éxito.\n");
	
	return &result;
}
