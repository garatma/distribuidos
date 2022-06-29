/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°1
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	// declaramos los structs para almacenar lo que retorna gettimeofday().
	struct timeval tiempo_antes, tiempo_despues;

	// marcamos el tiempo justo antes de crear el proceso.
	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

	pid_t pid = fork();

	// marcamos el tiempo justo después de crear el proceso.
	if (gettimeofday(&tiempo_despues, NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

	if (pid) 
	{
		// nada más el padre imprime por pantalla, para que no haya dos
		// mensajes.

		// imprimimos la resta entre la marca de después y de antes del fork
		// para obtener el tiempo que llevó el fork.
		printf("El proceso tardó %ld μs en crearse.\n", 
				1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
				( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
		);

	}
	else if (pid == -1)
	{
		printf("Error al crear el proceso. No se pudo medir el tiempo.\n");
		exit(errno);
	}

	// esperamos al hijo si no terminó.
	wait(NULL);

	return EXIT_SUCCESS;
}
