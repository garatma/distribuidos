const MAX_OPERANDOS_SUMA_RESTA = 4;
const BINARIO = 0;
const HEXA = 1;
const ENTERO = 2;
const ERROR = -1;
const SIZE_CADENA_BINARIO = 33;
const SIZE_CADENA_HEXA = 9;

/*
union entrada switch(int operacion) {
case SUMA_RESTA:
	int operandos[4];
	int cantidad;
case MULTIPLICACION_DIVISION:
	int operandos[2];
case DECIMAL_BINARIO:
	int operando;
case BINARIO_HEXA:
	char operando[SIZE_CADENA_BINARIO];
};
*/

struct entrada_suma_resta {
	int operandos[4];
	int cantidad;
};

struct entrada_multiplicacion_division {
	int operandos[2];
};

union salida switch(int tipo) {
	case BINARIO:
		char binario[SIZE_CADENA_BINARIO];
	case HEXA:
		char hexa[SIZE_CADENA_HEXA];
	case ENTERO:
		int entero;
	default:
		void;
};

struct entrada_binario_hexa {
	char operando[SIZE_CADENA_BINARIO];
};

program DECIMAL_BINARIO_PROGRAMA {
	version DECIMAL_BINARIO_VERSION {
		salida decimal_binario(int) = 1;
	} = 1;
} = 0x20000000;

program BINARIO_HEXA_PROGRAMA {
	version BINARIO_HEXA_VERSION {
		salida binario_HEXA(entrada_binario_hexa) = 1;
	} = 1;
} = 0x20000001;

program SUMA_PROGRAMA {
	version SUMA_VERSION {
		salida suma(entrada_suma_resta) = 1;
	} = 1;
} = 0x20000002;

program RESTA_PROGRAMA {
	version RESTA_VERSION {
		salida resta(entrada_suma_resta) = 1;
	} = 1;
} = 0x20000003;

program MULTIPLICACION_PROGRAMA {
	version MULTIPLICACION_VERSION {
		salida multiplicacion(entrada_multiplicacion_division) = 1;
	} = 1;
} = 0x20000004;

program DIVISION_PROGRAMA {
	version DIVISION_VERSION {
		salida division(entrada_multiplicacion_division) = 1;
	} = 1;
} = 0x20000005;
