/* 
 * Ejercicio 5 - Laboratorio 2
 * Alumnos: Cerdá Gianni Lucas - Garat Manuel
 * 
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>

//Estructura del mensaje
typedef struct
{
    long type;
    char* string;
} message;
#define tam sizeof(message)-sizeof(long)

int main()
{
	struct timespec ini, fin;
	long timedif;
	message msg;
	int cola;
	key_t clave;
	
	msg.type = 1; //Tipo 1 = peticion, Tipo 2 = recepcion
	msg.string = "Creación y Escritura por cola de mensajes";

// Medimos el tiempo de creacion de la cola de mensajes

	clock_gettime(CLOCK_REALTIME, &ini);
	clave = ftok("/etc", 'c'); //convert a pathname and a project identifier to a System V IPC key
	cola = msgget(clave, IPC_CREAT|0666);
	clock_gettime(CLOCK_REALTIME, &fin);
	timedif= (fin.tv_sec - ini.tv_sec)*1000000000 + (fin.tv_nsec - ini.tv_nsec);
	printf("El tiempo de creación de la cola de mensajes es: %ld nanosegundos \n", timedif);

// Medimos el tiempo en la escritura de la cola de mensajes

	clock_gettime(CLOCK_REALTIME, &ini);
	msgsnd(cola, (struct msgbuf*)&msg, tam, 0);
	clock_gettime(CLOCK_REALTIME, &fin);
	timedif = (fin.tv_sec - ini.tv_sec)*1000000000 + (fin.tv_nsec - ini.tv_nsec);
	printf("El tiempo de escritura en la cola de mensajes es: %ld nanosegundos \n", timedif);
	msgctl(clave, IPC_RMID, 0);

	return 0;
}




			

	

