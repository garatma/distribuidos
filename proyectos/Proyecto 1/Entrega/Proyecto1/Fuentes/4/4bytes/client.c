/*
	- Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	- Ejercicio N°4 - Sockets 4B - Cliente
	- Cerdá Gianni Lucas - Garat Manuel
*/

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PUERTO 15000 	// puerto en el que sirve el servidor.
#define MAX_DATOS 4		// tamaño máximo de datos leídos desde el servidor.

void cerrar(int fd) {
	if (close(fd) == -1) {
		perror("Close");
		exit(errno);
	}
}

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

	// nombre de host/dirección de IP del servidor
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

	// declaramos los structs para almacenar lo que retorna gettimeofday().
	struct timeval tiempo_antes, tiempo_despues;

	// marcamos el tiempo justo antes de establecer la conexión.
	if (gettimeofday(&tiempo_antes,NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

	// recibir datos del servidor
	if ((size_entrada = recv(socket_fd, buffer, MAX_DATOS, 0)) == -1) {
		perror("Receive");
		exit(errno);
	}

	// marcamos el tiempo justo después de haber establecido la conexión.
	if (gettimeofday(&tiempo_despues, NULL) == -1) {
		perror("Gettimeofday");
		exit(errno);
	}

	printf("Mensaje recibido:\n");

	// imprimir lo leído
	for (int i = 0; i < size_entrada; ++i) {
		putchar(buffer[i]);
	}

	printf("\n");

	printf("La solicitud llevó %ld μs.\n", 
			1000000*(tiempo_despues.tv_sec - tiempo_antes.tv_sec ) + 
			( tiempo_despues.tv_usec - tiempo_antes.tv_usec )
	);

	// cerrar socket
	cerrar(socket_fd);

	return EXIT_SUCCESS;
}
