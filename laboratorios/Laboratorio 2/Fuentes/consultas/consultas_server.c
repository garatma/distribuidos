/* 
 * Ejercicio 8 - Laboratorio 2
 * Alumnos: Cerdá Gianni Lucas - Garat Manuel
 * 
*/

#include "consultas.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct salida *
tiempo_1_svc(void *argp, struct svc_req *rqstp)
{
	static struct salida result;

	printf("Un cliente quiere saber la hora local en esta máquina.\n");

	time_t tiempo_local;
	struct tm * tm_info;

	tiempo_local = time(NULL);
	tm_info = localtime(&tiempo_local);

	strftime(result.salida_u.cadena, SIZE_CADENA-1, "%a %b %d %H:%M:%S %Y", tm_info);

	return &result;
}

struct salida *
maquina_1_svc(void *argp, struct svc_req *rqstp)
{
	static struct salida result;

	printf("Un cliente quiere saber el nombre de esta máquina.\n");

	if (gethostname(result.salida_u.cadena, SIZE_CADENA-1) == -1)
		result.tipo = ERROR;
	else
		result.tipo = CADENA;

	return &result;
}

struct salida *
usuarios_1_svc(void *argp, struct svc_req *rqstp)
{
	static struct salida result;

	printf("Un cliente quiere saber la cantidad de usuarios logueados en esta máquina.\n");

	if (system("who | wc -l > usuarios") == -1 )
		result.tipo = ERROR;
	else {
		FILE * fd = fopen("usuarios", "r");
		if (fd == NULL)
			result.tipo = ERROR;
		else { 
			if (fscanf(fd, "%i", &result.salida_u.entero) == EOF)
				result.tipo = ERROR;
			else
				result.tipo = ENTERO;
		}
		fclose(fd);
	}

	return &result;
}
