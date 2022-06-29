/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°2 - Fork Local
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{
	// pasamos nuestro argumento como argumento a la imagen, que representa
	// las iteraciones.
	char * iteraciones = 0;
	if (argc != 2){
		printf ("Debe especificar la cantidad de iteraciones a ejecutar.\n");
		exit(EXIT_FAILURE);
	}	
	else
		iteraciones = argv[1];

	// control de entrada.
	if (strlen(argv[1]) > 10) {
		printf("Ingresó un número muy grande (mayor que diez dígitos).\n");
		exit(EXIT_FAILURE);
	}

	// declaramos los structs para almacenar lo que retorna gettimeofday().
	struct timeval tiempo_antes, tiempo_despues;

	// marcamos el tiempo justo antes de crear el proceso.
	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

	// crear proceso
	int pid = vfork();

	if (!pid) {
		// hijo

		// cargar la imagen ejecutable.
		char * cmd [] = {"./tarea ", iteraciones, NULL};
		int ret = execv("./tarea",cmd);

		if (ret == -1) {
			printf("Error al cargar la imagen ejecutable.\n");
			printf("La medición no será correcta.\n");
		}
	}
	else if (pid == -1)
	{
		printf("Error al crear el proceso.\n");
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
