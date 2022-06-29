/* 
 * Ejercicio 4 - Laboratorio 2
 * Alumnos: Cerdá Gianni Lucas - Garat Manuel
 * 
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

int main (){
	struct timeval ini,fin;
	FILE *file;
	long timedif;
	int pipefd[2];
	char letra;

// Medimos el tiempo de escritura en el archivo

	file =fopen("escribir.txt","w");
	if (file==NULL){
		printf("No se pudo abrir el archivo.\n");
		exit (EXIT_FAILURE);
	}
	
	gettimeofday(&ini,NULL); // Inicializamos el reloj
	fprintf (file,"Escritura en el archivo.\n"); //Escribo en el archivo
	gettimeofday(&fin,NULL); // Fin del reloj
	timedif = (fin.tv_sec - ini.tv_sec)*1000000 + (fin.tv_usec - ini.tv_usec);//Calculamos el tiempo en ejecucion
	printf (" El tiempo para escribir en un archivo es: %lu microsegundos \n",timedif);
	fclose(file);

// Medimos el tiempo de escritura en el pipe
		
	if (pipe(pipefd)== -1){
		printf("Error en la creacion del pipe\n");
		exit(EXIT_FAILURE);
	}

	gettimeofday(&ini,NULL); // Inicializamos el reloj
	write(pipefd[1],"Escritura en el pipe.\n",sizeof(letra)*22); //Escribo en el pipe 
	gettimeofday(&fin,NULL); // Fin del reloj
	timedif = (fin.tv_sec - ini.tv_sec)*1000000 + (fin.tv_usec - ini.tv_usec);//Calculamos el tiempo en ejecucion
	printf (" El tiempo para escribir en un pipe es: %lu microsegundos \n",timedif);
	close(pipefd[0]);
	close(pipefd[1]);
	
	return 0;
}






			

	

