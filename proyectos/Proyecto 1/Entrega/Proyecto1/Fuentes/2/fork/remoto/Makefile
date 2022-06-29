all : client server tarea
.PHONY : all

server : proceso_server.c proceso_svc.o
	@echo "Compilando server..."	
	gcc -Wall -lnsl -o server proceso_server.c proceso_svc.o

proceso_svc.o : proceso_svc.c
	gcc -Wall -lnsl -c proceso_svc.c

client : proceso_client.c proceso_clnt.o
	@echo "Compilando cliente..."
	gcc -Wall -lnsl -o client proceso_client.c proceso_clnt.o

proceso_clnt.o : proceso_clnt.c
	gcc -Wall -lnsl -c proceso_clnt.c

proceso_clnt.c proceso_svc.c : proceso.x
	@echo "Generando protocolo RPC..."	
	rpcgen proceso.x

tarea : tarea.c
	@echo "Compilando tarea..."	
	gcc -Wall -o tarea tarea.c

clean :
	@echo "Limpiando archivos..."
	$(RM) *.o client server proceso_clnt.c proceso_svc.c tarea salida proceso.h
