/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Tarea
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char*argv[]){
    // cantidad de iteraciones 
    int iteraciones = 0;
	

	if (argc != 2){
		printf ("Error en cantidad de parámetros.\n");
		exit(EXIT_FAILURE);
	}	
    else
	{
		// convierto a entero. si el argumento no es un número, o es menor a 0,
		// se le asigna 0.
		iteraciones = atoi(argv[1]);
		if (iteraciones < 0)
			iteraciones = 0;
	}

#ifdef SP
	int s, p;
#endif


#ifdef SP
	for (int i = 0; i < iteraciones; ++i) {
		p = i - 10;
		s = i + 12;
		i++;
	}
#endif	

	FILE * archivo;
	archivo = fopen("salida","wt");

	if (archivo == NULL) {
		printf("No se pudo crear el archivo.\n");
		exit(EXIT_FAILURE);
	}

	int ret = fprintf(archivo,"Proyecto 1 Sistemas Distribuidos 2020.\nTarea ejecutada %i veces.",iteraciones);

	if (ret < 0) {
		printf("No se pudo escribir en el archivo.\n");
		exit(EXIT_FAILURE);
	}

	fclose(archivo);

	return EXIT_SUCCESS;
}
