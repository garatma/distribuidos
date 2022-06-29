#ifndef CONEXION_H
#define CONEXION_H

#include "constantes.h"

extern void inicializar_conexion(int, char **);
extern void anotarse_conexion();
extern void empezar_conexion();
extern void esperar_otro_jugador_conexion();
extern int permiso_conexion();
extern void jugadaanterior_conexion(int[DADOS]);
extern void resultadosparciales_conexion(int *, int *);
extern void jugar_conexion(int [DADOS]);
extern int tirar_conexion(int);
extern int anotarresultados_conexion(int, int);
extern void tachar_conexion(int);
extern void terminar_conexion();
extern void finalizarjugadores_conexion();

// jugada anterior?

#endif /* ifndef CONEXION */
