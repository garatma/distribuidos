/*
	- Laboratorio N°3 Primer Cuatrimestre 2020
	- Ejercicio N°9b - Sockets con Conexión - Server
	- Cerdá Gianni Lucas - Garat Manuel
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/utsname.h>
#include <string.h>
#include <time.h>


#define PUERTO 15000 	// puerto en el que se atenderán los pedidos de los clientes
#define BACKLOG 10 		// tamaño de la cola de conexiones recibidas
#define MAX_DATOS 101	// tamaño máximo de datos leídos desde el servidor.

void Consultar_so(int fd_cliente){
	static char * result;
	struct utsname uts;

	result = (char *) malloc(sizeof(char)*MAX_DATOS);

	printf("Un cliente quiere saber el sistema operativo de esta máquina.\n");

	if (uname(&uts) == -1) {
		perror("uname() error");
		result[0] = '\0';
	}
	else strcpy(result, uts.sysname);
	if (send(fd_cliente,result , MAX_DATOS, MSG_NOSIGNAL) == -1) {
		perror("Send");
		close(fd_cliente);
		exit(errno);
	}
}

void Fecha_hora(int fd_cliente){
	static char * result;
    result = (char *) malloc(sizeof(char)*MAX_DATOS);

	printf("Un cliente quiere saber la hora local en esta máquina.\n");

	time_t tiempo_local;
	struct tm * tm_info;

	tiempo_local = time(NULL);
	tm_info = localtime(&tiempo_local);

	strftime(result, MAX_DATOS-1, "%a %b %d %H:%M:%S", tm_info);
	if (send(fd_cliente,result , MAX_DATOS, MSG_NOSIGNAL) == -1) {
		perror("Send");
		close(fd_cliente);
		exit(errno);
	}
}
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
			Consultar_so(fd_cliente);
			Fecha_hora(fd_cliente);

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
