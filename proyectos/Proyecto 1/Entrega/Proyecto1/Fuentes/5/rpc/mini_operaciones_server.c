/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°5 - RPC - Servidor
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include "mini_operaciones.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void invertir_string(char * string, int max) {
	int j = max-1, aux;
	for (int i = 0; i < max/2; ++i) {
		aux = string[i];	
		string[i] = string[j];
		string[j--] = aux;
	}
}

salida *
decimal_binario_1_svc(int * argp, struct svc_req *rqstp)
{
	static salida  result;

	printf("El cliente pidió la conversión de decimal a binario.\n");

	if (*argp < 0)
		result.tipo = ERROR;
	else {
		result.tipo = BINARIO;
		int i = 0, numero = *argp;

		// se guarda el resultado invertido en result
		while (numero > 0) {
			result.salida_u.binario[i++] = (numero % 2) + '0';
			numero /= 2;
		}

		// agregar fin de string en la próxima posición
		result.salida_u.binario[i] = '\0';

		invertir_string(result.salida_u.binario, i);
	}

	return &result;
}

salida *
binario_hexa_1_svc(entrada_binario_hexa *argp, struct svc_req *rqstp)
{
	static salida  result;

	printf("El cliente pidió la conversión de binario a hexa.\n");

	int i = 0;
	// obtener la posición del último dígito
	while (argp->operando[i] != '\0') {
		// devolver error si un dígito no es 0 y no es 1
		if (argp->operando[i] != '0' && argp->operando[i] != '1') {
			result.tipo = ERROR;
			return &result;
		}
		i++;
	}

	result.tipo = HEXA;

	// convertir a decimal
	int decimal = 0, j = 0, actual;
	i--;
	while (i >= 0) {
		actual = argp->operando[i] - '0';
		decimal += actual * pow(2,j);
		i--; j++;
	}

	// convertir a hexa
	int resto = 0, cantidad_caracteres = 0;
	const char * caracteres_hexa = "0123456789ABCDEF";
	while (decimal != 0) {
		resto = decimal % 16;
		decimal /= 16;
		result.salida_u.hexa[cantidad_caracteres] = caracteres_hexa[resto % 16];
		cantidad_caracteres++;
	}
	result.salida_u.hexa[cantidad_caracteres] = '\0';

	invertir_string(result.salida_u.hexa, cantidad_caracteres);

	return &result;
}

salida *
suma_1_svc(entrada_suma_resta *argp, struct svc_req *rqstp)
{
	static salida  result;
	printf("El cliente pidió una suma.\n");

	result.tipo = ENTERO;
	result.salida_u.entero = 0;

	// no hace chequeos de overflow
	for (int i = 0; i < argp->cantidad; ++i)
		result.salida_u.entero += argp->operandos[i];

	return &result;
}

// retorna op1 - op2 - op3 - op4
salida *
resta_1_svc(entrada_suma_resta *argp, struct svc_req *rqstp)
{
	static salida  result;
	printf("El cliente pidió una resta.\n");

	result.tipo = ENTERO;
	result.salida_u.entero = 0;

	// no hace chequeos de overflow
    result.salida_u.entero = argp->operandos[0];
	for (int i = 1; i < argp->cantidad; ++i)
		result.salida_u.entero -= argp->operandos[i];

	return &result;
}

salida *
multiplicacion_1_svc(entrada_multiplicacion_division *argp, struct svc_req *rqstp)
{
	static salida  result;
	printf("El cliente pidió una multiplicación.\n");

	result.tipo = ENTERO;
	result.salida_u.entero = argp->operandos[0] * argp->operandos[1];

	return &result;
}

salida *
division_1_svc(entrada_multiplicacion_division *argp, struct svc_req *rqstp)
{
	static salida  result;
	printf("El cliente pidió una división.\n");

	if (argp->operandos[1] == 0)
		result.tipo = ERROR;
	else {
		result.tipo = ENTERO;
		result.salida_u.entero = argp->operandos[0] / argp->operandos[1];
	}

	return &result;
}
