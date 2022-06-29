/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°3 - LC
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

struct argumento {
	char arr [2048];
};

// si bien no es recomendable pasar estructuras completas como argumento ni
// retornarlas (es mejor utilizar punteros a las mismas), preferimos respetar
// exactamente lo que dice el enunciado: un arreglo de 2048 bytes de entrada y 
// uno de salida.
struct argumento sumar_arreglos(struct argumento entrada)
{
	struct argumento salida;

	// los primeros 100 elementos de la salida tienen la suma de los arreglos
	// que contiene la entrada (0-99 y 100-199).
	for (int i = 0; i < 100; ++i) {
		salida.arr[i] = entrada.arr[i] + entrada.arr[i+100];
	}
	
	return salida;
}

int main(int argc, char *argv[])
{
	struct argumento entrada;

	// los primeros 100 elementos de suma_arreglos_1_arg es el primer arreglo
	//   0,  1,  2, ..., 99
	for (int i = 0; i < 100; ++i) {
		entrada.arr[i] = i;
	}

	// los primeros 100 elementos de suma_arreglos_1_arg es el segundo arreglo
	// 100, 99, 98, ...,  1
	for (int i = 100; i < 200; ++i) {
		entrada.arr[i] = 200-i;
	}

	// suma esperada: 100, 100, 100, ..., 100
	
	// declaramos los structs para almacenar lo que retorna gettimeofday().
	struct timeval tiempo_antes, tiempo_despues;

	// marcamos el tiempo justo antes de realizar la LC.
	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

	// llamamos a la función.
	struct argumento salida = sumar_arreglos(entrada);

	// tomamos el tiempo justo después de la LC y realizamos la resta de
	// estos para obtener cuánto duró.
	if (gettimeofday(&tiempo_despues, NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}
	printf("La LC tardó %ld μs en realizarse.\n", 
			1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
			( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
	);

	// imprimimos la salida de una forma fácil de leer.
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			printf("%i ", salida.arr[10*i+j]);
		}
		printf("\n");
	}

	return EXIT_SUCCESS;
}
