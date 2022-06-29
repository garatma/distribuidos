/* 
 * Ejercicio 8 - Laboratorio 2
 * Alumnos: Cerdá Gianni Lucas - Garat Manuel
 * 
*/

const SIZE_CADENA = 101;
const CADENA = 0;
const ENTERO = 1;
const ERROR = 2;

union salida switch(int tipo) {
	case CADENA:
		char cadena[SIZE_CADENA];
	case ENTERO:
		int entero;
	default:
		void;
};

program TIEMPO_PROG{
	version TIEMPO_VERS{
		 salida tiempo()=1;
	}=1;
} =0x20000000;

program MAQUINA_PROG{
	version MAQUINA_VERS{
		salida maquina()=1;
	}=1;
} =0x20000001;

program USUARIOS_PROG{
	version USUARIOS_VERS{
		salida usuarios()=1;
	}=1;
} =0x20000002;

