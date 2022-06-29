#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#define DADOS 5 
#define CARAS 6

void tirar_dados(int mano[DADOS]) 
{
    for (int i = 0; i < DADOS; ++i) 
        mano[i] = 1 + random()%CARAS;
}
               
void print_mano(int mano[DADOS])
{
    for (int i = 0; i < DADOS; ++i) 
        printf("%i: %i\n", i, mano[i]);
}

int main(int argc, char *argv[])
{
	struct timeval tiempo;

    /* medimos el tiempo */ 
	if (gettimeofday(&tiempo,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

    /* inicializamos la semilla de la función aleatoria con el tiempo medido */
    srandom(tiempo.tv_sec + 1000000*tiempo.tv_usec);

    int mano[DADOS] = {0,0,0,0,0};

    tirar_dados(mano);
    print_mano(mano);

    return 0;
}
