struct tabla {
	int valores [11];
	int gano;
	int puntaje [11];
};

struct dados {
	int numeros [5]; 
};

struct jugada {
	int dados[5];
	int anotacion;
};

program GENERALA {
	version VERSGENERALA {
		int anotarse()=1;
		int empezar()=2;
		int jugadores()=3;
		int permiso(int)=4;
		struct jugada jugadaanterior()=5;
		struct tabla resultadosparciales(int)=6;
		struct dados jugar()=7;
		int anotarresultados(struct tabla)=8;
		int tachar(struct tabla)=9;
		int salir(int)=10;
		int tirar(int)=11;
		struct tabla terminar(int)=12;
		int finalizarjugadores(int)=13;
	}=1;
}=0x20900005;
		
