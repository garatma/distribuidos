all: rpcgen xdr.o svc.o servidor clnt.o gui.o logica.o conexion.o generala

generala: main.c logica.o gui.o conexion.o generala_clnt.o generala_xdr.o
	gcc -Wall -Wpedantic -Wextra -o generala main.c logica.o gui.o conexion.o generala_clnt.o generala_xdr.o `pkg-config --libs --cflags gtk+-3.0 libtirpc`

conexion.o: conexion.c gui.h logica.h conexion.h generala.h
	gcc -Wall -Wpedantic -Wextra -c conexion.c `pkg-config --libs --cflags libtirpc`

logica.o: logica.c gui.h logica.h conexion.h
	gcc -Wall -Wpedantic -Wextra -c logica.c `pkg-config --libs --cflags gtk+-3.0 libtirpc`

gui.o: gui.c gui.h logica.h
	gcc -Wall -Wpedantic -Wextra -c gui.c `pkg-config --libs --cflags gtk+-3.0 libtirpc`

clnt.o: generala_clnt.c generala.h
	gcc -Wall -Wpedantic -c generala_clnt.c `pkg-config --libs --cflags libtirpc`

servidor: servidor.c generala.h generala_svc.o generala_xdr.o
	gcc -Wall -Wpedantic -o servidor servidor.c generala_svc.o generala_xdr.o `pkg-config --libs --cflags libtirpc`

svc.o: generala_svc.c generala.h
	gcc -Wall -Wpedantic -c generala_svc.c `pkg-config --libs --cflags libtirpc`

xdr.o: generala_xdr.c generala.h
	gcc -Wall -Wpedantic -Wextra -c generala_xdr.c `pkg-config --libs --cflags libtirpc`

rpcgen: generala.x
	rpcgen generala.x

clean:
	rm -f *.o
	rm -f generala servidor
	rm -f generala_clnt.c generala_svc.c generala_xdr.c
	rm -f generala.h
