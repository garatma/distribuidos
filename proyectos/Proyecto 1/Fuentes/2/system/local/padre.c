/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°2 - System - Local
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	// declaramos los structs para almacenar lo que retorna gettimeofday().
	struct timeval tiempo_antes, tiempo_despues;

	if (argc < 2) {
		printf("Debe ingresar la cantidad de iteraciones.\n");
		exit(EXIT_FAILURE);
	}

	// control de entrada.
	if (strlen(argv[1]) > 10) {
		printf("Ingresó un número muy grande (mayor que diez dígitos).\n");
		exit(EXIT_FAILURE);
	}

	// creamos el comando y le pasamos como argumento nuestro argumento.
	char comando [20];
	strcpy(comando, "./tarea ");
	char * com = strcat(comando,argv[1]);

	// marcamos el tiempo justo antes de llamar a system.
	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

	// ejecutamos "com" en un shell
	int ret = system(com);

	// si es -1 -> no se pudo crear el proceso.
	// si es >0 y !=127-> error del shell, probablemente no encontró el comando.
	if (ret == -1 || (ret > 0 && ret != 127) ) {
		printf("Error al crear el nuevo proceso.\n");
		exit(errno);
	}

	// marcamos el tiempo justo después de la ejecución del hijo.
	if (gettimeofday(&tiempo_despues, NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}
	printf("El proceso tardó %ld μs en crear y ejecutar su imagen.\n", 
			1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
			( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
	);

	return EXIT_SUCCESS;
}
