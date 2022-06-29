all : client server
.PHONY : all

client: mini_operaciones_client.o mini_operaciones_clnt.o mini_operaciones_xdr.o
	echo "Compilando cliente..."
	gcc -Wall -lnsl -o client mini_operaciones_client.o mini_operaciones_clnt.o mini_operaciones_xdr.o `pkg-config --libs --cflags libtirpc`

server: mini_operaciones_server.o mini_operaciones_svc.o mini_operaciones_xdr.o
	echo "Compilando server..."
	gcc -Wall -lnsl -o server mini_operaciones_server.o mini_operaciones_svc.o mini_operaciones_xdr.o -lm `pkg-config --libs --cflags libtirpc`

mini_operaciones_server.o : mini_operaciones_server.c mini_operaciones.h
	gcc -Wall -c mini_operaciones_server.c `pkg-config --libs --cflags libtirpc`

mini_operaciones_client.o : mini_operaciones_client.c mini_operaciones.h
	gcc -Wall -c mini_operaciones_client.c `pkg-config --libs --cflags libtirpc`

mini_operaciones_svc.o : mini_operaciones_svc.c
	gcc -Wall -c mini_operaciones_svc.c `pkg-config --libs --cflags libtirpc`

mini_operaciones_clnt.o : mini_operaciones_clnt.c
	gcc -Wall -c mini_operaciones_clnt.c `pkg-config --libs --cflags libtirpc`

mini_operaciones_xdr.o : mini_operaciones_xdr.c
	gcc -Wall -c mini_operaciones_xdr.c `pkg-config --libs --cflags libtirpc`

mini_operaciones_xdr.c mini_operaciones_clnt.c mini_operaciones_svc.c mini_operaciones.h : mini_operaciones.x
	@echo "Generando protocolo RPC..."
	rpcgen mini_operaciones.x

clean :
	@echo "Limpiando archivos..."
	$(RM) *.o *_xdr.c client server mini_operaciones_clnt.c mini_operaciones_svc.c salida mini_operaciones.h
