all: clienteA clienteB servidorA servidorB

servidorA: servidorA.c operacionesA.o
	gcc -g -Wall -o servidorA servidorA.c operacionesA.o

servidorB: servidorB.c operacionesB.o servidorB.c
	gcc -Wall -o servidorB servidorB.c operacionesB.o

clienteA: clienteA.c operacionesA.o
	gcc -g -Wall -o clienteA clienteA.c operacionesA.o

clienteB: clienteB.c operacionesB.o
	gcc -g -Wall -o clienteB clienteB.c operacionesA.o

operacionesA.o: operacionesA.h constantes.h operacionesA.c
	gcc -Wall -c operacionesA.c

operacionesB.o: operacionesB.h constantes.h operacionesB.c
	gcc -Wall -c operacionesB.c

clean:
	$(RM) servidorA servidorB *.o clienteA clienteB
