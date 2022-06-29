
/*
	Ejercicio 1 - Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	Cerdá Gianni Lucas - Garat Manuel
*/
#include <sys/time.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	//CLOCK_THREAD_CPUTIME_ID
	//CLOCK_MONOTONIC_RAW 
	struct timeval tiempo_antes, tiempo_despues;

	gettimeofday(&tiempo_antes, NULL);

	for (int i = 0; i < 300000; ++i)
		printf("%i\n", i);

	gettimeofday(&tiempo_despues, NULL);

	printf("sec: %ld\n", (tiempo_despues.tv_sec - tiempo_antes.tv_sec ));
	printf("usec: %ld\n", (tiempo_despues.tv_usec - tiempo_antes.tv_usec ));
	printf("sec*1M + usec (usec): %ld\n", 1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + ( tiempo_despues.tv_usec - tiempo_antes.tv_usec ));
	printf("sec + usec/1M (sec): %f\n", (tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + ( tiempo_despues.tv_usec - tiempo_antes.tv_usec )/1000000.0);

	return EXIT_SUCCESS;
}
