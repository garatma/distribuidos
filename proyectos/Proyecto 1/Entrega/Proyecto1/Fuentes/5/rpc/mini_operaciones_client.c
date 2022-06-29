/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°5 - RPC - Cliente
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include "mini_operaciones.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>

// para no declarar en cada función
struct timeval tiempo_antes, tiempo_despues;

void
decimal_binario_programa_1(char *host)
{
	CLIENT *clnt;
	salida  *result_1;
	int decimal_binario_1_arg;

	printf("Ingrese el argumento: ");
	int ret = scanf("%i", &decimal_binario_1_arg);

	if (ret != 1) {
		if (ret == 0) printf("El operando ingresado es incorrecto.\n");
		else perror("Scanf");
		exit(EXIT_FAILURE);
	}

#ifndef	DEBUG
	clnt = clnt_create (host, DECIMAL_BINARIO_PROGRAMA, DECIMAL_BINARIO_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}
	result_1 = decimal_binario_1(&decimal_binario_1_arg, clnt);
	if (result_1 == (salida *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else {
		if (gettimeofday(&tiempo_despues, NULL) == -1) {
			perror("Gettimeofday");
			exit(errno);
		}
		printf("La llamada tardó %ld μs en ejecutarse.\n", 
				1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
				( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
		);
		if (result_1->tipo == BINARIO)
			printf("El resultado es: %s\n", result_1->salida_u.binario);
		else
			printf("El servidor devolvió un error al calcular la operación.\n");
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

void
binario_hexa_programa_1(char *host)
{
	CLIENT *clnt;
	salida  *result_1;
	entrada_binario_hexa binario_hexa_1_arg;

	// leer un número de pantalla
	printf("Ingrese el argumento: ");
	int ret = scanf("%32s", binario_hexa_1_arg.operando);

	if (ret != 1) {
		if (ret == 0) printf("El operando ingresado es incorrecto.\n");
		else perror("Scanf");
		exit(EXIT_FAILURE);
	}

#ifndef	DEBUG
	clnt = clnt_create (host, BINARIO_HEXA_PROGRAMA, BINARIO_HEXA_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}
	result_1 = binario_hexa_1(&binario_hexa_1_arg, clnt);
	if (result_1 == (salida *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else {
		if (gettimeofday(&tiempo_despues, NULL) == -1) {
			perror("Gettimeofday");
			exit(errno);
		}
		printf("La llamada tardó %ld μs en ejecutarse.\n", 
				1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
				( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
		);
		if (result_1->tipo == HEXA)
			printf("El resultado es: %s\n", result_1->salida_u.hexa);
		else
			printf("El servidor devolvió un error al calcular la operación.\n");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

void operandos_suma_resta(entrada_suma_resta * argumentos)
{
	int numero, ret;
	argumentos->cantidad = 0;
	for (int i = 0; i < MAX_OPERANDOS_SUMA_RESTA; i++) {
		if (i < 2)
			printf("Ingrese el %i° operando: ", i+1);
		else
			printf("Ingrese el %i° operando (0 para no ingresar nada): ", i+1);

		ret = scanf("%i", &numero);

		if (ret != 1) {
			if (ret == 0) printf("El operando ingresado es incorrecto.\n");
			else perror("Scanf");
			exit(EXIT_FAILURE);
		}

		// si ingresó un cero y ya ingresó dos argumentos: salir del while
		if (numero == 0 && i > 1)
			break;

		argumentos->cantidad++;
		argumentos->operandos[i] = numero;
	}
}

void
suma_programa_1(char *host)
{
	CLIENT *clnt;
	salida  *result_1;
	entrada_suma_resta  suma_1_arg;
	
	operandos_suma_resta(&suma_1_arg);

#ifndef	DEBUG
	clnt = clnt_create (host, SUMA_PROGRAMA, SUMA_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}
	result_1 = suma_1(&suma_1_arg, clnt);
	if (result_1 == (salida *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else {
		if (gettimeofday(&tiempo_despues, NULL) == -1) {
			perror("Gettimeofday");
			exit(errno);
		}
		printf("La llamada tardó %ld μs en ejecutarse.\n", 
				1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
				( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
		);
		if (result_1->tipo == ENTERO)
			printf("El resultado es: %i\n", result_1->salida_u.entero);
		else
			printf("El servidor devolvió un error al calcular la operación.\n");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


void
resta_programa_1(char *host)
{
	CLIENT *clnt;
	salida  *result_1;
	entrada_suma_resta  resta_1_arg;

	operandos_suma_resta(&resta_1_arg);
#ifndef	DEBUG
	clnt = clnt_create (host, RESTA_PROGRAMA, RESTA_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}
	result_1 = resta_1(&resta_1_arg, clnt);
	if (result_1 == (salida *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else {
		if (gettimeofday(&tiempo_despues, NULL) == -1) {
			perror("Gettimeofday");
			exit(errno);
		}
		printf("La llamada tardó %ld μs en ejecutarse.\n", 
				1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
				( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
		);
		if (result_1->tipo == ENTERO)
			printf("El resultado es: %i\n", result_1->salida_u.entero);
		else
			printf("El servidor devolvió un error al calcular la operación.\n");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

void operandos_multiplicacion_division(entrada_multiplicacion_division * argumentos) {
	int numero, ret;
	for (int i = 0; i < 2; ++i) {
		printf("Ingrese el %i° operando: ", i+1);
		ret = scanf("%i", &numero);

		if (ret != 1) {
			if (ret == 0) printf("El operando ingresado es incorrecto.\n");
			else perror("Scanf");
			exit(EXIT_FAILURE);
		}

		argumentos->operandos[i] = numero;
	}
}

void
multiplicacion_programa_1(char *host)
{
	CLIENT *clnt;
	salida  *result_1;
	entrada_multiplicacion_division multiplicacion_1_arg;

	operandos_multiplicacion_division(&multiplicacion_1_arg);

#ifndef	DEBUG
	clnt = clnt_create (host, MULTIPLICACION_PROGRAMA, MULTIPLICACION_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}
	result_1 = multiplicacion_1(&multiplicacion_1_arg, clnt);
	if (result_1 == (salida *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else {
		if (gettimeofday(&tiempo_despues, NULL) == -1) {
			perror("Gettimeofday");
			exit(errno);
		}
		printf("La llamada tardó %ld μs en ejecutarse.\n", 
				1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
				( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
		);
		if (result_1->tipo == ENTERO)
			printf("El resultado es: %i\n", result_1->salida_u.entero);
		else
			printf("El servidor devolvió un error al calcular la operación.\n");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


void
division_programa_1(char *host)
{
	CLIENT *clnt;
	salida  *result_1;
	entrada_multiplicacion_division  division_1_arg;

	operandos_multiplicacion_division(&division_1_arg);

#ifndef	DEBUG
	clnt = clnt_create (host, DIVISION_PROGRAMA, DIVISION_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}
	result_1 = division_1(&division_1_arg, clnt);
	if (result_1 == (salida *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else {
		if (gettimeofday(&tiempo_despues, NULL) == -1) {
			perror("Gettimeofday");
			exit(errno);
		}
		printf("La llamada tardó %ld μs en ejecutarse.\n", 
				1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
				( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
		);
		if (result_1->tipo == ENTERO)
			printf("El resultado es: %i\n", result_1->salida_u.entero);
		else
			printf("El servidor devolvió un error al calcular la operación.\n");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

void mensaje_operaciones() {
	printf("0: Decimal a binario\n1: Binario a hexa\n2: Suma\n3: Resta\n");
	printf("4: Multiplicación\n5: División\n6: Este mensaje\n7: Salir\n");
}

int main (int argc, char *argv[]) {
	char *host;

	if (argc < 2) {
		printf("Debe especificar el host del servidor.\n");
		exit (EXIT_FAILURE);
	}

	host = argv[1];

	mensaje_operaciones();
	while (1) {
		// si la operación ingresada no es un número, el default es 0.
		int operacion;
		printf("Ingrese una operación: ");
		int ret = scanf("%i", &operacion);
		if (ret != 1) {
			if (ret == -1) perror("Scanf");
			else printf("La operación ingresada es incorrecta.\n");
			exit(EXIT_FAILURE);
		}

		if (operacion > 7 || operacion < 0) {
			printf("La operacion ingresada es incorrecta.\n");
			exit(EXIT_FAILURE);
		}

		switch (operacion) {
			case 0:
				decimal_binario_programa_1(host);
				break;
			case 1:
				binario_hexa_programa_1(host);
				break;
			case 2:
				suma_programa_1(host);
				break;
			case 3:
				resta_programa_1(host);
				break;
			case 4:
				multiplicacion_programa_1(host);
				break;
			case 5:
				division_programa_1(host);
				break;
			case 6:
				mensaje_operaciones();
				break;
			case 7:
				exit(EXIT_SUCCESS);
				break;
			default:
				printf("Operación incorrecta.\n");
				exit(EXIT_FAILURE);
				break;
		}
	}

	return EXIT_SUCCESS;
}
