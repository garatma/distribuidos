/*
    - Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
    - Ejercicio N°6 - Servidor agencia B
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
#include "operacionesB.h"

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
    if (send(fd, mensaje, size, 0) == -1) {
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
            case CODIGO_TURNO_PATENTAR_AUTO:
                printf("El cliente pidió un turno para patentar un auto.\n");
                char mensaje_dominio;
                recibir(fd_cliente, &mensaje_dominio, sizeof(mensaje_dominio));

                int salida_turno_patentar_auto;
                salida_turno_patentar_auto = turno_patentar_auto();

                enviar(fd_cliente, &salida_turno_patentar_auto, sizeof(salida_turno_patentar_auto));
                break;
            case CODIGO_TURNO_TRANSFERENCIA_VEHICULO:
                printf("El cliente pidió un turno para la transferencia de un vehículo.\n");
                char mensaje_transferencia;
                recibir(fd_cliente, &mensaje_transferencia, sizeof(mensaje_transferencia));

                int salida_turno_transferencia_vehiculo;
                salida_turno_transferencia_vehiculo = turno_transferencia_vehiculo();

                enviar(fd_cliente, &salida_turno_transferencia_vehiculo, sizeof(salida_turno_transferencia_vehiculo));
                break;
            case CODIGO_INFORMACION_DOMINIO:
                printf("El cliente pidió información de un dominio.\n");
                int numero_patente;
                recibir(fd_cliente, &numero_patente, sizeof(numero_patente));

                char salida_dominio [LONGITUD_CADENA];
                informacion_dominio(numero_patente, salida_dominio);

                enviar(fd_cliente, &salida_dominio, sizeof(salida_dominio));
                break;
            case CODIGO_TERMINAR:
                printf("El cliente decidió terminar la conexión.\n");
                mantener_conexion = 0;
                break;
            default:
                printf("El cliente pidió un operación que no existe.\n");
                break;
        }
    }

    printf("Conexión con el cliente cerrada.\n");

    cerrar(fd_cliente);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    // inicializar lista de dominios
    inicializar_lista_dominios();

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
    direccion_socket.sin_port = htons(PUERTO_B);
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
