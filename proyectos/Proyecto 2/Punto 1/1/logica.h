#ifndef LOGICA_H
#define LOGICA_H

#include "constantes.h"

struct puntaje {
	// 1 2 3 4 5 6 escalera full poker generala
	int puntos[CANTIDAD_CATEGORIAS];
};

// para inicializar el módulo
extern void inicializar_logica(int *, char ***);

// tira los dados y los devuelve en el arreglo pasado por parámetro
extern void tirar_dados(int [DADOS]);

// descarta el i-ésimo dado pasado por parámetro
extern int descartar(int); 

// devuelve los puntajes para la jugada actual con la siguiente estructura:
//		JUGADA: PUNTAJE punto(s)	
// para los índices donde no hay puntaje (0), se escribe " "
extern void obtener_categorias(char [CANTIDAD_CATEGORIAS][LONGITUD_CADENA]); 

// indica que el índice de dicho puntaje debe quedarse
extern void decidir_puntaje(int);

// devuelve las categorías que se pueden tachar con la siguiente estructura:
//		JUGADA
// para los índices donde no se puede tachar, se escribe " "
extern void obtener_lista_tachar(char [CANTIDAD_CATEGORIAS][LONGITUD_CADENA]);

// indicar que el índice de dicho puntaje debe tacharse
extern void decidir_tachar(int);

// devuelve el puntaje total hasta el momento
extern int puntaje_total();

// retorna no-cero si terminó la partida
extern int partida_terminada();

#endif /* ifndef LOGICA */
