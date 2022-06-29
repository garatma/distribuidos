/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°7 - RPC - Servidor
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include "offset.h"

struct timeval tiempo;
static struct salida result;

struct salida *
offset_1_svc(void *argp, struct svc_req *rqstp)
{

	if (gettimeofday(&tiempo, NULL) == -1)
		result.tipo = ERROR;
	else
		result.tipo = TIEMPO;

	result.salida_u.tiempo = 1000000*tiempo.tv_sec + tiempo.tv_usec;

	return &result;
}
