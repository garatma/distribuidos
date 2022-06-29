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

int main(int argc, char *argv[])
{
	// crear socket
	int fd_socket;
	if ((fd_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
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
		exit(errno);
	}

	// permitir que el socket escuche conexiones
	if (listen(fd_socket, BACKLOG) == -1 ) {
		perror("Listen");
		exit(errno);
	}

	// datos del cliente
	int size_cliente = sizeof(struct sockaddr_in);
	int fd_cliente;
	struct sockaddr_in direccion_cliente;

	while (1) {
		// aceptar conexión de un cliente
		if ((fd_cliente = accept( fd_socket, (struct sockaddr *) &direccion_cliente, &size_cliente)) == -1) { 
			perror("Accept");
			continue;
		}

		// procesar conexión
		printf("Se recibió una conexión desde %s\n", inet_ntoa(direccion_cliente.sin_addr));
		int pid;
		if (!(pid = fork())) {
			// hijo
			// el 14 representa la longitud de "Hello, world!\n" (sin el \0)
			if (send(fd_cliente, "Hello, world!\n", 14, 0) == -1) {
				perror("Send");
				close(fd_cliente);
				exit(errno);
			}
			// si no hubo error, terminar el proceso
			close(fd_cliente);
			exit(EXIT_SUCCESS);
		}
		else if (pid == -1) perror("Fork");

		// cerrar la conexión
		close(fd_cliente);
	}

	return EXIT_SUCCESS;
}
