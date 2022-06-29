all : client server
.PHONY : all

client: fecha_so_client.o fecha_so_clnt.o
	echo "Compilando cliente..."
	gcc -Wall -lnsl -o client fecha_so_client.o fecha_so_clnt.o

server: fecha_so_server.o fecha_so_svc.o
	echo "Compilando server..."
	gcc -Wall -lnsl -o server fecha_so_server.o fecha_so_svc.o

fecha_so_server.o : fecha_so_server.c fecha_so.h
	gcc -Wall -c fecha_so_server.c

fecha_so_client.o : fecha_so_client.c fecha_so.h
	gcc -Wall -c fecha_so_client.c

fecha_so_svc.o : fecha_so_svc.c
	gcc -Wall -c fecha_so_svc.c

fecha_so_clnt.o : fecha_so_clnt.c
	gcc -Wall -c fecha_so_clnt.c

fecha_so_clnt.c fecha_so_svc.c fecha_so.h : fecha_so.x
	@echo "Generando protocolo RPC..."
	rpcgen fecha_so.x

clean :
	@echo "Limpiando archivos..."
	$(RM) *.o client server fecha_so_clnt.c fecha_so_svc.c salida fecha_so.h
