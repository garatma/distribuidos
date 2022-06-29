all:
	rpcgen generala.x
	gcc -c generala_cliente.c `pkg-config --libs --cflags libtirpc` -std=c99 
	gcc -Wall -c generala_servidor.c `pkg-config --libs --cflags libtirpc` -std=c99 
	gcc -c generala_clnt.c `pkg-config --libs --cflags libtirpc`
	gcc -Wall -c generala_svc.c `pkg-config --libs --cflags libtirpc`
	gcc -Wall -c generala_xdr.c `pkg-config --libs --cflags libtirpc`
	gcc -Wall `pkg-config --libs --cflags libtirpc` -lrt -lnsl -o generala_cliente generala_cliente.o generala_clnt.o generala_xdr.o
	gcc -Wall `pkg-config --libs --cflags libtirpc` -lnsl -o generala_servidor generala_servidor.o generala_svc.o generala_xdr.o

clean:
	rm -f *.o
	rm -f generala_cliente
	rm -f generala_servidor
	rm -f generala_clnt.c
	rm -f generala_svc.c
	rm -f generala_xdr.c
