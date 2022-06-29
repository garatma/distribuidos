all : cliente server 
.PHONY : all

server : MonoCliente/Server.java 
	@echo "Compilando Servidor.." 
	javac MonoCliente/Server.java

cliente : MonoCliente/Cliente.java 
	@echo "Compilando Cliente.."
	javac MonoCliente/Cliente.java
	
clean:
	 @echo "Limpiando archivos..." 
	 $(RM) MonoCliente/*.class
