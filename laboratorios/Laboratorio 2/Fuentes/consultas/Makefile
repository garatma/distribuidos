all : client server
.PHONY : all

client: consultas_client.o consultas_clnt.o consultas_xdr.o
	echo "Compilando cliente..."
	gcc -Wall -lnsl -o client consultas_client.o consultas_clnt.o consultas_xdr.o

server: consultas_server.o consultas_svc.o consultas_xdr.o
	echo "Compilando server..."
	gcc -Wall -lnsl -o server consultas_server.o consultas_svc.o consultas_xdr.o

consultas_server.o : consultas_server.c consultas.h
	gcc -Wall -c consultas_server.c

consultas_client.o : consultas_client.c consultas.h
	gcc -Wall -c consultas_client.c

consultas_svc.o : consultas_svc.c
	gcc -Wall -c consultas_svc.c

consultas_clnt.o : consultas_clnt.c
	gcc -Wall -c consultas_clnt.c

consultas_xdr.o : consultas_xdr.c
	gcc -Wall -c consultas_xdr.c

consultas_xdr.c consultas_clnt.c consultas_svc.c consultas.h : consultas.x
	@echo "Generando protocolo RPC..."
	rpcgen consultas.x

clean :
	@echo "Limpiando archivos..."
	$(RM) *.o *_xdr.c client server consultas_clnt.c consultas_svc.c salida consultas.h usuarios
