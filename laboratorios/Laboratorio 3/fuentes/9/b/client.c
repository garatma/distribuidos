/*
	- Laboratorio N°3 Primer Cuatrimestre 2020
	- Ejercicio N°9b - Sockets con Conexión - Cliente
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PUERTO 15000 	// puerto en el que sirve el servidor.
#define MAX_DATOS 101		// tamaño máximo de datos leídos desde el servidor.

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Debe especificar el host del servidor.\n");
		exit(EXIT_FAILURE);
	}

	int socket_fd;
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(errno);
	}
	
	struct hostent * host_servidor;

	// nombre de host del servidor
	if ((host_servidor = gethostbyname(argv[1])) == NULL) {
		perror("Get server host name");
		exit(h_errno);
	}

	struct sockaddr_in direccion_servidor;

	// datos del servidor
	direccion_servidor.sin_family = AF_INET;
	direccion_servidor.sin_port = htons(PUERTO);
	direccion_servidor.sin_addr = * (struct in_addr *) host_servidor->h_addr_list[0];
	bzero(&(direccion_servidor.sin_zero), 8);
	
	// conexión con el servidor
	if (connect(socket_fd, (struct sockaddr *) &direccion_servidor, sizeof(struct sockaddr)) == -1) {
		perror("Connect");
		exit(errno);
	}

	char buffer [MAX_DATOS];
	int size_entrada;
	
	

	// sistema operativo del servidor
	if ((size_entrada = recv(socket_fd, buffer, MAX_DATOS, 0)) == -1) {
		perror("Receive");
		exit(errno);
	}
		
	printf("El nombre del sistema operativo del servidor es: %s\n",buffer);
	
	// fecha y hora del servidor 
	if ((size_entrada = recv(socket_fd, buffer, MAX_DATOS, 0)) == -1) {
		perror("Receive");
		exit(errno);
	}
	
	printf("La fecha y hora del servidor es: %s\n",buffer);
	
	// cerrar socket
	close(socket_fd);

	return EXIT_SUCCESS;
}
