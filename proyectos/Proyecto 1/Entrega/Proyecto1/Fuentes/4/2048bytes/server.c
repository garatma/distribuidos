/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°4 - Sockets 2048B - Servidor
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

#define PUERTO 15000 	// puerto en el que se atenderán los pedidos de los clientes
#define BACKLOG 10 		// tamaño de la cola de conexiones recibidas
#define MAX_DATOS 2048	// tamaño máximo de datos enviados desde el servidor

int fd_socket;

void cerrar(int fd) {
	if (close(fd) == -1) {
		perror("Close");
		exit(errno);
	}
}

int main(int argc, char *argv[])
{
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
	direccion_socket.sin_port = htons(PUERTO);
	direccion_socket.sin_addr.s_addr = INADDR_ANY;
	bzero(&(direccion_socket.sin_zero), 8);

	// obtener nombre para el socket
	if (bind(fd_socket, (struct sockaddr *) &direccion_socket, sizeof(struct sockaddr)) == -1) {
		perror("Bind");
		cerrar(fd_socket);
		exit(errno);
	}

	// permitir que el socket escuche conexiones
	if (listen(fd_socket, BACKLOG) == -1 ) {
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
		int pid;
		if (!(pid = fork())) {
			// hijo
			// paquete de 2048 bytes
			char datos[MAX_DATOS];
			for (int i = 0; i < MAX_DATOS; ++i)
				datos[i] = 1;
			if (send(fd_cliente, datos, MAX_DATOS, 0) == -1) {
				perror("Send");
				cerrar(fd_cliente);
				exit(errno);
			}
			// si no hubo error, terminar el proceso
			cerrar(fd_cliente);
			exit(EXIT_SUCCESS);
		}
		else if (pid == -1) perror("Fork");

		// cerrar la conexión
		cerrar(fd_cliente);
	}

	return EXIT_SUCCESS;
}
