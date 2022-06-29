/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°3 - RPC - Servidor
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include "suma_arreglos.h"

argumento *
suma_arreglos_1_svc(argumento *argp, struct svc_req *rqstp)
{
	static argumento  result;

	printf("Recibí una llamada de un cliente.\n");

	// los primeros 100 elementos de la salida tienen la suma de los arreglos
	// que contiene la entrada (0-99 y 100-199).
	for (int i = 0; i < 100; ++i) {
		result.arr[i] = argp->arr[i] + argp->arr[i+100];
	}

	printf("Suma realizada.\n");

	return &result;
}
