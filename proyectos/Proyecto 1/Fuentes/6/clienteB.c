/*
    - Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
    - Ejercicio N°6 - Cliente de la agencia B: pide servicios a la agencia A.
    - Cerdá Gianni Lucas - Garat Manuel
*/

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "operacionesA.h"

int fd_socket = 0;

void cerrar(int fd) {
    if (close(fd) == -1) {
        perror("Close");
        exit(errno);
    }
}

void recibir(int fd, void * mensaje, int size) {
    if (recv(fd, mensaje, size, 0) == -1) {
        perror("Receive");
        exit(EXIT_FAILURE);
    }
}

void enviar(int fd, void * mensaje, int size) {
    if (send(fd, mensaje, size, MSG_NOSIGNAL) == -1) {
        perror("Send");
        exit(EXIT_FAILURE);
    }
}

void mostrar_mensaje_operaciones() {
	printf("%i: Turno licencia matrimonio\n%i: Información partida de nacimiento\n%i: Turno inscripción bebé recién nacido\n%i: Terminar la conexión\n%i: Mostrar este mensaje\n",
            CODIGO_TURNO_LICENCIA_MATRIMONIO, 
            CODIGO_INFORMACION_PARTIDA_NACIMIENTO, 
            CODIGO_TURNO_INSCRIPCION_RECIEN_NACIDO,
            CODIGO_TERMINAR,
            CODIGO_MOSTRAR_MENSAJE
    );
}

void turno_licencia(int fd_socket, int operacion) {
    // enviar operación.
    enviar(fd_socket, &operacion, sizeof(operacion));

    // enviar operando.
    char mensaje;
    enviar(fd_socket, &mensaje, sizeof(mensaje));

    // recibir resultado.
    int resultado;
    recibir(fd_socket, &resultado, sizeof(resultado));

    if (resultado)
        printf("El servidor pudo reservar el turno.\n");
    else
        printf("El servidor no pudo reservar el turno.\n");
}

void partida_nacimiento(int fd_socket, int operacion) {
    // enviar operación.
    enviar(fd_socket, &operacion, sizeof(operacion));

    // enviar operando.
    int numero_partida, ret, entrada_incorrecta = 1;
    while (entrada_incorrecta) {
        printf("Ingrese el número de partida a consultar: ");
        ret = scanf("%i", &numero_partida);
        if(ret == EOF) {
            perror("Scanf");
            exit(errno);
        }
        else if (ret == 0) {
            printf("No se ingresó un número\n");
            exit(EXIT_FAILURE);
        }
        else
            entrada_incorrecta = 0;
    }
    enviar(fd_socket, &numero_partida, sizeof(numero_partida));

    // recibir resultado.
    char partida[LONGITUD_CADENA];
    recibir(fd_socket, &partida, sizeof(partida));

    printf("El servidor retorno la siguiente partida de nacimiento: \"%s\"\n", partida);
}

void turno_inscripcion(int fd_socket, int operacion) {
    // enviar operación.
    enviar(fd_socket, &operacion, sizeof(operacion));

    // enviar operando.
    char mensaje;
    enviar(fd_socket, &mensaje, sizeof(mensaje));

    // recibir resultado.
    int resultado;
    recibir(fd_socket, &resultado, sizeof(resultado));

    if (resultado)
        printf("El servidor pudo reservar el turno.\n");
    else
        printf("El servidor no pudo reservar el turno.\n");
}

void terminar(int fd_socket, int operacion) {
    // enviar operación.
    enviar(fd_socket, &operacion, sizeof(operacion));
}

void salir_con_gracia(int sig) {
    if (fd_socket != 0) {
        terminar(fd_socket, CODIGO_TERMINAR);
        cerrar(fd_socket);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Debe especificar el host del servidor.\n");
        exit(EXIT_FAILURE);
    }

    if ((fd_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(errno);
    }

    struct hostent * host_servidor;

    // nombre de host/dirección de IP del servidor
    if ((host_servidor = gethostbyname(argv[1])) == NULL) {
        perror("Get server host name");
        exit(h_errno);
    }

    struct sockaddr_in direccion_servidor;

    // datos del servidor
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(PUERTO_A);
    direccion_servidor.sin_addr = * (struct in_addr *) host_servidor->h_addr_list[0];
    bzero(&(direccion_servidor.sin_zero), 8);
    
    // conexión con el servidor
    if (connect(fd_socket, (struct sockaddr *) &direccion_servidor, sizeof(struct sockaddr)) == -1) {
        perror("Connect");
        exit(errno);
    }

    struct sigaction signal;
    signal.sa_handler = &salir_con_gracia;
    sigaction(SIGINT, &signal, NULL);

    int seguir_operando = 1;
	mostrar_mensaje_operaciones();
	while (seguir_operando) {
		// si la operación ingresada no es un número, el default es 0.
		int operacion;
		printf("Ingrese una operación: ");
		int ret = scanf("%i", &operacion);
		if (ret != 1) {
			if (ret == -1) perror("Scanf");
			else printf("La operación ingresada es incorrecta.\n");
            continue;
		}

		switch (operacion) {
			case CODIGO_TURNO_LICENCIA_MATRIMONIO:
				turno_licencia(fd_socket, operacion);
				break;
			case CODIGO_INFORMACION_PARTIDA_NACIMIENTO:
				partida_nacimiento(fd_socket, operacion);
				break;
			case CODIGO_TURNO_INSCRIPCION_RECIEN_NACIDO:
			    turno_inscripcion(fd_socket, operacion);
				break;
            case CODIGO_MOSTRAR_MENSAJE:
                mostrar_mensaje_operaciones();
                break;
            case CODIGO_TERMINAR:
                terminar(fd_socket, operacion);
                seguir_operando = 0;
                break;
			default:
				printf("Operación incorrecta.\n");
				break;
		}
	}

	cerrar(fd_socket);

    return EXIT_SUCCESS;
}
