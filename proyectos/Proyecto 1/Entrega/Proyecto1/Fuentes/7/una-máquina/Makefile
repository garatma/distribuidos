all : client server
.PHONY : all

client: offset_client.o offset_clnt.o offset_xdr.o
	echo "Compilando cliente..."
	gcc -Wall -lnsl -o client offset_client.o offset_clnt.o offset_xdr.o

server: offset_server.o offset_svc.o offset_xdr.o
	echo "Compilando server..."
	gcc -Wall -lnsl -o server offset_server.o offset_svc.o offset_xdr.o

offset_server.o : offset_server.c offset.h
	gcc -Wall -c offset_server.c

offset_client.o : offset_client.c offset.h
	gcc -Wall -c offset_client.c

offset_svc.o : offset_svc.c
	gcc -Wall -c offset_svc.c

offset_clnt.o : offset_clnt.c
	gcc -Wall -c offset_clnt.c

offset_xdr.o : offset_xdr.c
	gcc -Wall -c offset_xdr.c

offset_xdr.c offset_clnt.c offset_svc.c offset.h : offset.x
	@echo "Generando protocolo RPC..."
	rpcgen offset.x

clean :
	@echo "Limpiando archivos..."
	$(RM) *.o *_xdr.c client server offset_clnt.c offset_svc.c salida offset.h
