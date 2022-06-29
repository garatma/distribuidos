/*
	-Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	-Ejercicio 5a-minioperaciones con cola de mensajes
	-Cerdá Gianni Lucas - Garat Manuel
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>

//Definimos la estructura para el intercambio de mensajes

typedef struct{
	long type; //Request =1 , Reply =2 
	int opcion; // Suma = 1 Resta =2, Multiplicacion =3 , Division =4, Dec-->Bin = 5 , Bin -> Hex =6 
	int cant_op ; // Cantidad de operandos ingresados
	int operandos[4];
	char resultado[30];
} message;

#define TAM sizeof(message) - sizeof(long)
#define Request 1
#define Reply 2

//------------------------------ Operaciones------------------------------------

// Verifica si el numero que recibo es un número binario

int EsBinario (int n){
	int d;
	while(n!=0){
		d=n%10;
		if(d>1)
			return 0;
		else
			n=n/10;
	}
return 1;
}

// SUMA

int suma (int *ops,int n){
    
	int i , sum ;
	sum=0;
	for(i=0;i<n;i++)
		sum+=ops[i];
	return sum;
}

// RESTA

int resta (int *ops,int n){
	int i ,res;
	res = ops[0];
	for(i=1;i<n;i++)
		res-=ops[i];
	return res;
}

// MULTIPLICACION

int multiplicacion(int *ops){
	int mult;
	mult=ops[0]*ops[1];
	return mult; 
}

// DIVISION

float division (int *ops){
	float div;
	div=(float)ops[0]/(float)ops[1];
	return div;
}

// CONVERSION DEC->BIN  Y BIN->HEX (Dado una base origen y una base destino convierte el numero pasado por parametro)

int conversion (int n, int baseorigen,int basedestino){
	int res, i,r;
	res =0; 
	i=1;
	while(n!=0){
		r = n % basedestino;
	        n = n / basedestino;
		res+=r*i;	
		i*=baseorigen;
	}
	return res;
}


//-----------------CLIENTE----------------------------------------------------------------

void cliente (){
	
	// Inicialización de variables
	key_t clave;
	int continuar,opcion, cola;
	message msg;
	struct timeval timeini,timefin; // para el calculo del tiempo
	long diftime ;
	int numbase ; // para cambio de base
	int n; // suma ,resta, mul y div
	int i,arg;
	// Creo cola de mensajes
	clave =ftok("/etc",'c');
	cola = msgget(clave,0);

	continuar = 1;

	while (continuar){
		printf(" \nSeleccione el servicio aritmetico que desea ejecutar:\n");
		printf("\nPresione 1 para Sumar\n");
		printf("Presione 2 para Restar\n");
		printf("Presione 3 para Multiplicar\n");
		printf("Presione 4 para Dividir\n");
		printf("Presione 5 para convertir de Decimal a Binario\n");
		printf("Presione 6 para convertir de Binario a Hexadecimal\n");
		printf("Presione 7 para Salir\n");

		printf("\nOpcion Seleccionada:");
		
		if(scanf("%d",&opcion)<=0 || opcion>7 || opcion<1){
			printf("\nNo ha seleccionado un servicio disponible\n");
			printf("Terminando ejecución.... \n");
			exit(1);
		}



		msg.type=Request;
		msg.opcion=opcion;

		//-----------------OPCIONES-------------------------------------------------
		switch(opcion){
			
			//-------------------SUMA-------------------------------------------	
			
			case(1): //Sumar
			//Consulto  la cantidad de operando que desea sumar
				printf("\n-------------------- SUMA--------------------------- \n");
				printf("\nIngrese la cantidad de operandos (entre 2 y 4): ");
				if (scanf("%d",&n)<=0  || n<2 || n>4){
					printf("\nCantidad de operandos no valido, La cantidad de operandos debe ser entre 2 y 4\n");
					printf("Terminando ejecución.... \n");
					exit(1);
				}
				else{
					//Defino la estructura 
					msg.cant_op = n;
					printf("\nIngrese operandos:\n");
					for (i=0; i<n ; i++){
						printf("Operando %d: ",i+1);
						if(scanf("%d",&arg)<=0 ){
                            printf("\nOperando no válido para la suma\n");
                            printf("Terminando ejecución.... \n");
							exit(1);
                        }
					msg.operandos[i] = arg;
					}
                    
					//Calculo el tiempo en microsegundos
					gettimeofday(&timeini,NULL);
					
					//Envio el mensaje al servidor para obtener el resultado
					msgsnd(cola,(struct msgbuf*)&msg,TAM,0);
					
					//Leo el mensaje del servidor
					msgrcv(cola,(struct msgbuf*)&msg,TAM,Reply,0);
			
					gettimeofday(&timefin,NULL);	
					
					// Imprimo la suma 
					printf("\nEl resultado de la suma es: %s\n",msg.resultado);
				    diftime = (timefin.tv_sec - timeini.tv_sec)*1000000 + (timefin.tv_usec - timeini.tv_usec);
				    printf("El tiempo de ejecucion en microsegundos fue de: %ld us\n", diftime);
                }
			break;
			
			//-------------------RESTA-------------------------------------------	
			
			case(2): //Restar
				//Consulto  la cantidad de operando que desea restar
				printf("\n-------------------- RESTA--------------------------- \n");
				printf("\nIngrese la cantidad de operandos (entre 2 y 4)");
				if (scanf("%d",&n)<=0  || n<2 || n>4){
				    printf("\nCantidad de operandos no valido, La cantidad de operandos debe ser entre 2 y 4\n");
				    printf("Terminando ejecución.... \n");
					exit(1);
				}
				else{
					//Defino la estructura 
					msg.cant_op = n;
					printf("\nIngrese operandos\n");
					for (i=0; i<n ; i++){
						printf("Operando %d: ",i+1);
						if(scanf("%d",&arg)<=0 ){
	                        printf("\nOperando no válido para la resta\n");
	                        printf("Terminando ejecución.... \n");
							exit(1);
                        }
					msg.operandos[i] = arg;
					}
					//Calculo el tiempo en microsegundos
					gettimeofday(&timeini,NULL);
					
					//Envio el mensaje al servidor para obtener el resultado
					msgsnd(cola,(struct msgbuf*)&msg,TAM,0);
					
					//Envio el mensaje al servidor para obtener el resultado
					msgrcv(cola,(struct msgbuf*)&msg,TAM,Reply,0);
					
					gettimeofday(&timefin,NULL);	
					
					// Imprimo la resta 
					printf("\nEl resultado de la resta es: %s\n",msg.resultado);
			
				
				diftime = (timefin.tv_sec - timeini.tv_sec)*1000000 + (timefin.tv_usec - timeini.tv_usec);
				printf("El tiempo de ejecucion en microsegundos fue de: %ld us\n", diftime);
               	} //Fin del else 
			break;	

			//-------------------MULTIPLICACION-------------------------------------------	
	
			case(3): //Multiplicar
				//Consulto  la cantidad de operando que desea multiplicar
				printf("\n-------------------- MULTIPLICACION--------------------------- \n");
				printf("\nIngrese dos operandos\n");
				for (i=0; i<2 ; i++){
					printf("Operando %d: ",i+1);
					if(scanf("%d",&arg)<=0 ){
                        printf("\nOperando no válido para la multiplicacion\n");
                        printf("Terminando ejecución.... \n");
						exit(1);
                    }
                msg.operandos[i]= arg;
				}
				//Defino la estructura del mensaje
				msg.cant_op=2;
				//Calculo el tiempo en microsegundos
				gettimeofday(&timeini,NULL);
				
				//Envio el mensaje al servidor para obtener el resultado
				msgsnd(cola,(struct msgbuf*)&msg,TAM,0);
				
				//Envio el mensaje al servidor para obtener el resultado
				msgrcv(cola,(struct msgbuf*)&msg,TAM,Reply,0);
				
				gettimeofday(&timefin,NULL);	
					
				// Imprimo la multiplicacion 
				printf("\nEl resultado de la multiplicacion es: %s\n",msg.resultado);	
				diftime = (timefin.tv_sec - timeini.tv_sec)*1000000 + (timefin.tv_usec - timeini.tv_usec);
				printf("El tiempo de ejecucion en microsegundos fue de: %ld us\n", diftime);
			break;
		
			//-------------------DIVISION-------------------------------------------	
	
			case(4): //Division
				//Consulto  la cantidad de operando que desea dividir
				printf("\n-------------------- DIVISION--------------------------- \n");
				printf("\nIngrese dos operandos\n");
				for (i=0; i<2 ; i++){
					printf("Operando %d: ",i+1);
				    if(scanf("%d",&arg)<=0 ){
                        printf("\nOperando no válido para la división\n");
                        printf("Terminando ejecución.... \n");
						exit(1);
                    }
				msg.operandos[i] = arg;
				}
				
				if (msg.operandos[1]==0)
				printf("\nNo se puede dividir por 0 \n");
				else{
				//Defino la estructura del mensaje
				msg.cant_op=2;
				//Calculo el tiempo en microsegundos
				gettimeofday(&timeini,NULL);
				
				//Envio el mensaje al servidor para obtener el resultado
				msgsnd(cola,(struct msgbuf*)&msg,TAM,0);
				
				//Envio el mensaje al servidor para obtener el resultado
				msgrcv(cola,(struct msgbuf*)&msg,TAM,Reply,0);
				
				gettimeofday(&timefin,NULL);	
					
				// Imprimo la multiplicacion 
				printf("\nEl resultado de la multiplicacion es: %s\n",msg.resultado);	
				diftime = (timefin.tv_sec - timeini.tv_sec)*1000000 + (timefin.tv_usec - timeini.tv_usec);
				printf("El tiempo de ejecucion en microsegundos fue de: %ld us\n", diftime);
			}
			break;
			default:
				printf("\nOpcion incorrecta\n");
		
			//-------------------DECIMAL A BINARIO-------------------------------------------	
			
			case(5):
				// Se lee el valor a convertir
				printf("\n-------------------- DECIMAL A BINARIO--------------------------- \n");
				printf("\nIngrese el numero en decimal para convertirlo a binario: ");
				if(scanf("%d",&numbase)<=0 ) {
					printf("\nNo ha ingresado un numero\n");
					printf("Terminando ejecución.... \n");
					exit(1);
				}
                else {
				    //Defino la estructura del mensaje 
				    msg.cant_op=1;
				    msg.operandos[0]= numbase;
				    
				    //Calculo el tiempo en microsegundos
				    gettimeofday(&timeini,NULL);
				    
				    // Envio el mensaje al servidor para que realice el calculo
				    msgsnd(cola,(struct msgbuf*)&msg,TAM,0);
				    
				    // Reply del servidor
				    msgrcv(cola,(struct msgbuf*)&msg,TAM,Reply,0);
				    
				    gettimeofday(&timefin,NULL);
				    
				    printf("\nEl numero %d en binario es: %s\n",numbase,msg.resultado);	
				    diftime = (timefin.tv_sec - timeini.tv_sec)*1000000 + (timefin.tv_usec - timeini.tv_usec);
				    printf("El tiempo de ejecucion en microsegundos fue de: %ld us\n", diftime);
                }
			break;
			
			//-------------------BINARIO A HEXADECIMAL-------------------------------------------	
			
			case(6):
				// Se lee el valor a convertir
				
				printf("\n-------------------- BINARIO A HEXADECIMAL--------------------------- \n");
				printf("\nIngrese el numero en binario para convertirlo a hexadecimal: ");
				if(scanf("%d",&numbase)<=0 ) {
                    printf("\nNo ha ingresado un numero\n");
                    printf("Terminando ejecución.... \n");
					exit(1);
				}
                else{
				    if (!EsBinario(numbase))
					    printf("\nEl numero ingresado no es un numero binario\n");
				    else{	
					    //Defino la estructura del mensaje 
					    msg.cant_op=1;
					    msg.operandos[0]= numbase;
						    
					    //Calculo el tiempo en microsegundos
					    gettimeofday(&timeini,NULL);
					    
					    // Envio el mensaje al servidor para que realice el calculo
					    msgsnd(cola,(struct msgbuf*)&msg,TAM,0);
					    
					    // Reply del servidor
					    msgrcv(cola,(struct msgbuf*)&msg,TAM,Reply,0);
					    
					    gettimeofday(&timefin,NULL);
					    
					    printf("\nEl numero %d en hexadecimal es: %s\n",numbase,msg.resultado);	
				        diftime = (timefin.tv_sec - timeini.tv_sec)*1000000 + (timefin.tv_usec - timeini.tv_usec);
				        printf("El tiempo de ejecucion en microsegundos fue de: %ld us\n", diftime);
                    }
                }
			break;

			case(7): 	
				printf("\nSaliendo.... \n");
                exit(1);
		}
		// 
		opcion=0;
		fflush(stdin);
		printf("\n\n¿ Desea realizar otra operación?\n\n Presione 1 si desea continuar\n Presione 0 para terminar la ejecución  \n");
		printf("\nUsted Selecciono: ");
		if(scanf("%d",&continuar)<=0  || continuar>1 || continuar<0){
			printf("\nError de entrada , debe seleccionar 0 o 1\n");
			printf("Terminando ejecución.... \n");
			exit(1);
		}
		if(continuar==0)
		printf("\nTerminando ejecución....\n");
	}
}// Fin cliente

//----------------------------SERVIDOR--------------------------------------------

void servidor (){
	key_t clave;
	int cola;
	message msg;
	clave =ftok("/etc",'c');
	cola = msgget(clave,0);
	
	while(1){
		msgrcv(cola,(struct msgbuf*)&msg,TAM,Request,0);
		msg.type = Reply;

		switch(msg.opcion){
			case 1: if (sprintf(msg.resultado,"%d",suma(msg.operandos,msg.cant_op))<0){
						printf ("Error en el servidor\n");
						exit(EXIT_FAILURE);
					}
					else  break;
			case 2: if (sprintf(msg.resultado,"%d",resta(msg.operandos,msg.cant_op))<0){
						printf ("Error en el servidor\n");
						exit(EXIT_FAILURE);
					}
					else  break;
			case 3: if (sprintf(msg.resultado,"%d",multiplicacion(msg.operandos))<0){
						printf ("Error en el servidor\n");
						exit(EXIT_FAILURE);
					}
					else  break;
			case 4: if (sprintf(msg.resultado,"%.2f",division(msg.operandos))<0){
						printf ("Error en el servidor\n");
						exit(EXIT_FAILURE);
					}
					else  break;
			case 5: if (sprintf(msg.resultado,"%d",conversion(msg.operandos[0],10,2))<0){
						printf ("Error en el servidor\n");
						exit(EXIT_FAILURE);
					}
					else  break;
			case 6: if (sprintf(msg.resultado,"%x",conversion(msg.operandos[0],2,10))<0){
						printf ("Error en el servidor\n");
						exit(EXIT_FAILURE);
					}
					else  break;
		}
		msgsnd(cola,(struct msgbuf*)&msg,TAM,0);
	}													
}

int main(){
	key_t clave;
	int oldcola,cola;
	
	pid_t pid;
	clave = ftok("/etc",'c');
	
	//Elimino la cola anterior, si existe
	oldcola=msgget(clave,0);

	if(oldcola!=-1)
		if (msgctl(oldcola,IPC_RMID,NULL)!=-1)
			printf("Cola anterior eliminada\n");

    cola=msgget(clave, IPC_CREAT|0666);
	
	pid = fork();
	
	if (pid==0)
		servidor();
	else {
		if (pid==-1) printf("Error al crear el proceso\n");
		else cliente();
	}
	return 0;
}
		
					
						
						
						
						
	
	
		
		
				
			
				
				
				
				
				
					
						









		

