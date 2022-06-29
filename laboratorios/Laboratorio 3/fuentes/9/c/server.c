/*
	- Laboratorio N°3 Primer Cuatrimestre 2020
	- Ejercicio N°9c - Sockets sin Conexión - Server
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

struct mensaje {
    char so [MAX_DATOS];
    char fecha [MAX_DATOS];
};

void Consultar_so(struct mensaje * result, int socket_fd) {
	struct utsname uts;

	printf("Un cliente quiere saber el sistema operativo de esta máquina.\n");

	if (uname(&uts) == -1) {
		perror("uname() error");
		result->fecha[0] = '\0';
	}
	else strcpy(result->so, uts.sysname);
}

void Fecha_hora(struct mensaje * result, int socket_fd){
	printf("Un cliente quiere saber la hora local en esta máquina.\n");

	time_t tiempo_local;
	struct tm * tm_info;

	tiempo_local = time(NULL);
	tm_info = localtime(&tiempo_local);

	strftime(result->fecha, MAX_DATOS-1, "%a %b %d %H:%M:%S", tm_info);
}

int main(int argc, char *argv[])
{
	// crear socket
	int socket_fd;
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror("Socket");
		exit(errno);
	}

	// información necesaria para bind
	struct sockaddr_in direccion_servidor;
	direccion_servidor.sin_family = AF_INET;
	direccion_servidor.sin_port = htons(PUERTO);
	direccion_servidor.sin_addr.s_addr = INADDR_ANY;
	bzero(&(direccion_servidor.sin_zero), 8);

	// obtener nombre para el socket
	if (bind(socket_fd, (struct sockaddr *) &direccion_servidor, sizeof(struct sockaddr)) == -1) {
		perror("Bind");
		exit(errno);
	}

    socklen_t sl = sizeof(struct sockaddr_in);
    struct mensaje mensaje;
    char m [MAX_DATOS];

	while (1) {
        if(recvfrom(socket_fd, m, sizeof(char)*MAX_DATOS, MSG_WAITALL, 
                  (struct sockaddr *) &direccion_servidor, &sl) == -1) {
            perror("recvfrom");
            exit(errno);
        }

        printf("Recibí petición del cliente.\n");

        Consultar_so(&mensaje, socket_fd);
        Fecha_hora(&mensaje, socket_fd);
        if(sendto(socket_fd, &mensaje, sizeof(mensaje), MSG_CONFIRM, 
                  (struct sockaddr *) &direccion_servidor, sl) == -1) {
            perror("sendto");
            exit(errno);
        }
	}

	return EXIT_SUCCESS;
}
