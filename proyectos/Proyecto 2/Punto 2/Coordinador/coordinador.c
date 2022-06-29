#include <stdio.h>
#include <stdlib.h>
#include "consultas.h"
#include <string.h>

#define N 100

// Funcion para verificar si un archivo/carpeta es valido
// Recibe un tipo (0=carpeta,1=archivo), un nombre y la ubicacion del nodo actual
// Retorna 0=falso, 1=verdadero y 2=tipoInvalido
int esValido(char tipo, char* Nombre, char* Ubicacion)
{
	if (tipo == '0')
	{
		struct archivo aux = *buscarCarpeta(Nombre);
		if (strcmp(aux.permiso,"N")==0) return 0;
		else return 1;	
	}
	else
		if (tipo == '1')
		{
			struct archivo aux = *buscarArchivo(Nombre,Ubicacion);
			if (strcmp(aux.permiso,"N")==0) return 0;
			else return 1;
		}
		else
			return 2;
}


// Funcion encargada del LS
// Recibe una ubicacion (carpeta actual)
// Retorna un string con todos los elementos en la carpeta
char* funcionListar(char* Direccion)
{	
	struct listado list = *funcionLS(Direccion);
	
	char temp[N];
	
	// Revisar porque es necesario esto, pero si no lo pones no anda
	strcpy(temp,"");
	
	int i;
	struct archivo archivoaux;
	char* nombreaux;
	
	for (i=0;i<list.cantidad;i++)
	{	
		archivoaux = list.elementos[i];
		nombreaux = archivoaux.nombre;
		strcat(temp, nombreaux);
		strcat(temp, ",");
	}


	int size = strlen(temp);
	char *toRet;
	toRet = malloc(sizeof(temp)*size);
	strcpy(toRet,temp);
	
	return (toRet);	
}

// Funcion para retornar la IP en la que se encuentra un archivo
// Recibe el nombre de un archivo
// Retorna un string con el IP del nodo que tiene el archivo
char* obtenerIP(char* Archivo, char* Ubicacion)
{	
	struct archivo aux = *buscarArchivo(Archivo, Ubicacion);
	char* toRet = aux.ip;
	
	return (toRet);	
}


int carpetaVacia(char* Nombre)
{
	struct listado aux = *funcionLS(Nombre);
	if (aux.cantidad == 0) return 1;
	else return 0;	
}

int insert(char tipo, char* Nombre,char* IP, char* Ubicacion)
{
	if (tipo == '0')
	{
		char aux[40]="raiz/";
		strcat(aux,Nombre);
		insertar(Nombre,"-","raiz","R","0","0",aux);
		return 1;
	}
	else
		if (tipo == '1')
		{
			char aux[40]="raiz/";
			if ( strcmp(Ubicacion,aux) !=0 ) strcat(aux,Ubicacion);
			insertar(Nombre,IP,Ubicacion,"W","0","1",aux);
			return 1;
		}
		else
			return 0;
}

int delete(char tipo, char* Nombre,char* IP, char* Ubicacion)
{
	if (tipo == '0')
	{
		eliminar(Nombre,"-","raiz","R");
		return 1;
	}
	else
		if (tipo == '1')
		{
			eliminar(Nombre,IP,Ubicacion,"W");
			return 1;
		}
		else
			return 0;
}

int modificarIP(char* NuevaIP, char* Archivo, char* Carpeta)

{

	struct archivo aux = *buscarArchivo(Archivo,Carpeta);

	

	updateip(Archivo,NuevaIP,aux.direccion,aux.permiso,"0","1",aux.ruta);

	return 1;

}



int isFile(char* Nombre, char* Ubicacion)

{

	struct archivo aux = *buscarArchivo(Nombre,Ubicacion);

	

	if (strcmp(aux.permiso,"N") ==0) return 0;

	else return 1;

}

int modificarDirectorio(char* Nombre, char* CarpetaVieja, char* CarpetaNueva)
{
	char dir[40]="raiz/";

	if (strcmp(CarpetaNueva,"raiz") != 0) strcat(dir,CarpetaNueva);

	strcat(dir,"/");
	strcat(dir,Nombre);

	struct archivo aux = *buscarArchivo(Nombre,CarpetaVieja);
	updatePosicion(Nombre,aux.ip,CarpetaNueva,aux.permiso,"0","1",dir);

	return 1;
}


int main()
{
	
	
	return 1;

}
