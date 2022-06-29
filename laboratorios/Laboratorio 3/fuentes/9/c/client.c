/*
	- Laboratorio N°3 Primer Cuatrimestre 2020
	- Ejercicio N°9c - Sockets sin Conexión - Cliente
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

struct mensaje {
    char so [MAX_DATOS];
    char fecha [MAX_DATOS];
};

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Debe especificar el host del servidor.\n");
		exit(EXIT_FAILURE);
	}

	int socket_fd;
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
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
	
    socklen_t sl = sizeof(struct sockaddr_in);
    struct mensaje mensaje;

    if(sendto(socket_fd, "ping", sizeof(char)*MAX_DATOS, MSG_CONFIRM, 
              (struct sockaddr *) &direccion_servidor, sl) == -1) 
    {
        perror("sendto");
        exit(errno);
    }

    if(recvfrom(socket_fd, &mensaje, sizeof(mensaje), MSG_WAITALL, 
              (struct sockaddr *) &direccion_servidor, &sl) == -1) 
    {
        perror("recvfrom");
        exit(errno);
    }

	printf("El nombre del sistema operativo del servidor es: %s\n",mensaje.so);

	printf("La fecha y hora del servidor es: %s\n",mensaje.fecha);

	// cerrar socket
	close(socket_fd);

	return EXIT_SUCCESS;
}
