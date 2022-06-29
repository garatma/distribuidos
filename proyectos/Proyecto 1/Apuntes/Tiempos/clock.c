
/*
	Ejercicio 1 - Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	Cerdá Gianni Lucas - Garat Manuel
*/
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	//CLOCK_THREAD_CPUTIME_ID
	//CLOCK_MONOTONIC_RAW 
	struct timespec tiempo_antes, tiempo_despues;

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo_antes);

	for (int i = 0; i < 300000; ++i)
		printf("%i\n", i);



	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo_despues);

	printf("sec: %ld\n", tiempo_despues.tv_sec - tiempo_antes.tv_sec);
	printf("nsec: %ld\n",  (tiempo_despues.tv_nsec - tiempo_antes.tv_nsec));
	printf("sec*1000M + nsec (nsec): %ld\n",  (tiempo_despues.tv_nsec - tiempo_antes.tv_nsec) + 1000000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec));
	printf("sec + nsec/1000M (sec): %lf\n",  (tiempo_despues.tv_nsec - tiempo_antes.tv_nsec)/1000000000.0 + (tiempo_despues.tv_sec - tiempo_antes.tv_sec));

	return EXIT_SUCCESS;
}
