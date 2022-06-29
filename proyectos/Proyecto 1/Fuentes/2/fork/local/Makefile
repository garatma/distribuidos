all : padre tarea 
.PHONY : all
padre : padre.c	
	@echo "Compilando..."
	gcc -Wall padre.c -o padre
		
tarea : tarea.c
	gcc -Wall tarea.c -o tarea 

clean :
	@echo "Limpiando archivos..." 
	$(RM) salida tarea padre 
