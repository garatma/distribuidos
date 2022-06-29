/* Proyecto 2 - Sistemas distribuidos 2020
 * Alumnos:
 * 	Gianni Lucas Cerdá
 * 	Garat Manuel
 * GeneralaRPC - Cliente 
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "generala.h"

// Cliente 
CLIENT *clnt;

//Anotarse
char *anotarse_1_arg;
int  *result_1;

//Empezar
char *empezar_1_arg;
int  *result_2;

//Jugadores
char *jugadores_1_arg;
int  *result_3;

//Permiso
int  permiso_1_arg;
int  *result_4;

//Jugada Anterior
char *jugadaanterior_1_arg;
struct jugada  *result_5;

//Resultados Parciales
char *resultadosparciales_1_arg;
char **result_6;

//Jugar
char *jugar_1_arg;
struct dados *result_7;

//Anotar Resultados
struct tabla  anotarresultados_1_arg;
int  *result_8;

//Tachar
struct tabla  tachar_1_arg;
int  *result_9;

//Salir
int  salir_1_arg;
int  *result_10;

//Tirar
int  tirar_1_arg;
int  *result_11;

// Terminar
int  terminar_1_arg;
struct tabla  *result_12;

//Finalizar Jugadores
int finalizarjugadores_1_arg;
int *result_13;

int opcion; //opcion menu
int i;
int turnos; 
int nuevo[5]; // nuevo tiro del jugador
int puntos; // puntos anotados por cada categoria en la tabla
int tiros; // cantidad de dados que elige el jugador para el proximo tiro
int anoto[11]; //para anotar puntaje en cada categoria
int *jugando;
int indice; //para mostrar resultados parciales y finales
int total;
int flag=0;

// Se Dibuja los dados para mostrar al jugador su tiro 

void dibujar(int* numbers){
  
	char* arreglo[7];
	arreglo[0] = "╔═══════╗";
	arreglo[1] = "║       ║";
	arreglo[2] = "║●      ║";
	arreglo[3] = "║      ●║";
	arreglo[4] = "║   ●   ║";
	arreglo[5] = "║●     ●║";
	arreglo[6] = "╚═══════╝";

	char* arregloDados[6][3];
	arregloDados[0][0] = arreglo[1];
	arregloDados[0][1] = arreglo[4];
	arregloDados[0][2] = arreglo[1];
	arregloDados[1][0] = arreglo[2];
	arregloDados[1][1] = arreglo[1];
	arregloDados[1][2] = arreglo[3];
	arregloDados[2][0] = arreglo[2];
	arregloDados[2][1] = arreglo[4];
	arregloDados[2][2] = arreglo[3];
	arregloDados[3][0] = arreglo[5];
	arregloDados[3][1] = arreglo[1];
	arregloDados[3][2] = arreglo[5];
	arregloDados[4][0] = arreglo[5];
	arregloDados[4][1] = arreglo[4];
	arregloDados[4][2] = arreglo[5];
	arregloDados[5][0] = arreglo[5];
	arregloDados[5][1] = arreglo[5];
	arregloDados[5][2] = arreglo[5];

	printf("\033[%d;%dm %s %s %s %s %s \n",1,37,arreglo[0],arreglo[0],arreglo[0],arreglo[0],arreglo[0]);

	for(int i=0; i<3; i++)
	{
		for(int j=0; j< 5;j++)
		{
			printf("\033[%d;%dm %s", 1, 37,arregloDados[numbers[j]-1][i]);
		}
		printf("\n");
	}

	printf("\033[%d;%dm %s %s %s %s %s \n ",1,37,arreglo[6],arreglo[6],arreglo[6],arreglo[6],arreglo[6]);
}

// Funcion auxiliar para la busqueda de un dado en el tiro del jugador
int Buscardado (int num, int arreglo[], int length){
	int resultado = 0;
	for(int i=0;i<length;i++)
		if(arreglo[i]=num)
			resultado = 1;
	return resultado;
}

// Escalera en la jugada del jugador
int Escalera(){
	int resultado=0;
	if(Buscardado(2,result_7->numeros,5) && Buscardado(3,result_7->numeros,5) && Buscardado(4,result_7->numeros,5) && Buscardado(5,result_7->numeros,5))
		if(Buscardado(1,result_7->numeros,5) || Buscardado(6,result_7->numeros,6))
			resultado =1;
	return resultado;
}

//Full en la jugada del jugador
int Full(){
	int resultado=0;
	int i=0;
	int repetidos[6];
	for(i=0;i<6;i++)
		repetidos[i]=0;
	i=0;
	while(i<5){
		repetidos[(result_7->numeros[i])-1]+=1;
		i++;
	}
	resultado=Buscardado(2,repetidos,6) && Buscardado(3,repetidos,6);
	return resultado;
}

// Poker en la jugada del jugador
int Poker(){
	int poker =0;
	int resultado=0;
	int i=0;
	int repetidos[6];
	for(i=0;i<6;i++)
		repetidos[i]=0;
	i=0;
	while(i<5){
		repetidos[(result_7->numeros[i])-1]+=1;
		i++;
	}
	i=0;
	while(i<6 && poker==0){
		poker=(repetidos[i]>=4);
		i++;
	}
	return poker;
}

// Generala en la jugada del jugador
int Generala(){
	int resultado = result_7->numeros[0] == result_7->numeros[1] && result_7->numeros[1] == result_7->numeros[2] && result_7->numeros[2] == result_7->numeros[3] && result_7->numeros[3] == result_7->numeros[4]; 
	return resultado;
}

// Tachar jugada 
void Cargar_Valores_Tachar(int op){
	int i=0;
	while(i<11){
		if(i==op)
			tachar_1_arg.valores[i]=1;
		else
			tachar_1_arg.valores[i]=0;
	i++;
	}
}

// Anotar Jugada
void Cargar_Valores_Anotar(int op){
	int i=0;
	while(i<11){
		if(i==op)
			anotarresultados_1_arg.valores[i]=1;
		else
			anotarresultados_1_arg.valores[i]=0;
	i++;
	}
}

//Volver a tirar dados por parte del jugador
int VolveraTirarDados(){
	for(int j=0;j<5;j++)
		nuevo[j]=-1;
	
	int control=1;

	//El jugador elige que dados desea tirar para el proximo tiro
	while(control){
		printf("¿Cuantos dados desea volver a tirar?\n ");
		scanf("%i",&tiros);
		if(tiros>=1 && tiros<=5)
			control=0;
		else{
			printf("Debe elegir una cantidad de dados válida");
			fflush(stdin);
		}
	}
	
	printf("Los dados se numeran de izquierda a derecha de 1 a 5\n");

	i=0;
	while(i<tiros){
		control=1;
		int op;
		while(control){
			switch(i){
					case 0:
						printf("¿Que dado desea tirar primero? ");
					break;
					case 1:
						printf("¿Que dado desea tirar segundo? ");
					break;
					case 2:
						printf("¿Que dado desea tirar tercero? ");
					break;
					case 3:
						printf("¿Que dado desea tirar cuarto? ");
					break;
					case 4:
						printf("¿Que dado desea tirar quinto? ");
					break;
					default:
					break;
			}
			scanf("%i",&op);
			if(op>0 && op<6)
				control=0;
			else{
				printf("Ingrese una opción válida\n0");
				fflush(stdin);
			}
		}
		opcion=op-1;
		result_11=tirar_1(&opcion,clnt);
		if(result_11==(int*) NULL)
			clnt_perror(clnt,"Call Failed--> VolverTirarDados");
		nuevo[opcion]=*result_11;
		i++;
	}
	i=0;
	while(i<5){
		if(nuevo[i]!=-1)
			result_7->numeros[i]=nuevo[i];
	i++;		
	}
	printf("El resultado de tirar los dados seleecionados nuevamente es: \n\n");
	
	//Dibujo de nuevo los dados en pantalla para que el usuario lo puede visualizar
	dibujar(result_7->numeros);
}

// Se tacha la categoria seleccionada en base a los resultados parciales de cada jugador
int TacharCategoria(){
	int control=1;
	while(control){
		printf("Elija que categoría desea tachar: \n");
		printf("                                 1 - Uno \n");
		printf("                                 2 - Dos \n");
		printf("                                 3 - Tres \n");
		printf("                                 4 - Cuatro \n");
		printf("                                 5 - Cinco \n");
		printf("                                 6 - Seis \n");
		printf("                                 7 - Escalera \n");
		printf("                                 8 - Full \n");
		printf("                                 9 - Poker \n");
		printf("                                10 - Generala \n");
		printf("                                11 - Generala Doble \n");
	
		printf("Ingrese categoría a tachar: ");
		scanf("%i",&opcion);
		if(opcion>0 && opcion<12)
			control=0;
		else {
			printf("Categoría incorrecta. Ingresa una categoría válida ");
			fflush(stdin);
		}
	}

	printf("\n\n● Usted va a tachar la categoría %i \n",opcion);
	
	if (anoto[opcion-1]!=-1){
		Cargar_Valores_Tachar(opcion-1);
		result_9=tachar_1(&tachar_1_arg, clnt);

		if(result_9 == (int *) NULL){
			clnt_perror(clnt,"Call Error-->TacharCategoria");
		}
		anoto[opcion-1]=-1;
		printf("● Ha tachado la categoria %i\n",&opcion);
	}
	else
	{
		printf("● La categoria elegida ya se encuentra tachada, selecciona una distinta \n\n");
		TacharCategoria();
	}	
}

// Se tacha la categoria seleccionada en base a los resultados parciales de cada jugador
int AnotarCategoria(){
	int control=1;
	while(control){
		printf("Elija que categoría desea anotar: \n");
		printf("                                 1 - Uno \n");
		printf("                                 2 - Dos \n");
		printf("                                 3 - Tres \n");
		printf("                                 4 - Cuatro \n");
		printf("                                 5 - Cinco \n");
		printf("                                 6 - Seis \n");
		printf("                                 7 - Escalera \n");
		printf("                                 8 - Full \n");
		printf("                                 9 - Poker \n");
		printf("                                10 - Generala \n");
		printf("                                11 - Generala Doble \n");
	
		printf("Ingrese categoría a anotar: ");
		scanf("%i",&opcion);
		if(opcion>0 && opcion<12)
			control=0;
		else {
			printf("Categoría incorrecta. Ingresa una categoría válida ");
			fflush(stdin);
		}
	}

	printf("\n\n● Usted va a anotar la categoría %i \n",opcion);

	if (anoto[opcion-1]==0){
		anoto[opcion-1]=1;

		if(opcion<7){
			i=0;
			puntos=0;
			while(i<5){
				if(result_7->numeros[i]==opcion)
					puntos+=opcion;
			i++;
			}
			printf("● Ha anotado %i puntos al %i \n\n",puntos,opcion);
		
			if (puntos==0) puntos=-5; //0 puntos
			anotarresultados_1_arg.valores[opcion-1]=puntos;
		
			result_8=anotarresultados_1(&anotarresultados_1_arg,clnt);
			if (result_8==(int*) NULL)
				clnt_perror(clnt,"Call Failed-->AnotarCategoria");
		}
		else{
			switch (opcion){
				case 7:
					if (Escalera())
					{
						Cargar_Valores_Anotar(opcion-1);
						result_8=anotarresultados_1(&anotarresultados_1_arg,clnt);
						if (result_8==(int*) NULL)
							clnt_perror(clnt,"call failed-->AnotarCategoria (Escalera Anotar)");
						printf("● Ha anotado Escalera \n\n");
					}
					else
					{
						Cargar_Valores_Tachar(opcion-1);
						result_9=tachar_1(&tachar_1_arg,clnt);
						if (result_9==(int*) NULL)
							clnt_perror(clnt,"call failed-->AnotarCategoria (Escalera Tachar)");
						anoto[opcion-1]=-1;
						printf("● No tiene Escalera . Ha tachado Escalera \n\n");
					}
				break;
				case 8:
					if (Full())
					{
						Cargar_Valores_Anotar(opcion-1);
						result_8=anotarresultados_1(&anotarresultados_1_arg,clnt);
						if (result_8==(int*) NULL)
							clnt_perror(clnt,"call failed-->AnotarCategoria (Full Anotar)");
						printf("● Ha anotado Full \n\n");
					}
					else
					{
						Cargar_Valores_Tachar(opcion-1);
						result_9=tachar_1(&tachar_1_arg,clnt);
						if (result_9==(int*) NULL)
							clnt_perror(clnt,"call failed-->AnotarCategoria (Full Tachar)");
						anoto[opcion-1]=-1;
						printf("● No tiene Full . Ha tachado Full \n\n");
					}		
				break;
				case 9:
					if (Poker())
					{
						Cargar_Valores_Anotar(opcion-1);
						result_8=anotarresultados_1(&anotarresultados_1_arg,clnt);
						if (result_8==(int*) NULL)
							clnt_perror(clnt,"call failed-->AnotarCategoria (Poker Anotar)");
						printf("● Ha anotado Poker \n\n");
					}
					else
					{
						Cargar_Valores_Tachar(opcion-1);
						result_9=tachar_1(&tachar_1_arg,clnt);
						if (result_9==(int*) NULL)
							clnt_perror(clnt,"call failed-->AnotarCategoria (Poker Tachar)");
						anoto[opcion-1]=-1;
						printf("● No tiene Poker . Ha tachado Poker \n\n");
					}		
				break;
				case 10:
					if (Generala())
					{
						Cargar_Valores_Anotar(opcion-1);
						result_8=anotarresultados_1(&anotarresultados_1_arg,clnt);
						if (result_8==(int*) NULL)
							clnt_perror(clnt,"call failed-->AnotarCategoria (Generala Anotar)");
						printf("● Ha anotado Generala \n\n");
					}
					else
					{
						Cargar_Valores_Tachar(opcion-1);
						result_9=tachar_1(&tachar_1_arg,clnt);
						if (result_9==(int*) NULL)
							clnt_perror(clnt,"call failed-->AnotarCategoria (Generala Tachar)");
						anoto[opcion-1]=-1;
						printf("● No tiene Generala . Ha tachado Generala \n\n");
					}	
					
				break;
				case 11:
					if (Generala() && anoto[9]==1)
					{
						Cargar_Valores_Anotar(opcion-1);
						result_8=anotarresultados_1(&anotarresultados_1_arg,clnt);
						if (result_8==(int*) NULL)
							clnt_perror(clnt,"call failed-->AnotarCategoria (Generala Doble Anotar)");
						printf("● Ha anotado Generala Doble \n\n");
					}
					else
					{
						Cargar_Valores_Tachar(opcion-1);
						result_9=tachar_1(&tachar_1_arg,clnt);
						if (result_9==(int*) NULL)
							clnt_perror(clnt,"call failedAnotarCategoria (Generala Doble Tachar)");
						anoto[opcion-1]=-1;
						printf("● No tiene Generala Doble .Ha tachado Generala Doble \n\n");
					}	
				break;
				default:
					printf("AnotarResultadosParciales :Opcion inválida");
					exit(EXIT_FAILURE);
				break;
			}
		}
		anotarresultados_1_arg.valores[opcion-1]=0;
	}
	else {
		if (anoto[opcion-1]==1) printf("● Ya ha anotado esta categoría \n\n");
		else printf("● Ya ha tachado esta categoría \n\n");
	AnotarCategoria();
	return 0;
	}
}

//Mostrar los resultados parciales al jugador (tabla con categorias y los puntos correspondintes a cada una)
void MostrarResultadosParciales(){
	printf("┌───────────────────────────────┐\n");
	printf("│  Categoría     │ Resultado    │\n");
	printf("│───────────────────────────────│\n"); 
	result_6=resultadosparciales_1((void*)&resultadosparciales_1_arg,clnt);
	if (result_6 == (char **)NULL)
		clnt_perror(clnt,"Call Failed -->Resultados Parciales");
	int aux[total];
	indice=0;
	//Jugador que corresponde
	(*result_6)=(*result_6)+(*result_1*11);
	while(indice<11){
		aux[indice]=**result_6 - 65;
		(*result_6)++;
		switch (indice){
			case 0:
				printf("│ 1              │");
			break;
			case 1:
				printf("│ 2              │");
			break;
			case 2:
				printf("│ 3              │");
			break;
			case 3:
				printf("│ 4              │");
			break;
			case 4:
				printf("│ 5              │");
			break;
			case 5:
				printf("│ 6              │");
			break;
			case 6:
				printf("│ Escalera       │");
			break;
			case 7:
				printf("│ Full           │");
			break;
			case 8:
				printf("│ Poker          │");
			break;
			case 9:
				printf("│ Generala       │");
			break;
			case 10:
				printf("│ Generala Doble │");
			break;
		}
		if(aux[indice]>=0){
			if(aux[indice]<=9)
				printf("%i             │\n",aux[indice]);
			else
				printf("%i         |   │\n",aux[indice]);
		}
		else
			printf("X             │\n");

		indice++;
	}
	printf("└───────────────────────────────┘\n");
}

//Mostrar los resultados finales al jugador que corresponde
int MostrarResultadosFinales(int puntaje, int gano){
	result_6=resultadosparciales_1((void*)&resultadosparciales_1_arg, clnt);
	if (result_6==(char**)NULL)
		clnt_perror(clnt,"Call Failed-->MostrarResultadosFinales");

	printf("═══════════════════════════════════════════════════════════════════════════\n");
	printf("═══════════════════════════Resultados Parciales:═══════════════════════════\n");
	int aux[total];
	indice=0;
	if(*result_1==0){
		printf("Usted: \n");
		if (puntaje>=0 && gano>=0){
			printf("Su puntaje es %i \n",puntaje);
			if(gano){
				printf("Felicitaciones usted a ganado el juego :D \n");
			}
			else
			{
				printf("Ha perdido :C, pero la vida siempre da revancha 💪\n");
			}		
		}
	}
	else printf("Jugador 0: \n");

	while(indice>total){
		aux[indice]=**result_6 -65;
		(*result_6)++;
		if (indice!=0){
			if(indice%11 == 0){
				if(indice/11==*result_1){
					printf("______________________________________________________________________\n");
					printf("Usted: \n");
					if (puntaje>=0 && gano>=0){
						printf("Su puntaje es %i \n",puntaje);
						if(gano){
							printf("Felicitaciones usted a ganado el juego :D \n");
						}
						else
						{
							printf("Ha perdido :C, pero la vida siempre da revancha 💪\n");
						}		
					}
				}
				else{
					printf("______________________________________________________________________\n");
					printf("Jugador %i \n",indice/11);
				}
			}
		}

		switch (indice % 11){
			case 0:
				printf(" Uno            ");
				break;
			case 1:
				printf(" Dos             ");
				break;
			case 2:
				printf(" Tres            ");
				break;
			case 3:
				printf(" Cuatro         ");
				break;
			case 4:
				printf(" Cinco          ");
				break;
			case 5:
				printf(" Seis            ");
				break;
			case 6:
				printf(" Escalera        ");
				break;
			case 7:
				printf(" Full            ");
				break;
			case 8:
				printf(" Poker           ");
				break;
			case 9:
				printf(" Generala        ");
				break;
			case 10:
				printf(" Generala Doble  ");
				break;
		}
		if( aux[indice]>=0){
			printf("%i \n",aux[indice]);
		}
		else{
			printf("X \n");
		}
		indice++;
	}
	printf("═══════════════════════════════════════════════════════════════════════════\n");
}

// ═════════════════════════════════════════════MAIN════════════════════════════════════════════════

int main (int argc, char *argv[]){
	char *host;
	jugando=(int*)malloc(sizeof(int)); 
	*jugando=-1;
	opcion=0;
	i=0;
	turnos=0;
	nuevo[5]; 
	puntos=0;
	tiros=0;
	//inicializo arreglo anoto
	while(i<11){
		anoto[i]=0;
		i++;
	}
	i=0;
	//Si el usuario no pasa ningun argumento por pantalla
	if(argc < 1){
		printf("Usage : %s server\n",argv[0]);
		exit(1);
	}
	// Localhost
	char *auxiliar="localhost";
	if(argc==1)
		host=auxiliar;
	else
		host=argv[1];	

	printf("                      ▛▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▜                      \n");
	printf("███████████████████  	      GENERALA           ███████████████████\n");
	printf("███████████████████   Sistemas Distribuidos 2020 ███████████████████\n");
	printf("███████████████████         Cerdá - Garat        ███████████████████\n");
	printf("                      ▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟                      \n");

	//turnos=0;

	//Creo el cliente
	clnt=clnt_create(host,GENERALA,VERSGENERALA,"udp");
	if (clnt==NULL){
		clnt_pcreateerror(host);
		exit(EXIT_FAILURE);
	}

	// El cliente se anota en el servidor
	result_1= anotarse_1((void*)&anotarse_1_arg,clnt);
		if (result_1== (int*) NULL)
			clnt_perror(clnt,"Call Failed-->Anotarse");
	
	int control=1;
	while(control){
		printf("Usted es el jugador con el número %i.\n\n ╬ Elija una opción:\n 0 ─ Esperar a que otro cliente inicie el juego\n 1 ─ Iniciar Generala\n\n Opcion: ", *result_1);
		scanf("%i",&opcion);
		if (opcion==0 || opcion==1)
      		control=0;
    	else{
    	    printf("Ingrese una opción válida ( 0 ó 1)\n");
            fflush(stdin);
    	}
    }
if (opcion==1){
	result_2=empezar_1((void*)&empezar_1_arg, clnt);
	if (result_2==(int *)NULL){
		clnt_perror(clnt,"Call Failed-->Empezar");
	}
	if (*result_2==0)
	{
		printf(" No puede empezar el juego porque solo hay un jugador.Cuando otro jugador inicie, podra empezar a jugar\n");
	}	
}	
else
{
	printf(" Esperando que otro jugador pueda iniciar el juego...");
}

// Tengo que saber cuantos jugadores hay 
while(*jugando==-1){
	jugando=jugadores_1((void*)&jugadores_1_arg, clnt);
	if (jugando == (int*) NULL){
		clnt_perror(clnt,"Call Failed---> Jugando");
	}
}
total=*jugando*11;
printf("═══════════════════════════════════════════════════════════════════════════\n");
printf("Se ha anotado más jugadores.. Comenzando el juego\n\n");
//Comienza el juego
turnos=0;
while(turnos<11){
	result_4=permiso_1(result_1,clnt);
	if (result_4 == (int*) NULL){
		clnt_perror(clnt,"Call Failed---> Permiso1");
	}
	if (*result_4==1){
		turnos++;
		//Se muestra los jugadores anterior y los resultados parciales
		result_5=jugadaanterior_1((void*)&jugadaanterior_1_arg,clnt);
		if(result_5==(struct jugada *)NULL){
			clnt_perror(clnt,"Call Failed-->Jugada");
		}
		i=0;
		if (result_5->anotacion<11){
			printf("Ultima jugada del adversario: \n");
			dibujar(result_5->dados);
			printf("\n");
			switch(result_5->anotacion){
				case 0:
					printf("Anoto al UNO \n");
					break;
				case 1:
					printf("Anoto al DOS \n");
					break;
				case 2:
					printf("Anoto al TRES \n");
					break;
				case 3:
					printf("Anoto al CUATRO \n");
					break;
				case 4:
					printf("Anoto al CINCO \n");
					break;
				case 5:
					printf("Anoto al SEIS \n");
					break;
				case 6:
					printf("Anoto ESCALERA \n");
					break;
				case 7:
					printf("Anoto FULL \n");
					break;
				case 8:
					printf("Anoto POKER \n");
					break;
				case 9:
					printf("Anoto GENERALA \n");
					break;
				case 10:
					printf("Anoto GENERALA DOBLE \n");
					break;
				case -1:
					printf("Tacho al DOS \n");
					break;
				case -2:
					printf("Tacho al TRES \n");
					break;
				case -3:
					printf("Tacho al CUATRO \n");
					break;
				case -4:
					printf("Tacho al CINCO \n");
					break;
				case -5:
					printf("Tacho al SEIS \n");
					break;
				case -6:
					printf("Tacho ESCALERA \n");
					break;
				case -7:
					printf("Tacho FULL \n");
					break;
				case -8:
					printf("Tacho POKER \n");
					break;
				case -9:
					printf("Tacho GENERALA \n");
					break;
				case -10:
					printf("Tacho GENERALA DOBLE \n");
					break;
			}
		}
		else
		{
			printf("Usted empezara el juego!\n");
		}
		printf("\n═══════════════════════════════════════════════════════════════════════════\n");
		printf("\n");
		
		printf(" Turno: %i\n",turnos);
		MostrarResultadosParciales();
		printf("\n");
		printf("Es su turno. Se tiran los dados...\n\n");

		result_7=jugar_1((void*)&jugar_1_arg, clnt);
		if(result_7== (struct dados *)NULL)
			clnt_perror(clnt,"Call Failed--> Jugar");
		
		printf("Han salido los dados: \n\n");
		
		dibujar(result_7->numeros);
		
		printf("\n");
		control=1;
		while(control){
			printf(" Elija una opcion:\n\n 0 - Volver a tirar\n 1 - Anotar\n 2 - Tachar\n 3 - Resultados Parciales\n 4 - Ver Dados\n Opcion: \n");
			scanf("%i",&opcion);
			if (opcion==0 || opcion==1 || opcion==3 || opcion==4){
				if(opcion==3)
					MostrarResultadosFinales(-1,-1);
				else 
					if (opcion==4) dibujar(result_7->numeros);
					else control=0;
			}
			else
			{
				printf("Ingrese una opción válida\n");
				fflush(stdin);
			}
		}
		switch (opcion){
				case 0:
					VolveraTirarDados();
					control=1;
					while (control){
						printf(" Elija una opcion:\n\n 0 - Volver a tirar\n 1 - Anotar\n 2 - Tachar\n 3 - Resultados Parciales\n 4 - Ver Dados\n Opcion: \n");
						scanf("%i",&opcion);
						if (opcion==0 || opcion==1 || opcion==2 || opcion==3 || opcion==4){
							if (opcion==3)
								MostrarResultadosFinales(-1,-1);
							else
								if (opcion==4)
									dibujar(result_7->numeros);
								else
									control=0;
						}
						else
						{
				                        printf("Ingrese una opcion valida\n");
                        				fflush(stdin);
                				}
					}
					switch (opcion){
						case 0:
							VolveraTirarDados();
							control=1;
							while (control){
								printf(" Elija una opcion:\n 1-Anotar\n 2-Tachar \n 3-Resultados Parciales\n 4-Ver Dados\n Opcion: \n");
								scanf("%i",&opcion);
								if (opcion==1 || opcion==2 || opcion==3 || opcion==4){
									if (opcion==3)
										MostrarResultadosFinales(-1,-1);
									else
										if (opcion==4)
											dibujar(result_7->numeros);
										else
											control=0;
								}
								else
								{
                        						printf("Ingrese una opcion valida\n");
						                        fflush(stdin);
                						}
							}
							switch (opcion){
								case 1:
									AnotarCategoria();
									break;
								case 2:
									TacharCategoria();
									break;
							}
							break;
						case 1:
							AnotarCategoria();
							break;
						case 2:
							TacharCategoria();
							break;
					}
					break;
				case 1:
					AnotarCategoria();
					break;
				case 2:
					TacharCategoria();
					break;
			}
			// se tiraron los 3 tiros , indico que termino el turno
			result_4=permiso_1(result_1,clnt);
			if (result_4 == (int*) NULL){
				clnt_perror(clnt,"Call Failed---> Permiso2");
			}
			printf("»» Es el turno del otro jugador \n");
			printf("\n═══════════════════════════════════════════════════════════════════════════\n");
            printf("\n");
			flag=1;
	}
	else
		if(flag==0){
			printf("Otro jugador inicio primero\n");
			printf("\n═══════════════════════════════════════════════════════════════════════════\n");
            printf("\n");
			flag=1;

		}
}

//Finalizar 
result_13=(int*)malloc(sizeof(int));
*result_13=0;

while (*result_13==0)
{
	result_13=finalizarjugadores_1(result_1, clnt);
	if(result_13==(int*) NULL)
		clnt_perror(clnt,"Call Failed--> Finalizar");
}

printf("El juego ha terminado. \n");
printf("\n═══════════════════════════════════════════════════════════════════════════\n");
printf("\n");
result_12=terminar_1(result_1,clnt);
if(result_12== (struct tabla*) NULL){
	clnt_perror(clnt,"Call Failed--> Terminar");
}
printf("Resultados Finales: \n");
result_6= resultadosparciales_1((void*)&resultadosparciales_1_arg,clnt);
if(result_6 == (char**) NULL){
	clnt_perror(clnt,"Call Failed--> Resultados Parciales ");
}
MostrarResultadosFinales(result_12->puntaje,result_12->gano);
clnt_destroy(clnt);

//Reseteo los colores 
printf("\033[0m");
exit(0);
}

















