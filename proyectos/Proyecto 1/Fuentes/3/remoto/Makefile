all : client server
.PHONY : all

client: suma_arreglos_client.o suma_arreglos_clnt.o suma_arreglos_xdr.o
	echo "Compilando cliente..."
	gcc -Wall -lnsl -o client suma_arreglos_client.o suma_arreglos_clnt.o suma_arreglos_xdr.o `pkg-config --libs --cflags libtirpc`

server: suma_arreglos_server.o suma_arreglos_svc.o suma_arreglos_xdr.o
	echo "Compilando server..."
	gcc -Wall -lnsl -o server suma_arreglos_server.o suma_arreglos_svc.o suma_arreglos_xdr.o `pkg-config --libs --cflags libtirpc`

suma_arreglos_server.o : suma_arreglos_server.c suma_arreglos.h
	gcc -Wall -c suma_arreglos_server.c `pkg-config --libs --cflags libtirpc`

suma_arreglos_client.o : suma_arreglos_client.c suma_arreglos.h
	gcc -Wall -c suma_arreglos_client.c `pkg-config --libs --cflags libtirpc`

suma_arreglos_svc.o : suma_arreglos_svc.c
	gcc -Wall -c suma_arreglos_svc.c `pkg-config --libs --cflags libtirpc`

suma_arreglos_clnt.o : suma_arreglos_clnt.c
	gcc -Wall -c suma_arreglos_clnt.c `pkg-config --libs --cflags libtirpc`

suma_arreglos_xdr.o : suma_arreglos_xdr.c
	gcc -Wall -c suma_arreglos_xdr.c `pkg-config --libs --cflags libtirpc`

suma_arreglos_xdr.c suma_arreglos_clnt.c suma_arreglos_svc.c suma_arreglos.h : suma_arreglos.x
	@echo "Generando protocolo RPC..."
	rpcgen suma_arreglos.x

clean :
	@echo "Limpiando archivos..."
	$(RM) *.o *_xdr.c client server suma_arreglos_clnt.c suma_arreglos_svc.c salida suma_arreglos.h
