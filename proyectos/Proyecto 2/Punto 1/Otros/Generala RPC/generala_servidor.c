
/* Proyecto 2 - Sistemas distribuidos 2020
 * Alumnos:
 * 	Gianni Lucas Cerdá
 * 	Garat Manuel
 * GeneralaRPC - Server 
 */

#include "generala.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int clientes=0; //Cantidad de usuarios
int id=0;// Identificación del usuario
int punteroSiguiente=-1; 
struct tabla *tablas; // Tablas con el puntaje y anotaciones de cada cliente
int jugando=-2; //Clientes jugando a la generala
struct jugada Anterior ; 
int * terminaron;
int terminados=0;
int jugadas;
int calls;

// Se registra un jugador y se le asigna una identificacion

int *anotarse_1_svc(void *argp, struct svc_req *rqstp)
{
	static int result;
	if(jugando==-2){
		printf("Se ha registrado un jugador\n");
		result=id;
		clientes++;
		id++;
	}
	else result=-1;

	return &result;
}

//Una vez registrado los jugadores se inicializa la tabla con los valores para cada jugador(Cada jugador tendrá cómo máximo 11 jugadas)

int *empezar_1_svc(void *argp, struct svc_req *rqstp) {
    static int result;
    int i;
    int j;
    punteroSiguiente=-1;
    if(clientes>1 && jugando==-2){
        punteroSiguiente=0;//Comienza el primer jugador registrado
        result=1;
        jugando=-1; //No hay nadie jugando
        tablas=(struct tabla *)malloc(clientes*sizeof(struct tabla));
        i=0;
        j=0;
        while(i<clientes){
            while(j<11){
                tablas[i].valores[j]=0;
                j++;
            }
            tablas[i].gano=0;
            tablas[i].puntaje=0;
            i++;
            j=0;
        }
        terminaron=(int *)malloc(clientes*sizeof(int));
        jugadas=-1;
        
        Anterior.dados[0]=0;         
        Anterior.dados[1]=0;         
        Anterior.dados[2]=0;         
        Anterior.dados[3]=0;         
        Anterior.dados[4]=0;         
        Anterior.anotacion=11; 
    
        i=0;
        while(i<clientes){
            terminaron[i]=0;
            i++;
        }
    }
    else{
        if(jugando>-2){
            result=1; //Comienzo del juego
            printf("Comenzando el juego\n");
        }
        else
        {
            result=0; //Se espera por más jugadores
            printf("Esperando por nuevos jugadores\n");
        }
    }

    return &result;
}

//Comienzo del juego, se mantiene la cantidad de jugadores dentro del juego	

int *jugadores_1_svc(void *argp, struct svc_req *rqstp){
    static int result;
    result =-1;
    if (jugando!=-2){
        result=clientes; //Comienza el juego, no es posible anotar más jugadores al juego
    }
    return &result;
}

//EL jugador actual indica que ha finalizado su turno y que puede continuar. El servidor actualiza el siguiente jugador que puede jugar y cuando éste lo invoque podrá darle el permiso.

int *permiso_1_svc(int *argp, struct svc_req *rqstp){
    static int result;
    result=0;
    if(jugando==-1){
        //NO hay jugadores jugando 
        if (punteroSiguiente==*argp){
            //El que invoca es quien debe jugar
            result=1;
            calls=0;
            jugando=*argp; //cliente jugando 
            jugadas++;
        }
        else{
            calls++;
            if (calls>1000){
                //Si despues de esta cantidad de llamadas realizadas, se asume que el cliente se ha caido
                punteroSiguiente=(punteroSiguiente+1)%clientes;
                calls=0;
            }
            result=0;
        }
    }  
    else{
        if(jugando==*argp){
            //El que juega es quien llama a la función
            punteroSiguiente=(punteroSiguiente+1)%clientes;
            result=1;
            jugando=-1; // No hay nadie jugando
        }
    }
    return &result;
}

//Jugada anterior del participante 

struct jugada *jugadaanterior_1_svc(void *argp, struct svc_req *rqstp){
    
    return &Anterior;
}

//Resultados parciales del participante

char **resultadosparciales_1_svc(void *argp, struct svc_req *rqstp){
    static char *result;
    int c; // variable para recorrer los clientes/jugador;
    int p; // variable para recorrer categorias;
    int s; // variable para recorrer strings;
    
    s=0;
    result = (char *)malloc(clientes*11*sizeof(char));
    for (c=0;c<clientes;c++){
        for(p=0;p<11;p++){
            result[s]=(char)(65+tablas[c].valores[p]);
            s++;
        }
    }
    return &result;
}

// simula la tirada de los 5 dados por parte del jugador, estos valores son aleatorios tomando dichos valores entre 1 y 6 el cual corresponde a cada numero del dado.

struct dados *jugar_1_svc(void *argp, struct svc_req *rqstp){
    static struct dados result;
   
    srand(time(NULL));
   
    result.numeros[0]=(rand()%6)+1;
    result.numeros[1]=(rand()%6)+1;
    result.numeros[2]=(rand()%6)+1;
    result.numeros[3]=(rand()%6)+1;
    result.numeros[4]=(rand()%6)+1;

    Anterior.dados[0]=result.numeros[0];
    Anterior.dados[1]=result.numeros[1];
    Anterior.dados[2]=result.numeros[2];
    Anterior.dados[3]=result.numeros[3];
    Anterior.dados[4]=result.numeros[4];

    return &result;
}

// Se anota los puntos que el jugador actual obtuvo en sus tres tiros , estos puntos se agregan a la tabla asociada a dicho jugador.
int *anotarresultados_1_svc(struct tabla *argp, struct svc_req *rqstp){
    static int result;
    int encontre;
    int aux;
    int i;

    i=-1;
    encontre=0;
    aux=0;
    while(!encontre){
        i++;
        aux=argp->valores[i];
        encontre=aux!=0;
    }
    // Si encontre pregunto si es un valor para un numero u otra categoría
    if (i<6){
        //Es un numero
        if(aux==-5)
            aux=0;
        tablas[punteroSiguiente].valores[i]=aux; //Puntero siguiente se refiere a que todavia es el jugador actual
    }
    else{
        switch(i){
            case 6:
                tablas[punteroSiguiente].valores[i]=20; //Escalera
                break;
            case 7:
                tablas[punteroSiguiente].valores[i]=30; //Full
                break;
            case 8:    
                tablas[punteroSiguiente].valores[i]=40; //Poker
                break;
            case 9:    
                tablas[punteroSiguiente].valores[i]=50; //Generala
                break;
            case 10:    
                tablas[punteroSiguiente].valores[i]=100;//Generala Doble
                break;
        }
    }
    // guardo la anotacion de la categoria o numero
    Anterior.anotacion=i;
    i=0;

    result=1;
    return &result;
}

//Anula en la tabla del jugador actual la categoria que éste quiera tachar. A la categoría correspondiente le asigna el valor cero.

int *tachar_1_svc(struct tabla *argp, struct svc_req *rqstp){
    static int result;
    int encontre;
    int aux;
    int i;

    i=-1;
    encontre=0;
    aux=0;
    while(!encontre){
        i++;
        aux=argp->valores[i];
        encontre=aux!=0;
    }
    tablas[punteroSiguiente].valores[i]=-1; 
    Anterior.anotacion=i*(-1);

    return &result;
}

// Salir de la jugada

int *salir_1_svc(int *argp, struct svc_req *rqstp){
    static int result;
    result=0;
    return &result;
}

// Tirada

int *tirar_1_svc(int *argp, struct svc_req *rqstp){
    static int result;
    result=(rand()%6)+1;
    Anterior.dados[*argp]=result;
    return &result;
}

// Al jugar sus 11 turnos, el jugador indica al servidor que ha finalizado, y que se encuentra a la espera de la finalización de sus contrincantes.

struct tabla *terminar_1_svc(int *argp, struct svc_req *rqstp){
    static struct tabla result;
    int j; //variable para recorrer los jugadores
    int v; //variable para recorrer cada una de las categorias (1 a GeneralaDoble)
    int puntos;
    int puntosmaximos;
    int sumactual;

    //inicializo variables
    puntos=0;
    puntosmaximos=0;
    sumactual=0;
    j=0;  // cantidad de jugadores
    v=0; // valor de cada categoria registrada en la tabla

    //inicializo los valores de cada categoria de la tabla
    while(v<11){
        result.valores[v]=0;
        v++;
    }
    //inicalizo puntaje y ganador 
    result.gano=0;
    result.puntaje=0;

    v=0;
    // recorro por cada cliente su tabla asociada y la almaceno su puntaje total
    while(j<clientes){
        while(v<11){
            if(tablas[j].valores[v]<0)
                tablas[j].valores[v]=0;
            sumactual+=tablas[j].valores[v];
            if(j==*argp)
                result.valores[v]=tablas[j].valores[v];
            v++;
        }
        if (j==*argp)
            puntos=sumactual;
        if(sumactual>puntosmaximos)
            puntosmaximos=sumactual;
        sumactual=0;
        j++;
        v=0;
    }
    result.puntaje=puntos;
    result.gano=(puntos==puntosmaximos);
    return &result;
}

// Una vez que todos los participantes han finalizado el juego, el servidor informa los resultados obtenidos (puntajes correspondientes y ganador del juego).

int *finalizarjugadores_1_svc(int *argp, struct svc_req *rqstp){
    static int result;
    result=0;
    if(terminaron[*argp]==0){
        terminaron[*argp]=1;
        terminados++;
    }
    if(terminados==clientes)
        result=1;
        
    return &result;
}


                
