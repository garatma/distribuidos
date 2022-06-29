all : cliente server 
.PHONY : all

server : MultiCliente/Server.class 
	java MultiCliente.Server 14550 

MultiCliente/Server.class : MultiCliente/Server.java 
	@echo "Compilando Servidor.." 
	javac MultiCliente/Server.java

cliente : MultiCliente/Cliente.java 
	@echo "Compilando Cliente.."
	javac MultiCliente/Cliente.java
	
clean:
	 @echo "Limpiando archivos..." 
	 $(RM) MultiCliente/*.class
