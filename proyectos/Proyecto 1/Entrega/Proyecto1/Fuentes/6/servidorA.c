/*
    - Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
    - Ejercicio N°6 - Servidor agencia A
    - Cerdá Gianni Lucas - Garat Manuel
*/

#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "operacionesA.h"

void cerrar(int fd) {
    if (close(fd) == -1) {
        perror("Close");
        exit(errno);
    }
}

void recibir(int fd, void * mensaje, int size) {
    if (recv(fd, mensaje, size, 0) == -1) {
        perror("Receive");
        exit(errno);
    }
}

void enviar(int fd, void * mensaje, int size) {
    if (send(fd, mensaje, size, MSG_NOSIGNAL) == -1) {
        perror("Send");
        exit(errno);
    }
}

void procesar_entrada(int fd_cliente) {
    int operacion, mantener_conexion = 1;

    while (mantener_conexion) {

        // leer operación
        recibir(fd_cliente, &operacion, sizeof(int));

        switch (operacion) {
            case CODIGO_TURNO_LICENCIA_MATRIMONIO:
                printf("El cliente pidió un turno para una licencia de matrimonio.\n");
                char mensaje_licencia;
                recibir(fd_cliente, &mensaje_licencia, sizeof(mensaje_licencia));

                int salida_turno_licencia_matrimonio;
                salida_turno_licencia_matrimonio = turno_licencia_matrimonio();

                enviar(fd_cliente, &salida_turno_licencia_matrimonio, sizeof(salida_turno_licencia_matrimonio));
                break;
            case CODIGO_INFORMACION_PARTIDA_NACIMIENTO:
                printf("El cliente pidió información de una pertida de nacimiento.\n");
                int numero_partida;
                recibir(fd_cliente, &numero_partida, sizeof(numero_partida));

                char salida_partida [LONGITUD_CADENA];
                informacion_partida_nacimiento(numero_partida, salida_partida);

                enviar(fd_cliente, &salida_partida, sizeof(salida_partida));
                break;
            case CODIGO_TURNO_INSCRIPCION_RECIEN_NACIDO:
                printf("El cliente pidió un turno para una inscripción de un bebé.\n");
                char mensaje_inscripcion;
                recibir(fd_cliente, &mensaje_inscripcion, sizeof(mensaje_inscripcion));

                int salida_turno_inscripcion_bebe;
                salida_turno_inscripcion_bebe = turno_inscripcion_recien_nacido();

                enviar(fd_cliente, &salida_turno_inscripcion_bebe, sizeof(salida_turno_inscripcion_bebe));
                break;
            case CODIGO_TERMINAR:
                printf("El cliente decidió terminar la conexión.\n");
                mantener_conexion = 0;
                break;
            default:
                printf("El cliente pidió un operación que no existe.\n");
                mantener_conexion = 0;
                break;
        }
    }

    printf("Conexión con el cliente cerrada.\n");

    cerrar(fd_cliente);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    // inicializar lista de partidas de nacimiento
    inicializar_lista_partidas_nacimiento();

    int fd_socket;

    // crear socket
    if ((fd_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(errno);
    }

    // para poder volver a utilizar la quíntupla al volver a ejecutar el proceso
    // después de poco tiempo de cerrado el mismo.
    int optval = 1;
    if (setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
        perror("Set socket option");        
        exit(errno);
    }    

    // información necesaria para bind
    struct sockaddr_in direccion_socket;
    direccion_socket.sin_family = AF_INET;
    direccion_socket.sin_port = htons(PUERTO_A);
    direccion_socket.sin_addr.s_addr = INADDR_ANY;
    bzero(&(direccion_socket.sin_zero), 8);

    // obtener nombre para el socket
    if (bind(fd_socket, (struct sockaddr *) &direccion_socket, sizeof(struct sockaddr)) == -1) {
        perror("Bind");
        cerrar(fd_socket);
        exit(errno);
    }

    // permitir que el socket escuche conexiones
    if (listen(fd_socket, CONEXIONES_ABIERTAS) == -1 ) {
        perror("Listen");
        cerrar(fd_socket);
        exit(errno);
    }

    // datos del cliente
    int size_cliente = sizeof(struct sockaddr_in);
    int fd_cliente;
    struct sockaddr_in direccion_cliente;

    while (1) {
        // aceptar conexión de un cliente
        if ((fd_cliente = accept(fd_socket, (struct sockaddr *) &direccion_cliente, (socklen_t *) &size_cliente)) == -1) { 
            perror("Accept");
            continue;
        }

        // procesar conexión
        printf("Se estableció una conexión desde %s\n", inet_ntoa(direccion_cliente.sin_addr));

        // crear un proceso para que administre la conexión
		int pid;
		if (!(pid = fork())) procesar_entrada(fd_cliente);
		else if (pid == -1) perror("Fork");

		// cerrar la conexión
		cerrar(fd_cliente);
    }

    return EXIT_SUCCESS;
}
