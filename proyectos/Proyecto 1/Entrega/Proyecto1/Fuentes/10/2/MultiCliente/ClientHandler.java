/**
*-Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
*-Ejercicio 10 -minioperaciones con sockets-ClientHandler-MultiCliente
*-Cerdá Gianni Lucas - Garat Manuel
**/

package MultiCliente;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class ClientHandler extends Thread{
	private Socket socketclient;
	private ObjectInputStream streamlectura;
	private ObjectOutputStream streamescritura;

	private long timeini , timefin ;

	//Constructor
	public ClientHandler (Socket s, ObjectInputStream sl, ObjectOutputStream se ) {
		this.socketclient = s;
        this.streamlectura = sl;
        this.streamescritura = se;
        
	}
// Hilo que ejecuta los servicios pedidos por los diferentes clientes
	public void run() {
		int result;
        float resultdiv;
		String result2;
		Servicios servicio;
		Servicios2 servicio2;

		while(true) {
            try{
			    servicio = (Servicios) streamlectura.readObject();	
			    this.recordTime();
			    
			    if (servicio.getOperacion().equals("Salir")) {
                    System.out.println("");
				    System.out.println("El cliente envió Salir...");
            		System.out.print("Cerrando conexión..");
            		this.socketclient.close();
            		System.out.println("..conexión cerrada.");
            		break;
			    }
			    else {
				    if ((servicio instanceof Servicios2)) {
					    servicio2 = (Servicios2) servicio;
				        result2 = BinarioHexadecimal(servicio.getArgumentos()[0]);
				        servicio2.setResultado2(result2);
				        // Escribimos el stream de salida basado a la respuesta del cliente
				        streamescritura.writeObject(servicio2);
				    }
				    else {
                      	if(servicio.getOperacion().equals("/")) {
					       	 resultdiv = Division(servicio.getArgumentos());
					       	 servicio.setResultadoDiv(resultdiv);
					       	 // Escribimos el stream de salida basado a la respuesta del cliente
					       	 streamescritura.writeObject(servicio);
				        	}	
                        	else{
                            	result = EjecutarOperacion(servicio.getOperacion(),servicio.getArgumentos());
					        	servicio.setResultado(result);
					        	// Escribimos el stream de salida basado a la respuesta del cliente
					        	streamescritura.writeObject(servicio);
                        		}
                   		 }		
			    }
			    this.stopTime();
                
                if (servicio.getOperacion().equals("+"))
			    System.out.println("* La operación SUMA demoro "+ getTime() +" nanosegundos ...");	 
                else
                    if (servicio.getOperacion().equals("-"))
                        System.out.println("* La operacion RESTA demoro "+ getTime() +" nanosegundos ...");	
                    else
                        if (servicio.getOperacion().equals("*"))
                            System.out.println("* La operación MULTIPLICACIÓN demoro "+ getTime() +" nanosegundos ...");	
                        else 
                            if (servicio.getOperacion().equals("/"))
                                System.out.println("* La operación DIVISIÓN demoro "+ getTime() +" nanosegundos ...");	
                            else
                                if (servicio.getOperacion().equals("dab"))
                                    System.out.println("* La operación DECIMAL A BINARIO demoro "+ getTime() +" nanosegundos ...");	
                                else
                                    if (servicio.getOperacion().equals("bah"))
                                        System.out.println("* La operación BINARIO A HEXADECIMAL demoro "+ getTime() +" nanosegundos ...");	
	        }catch (IOException e) {e.printStackTrace();}
	         catch (ClassNotFoundException e) {System.out.println("Fallo en el handler del cliente");}
        }
        try
        {
            // closing resources
            this.streamlectura.close();
            this.streamescritura.close();
            this.stop(); 
        }catch(IOException e){
            e.printStackTrace();}
	} 
  

	private int EjecutarOperacion (String operacion, int [] args) {
		int i ; 
        int resultado;
		resultado = 0;

		//SUMA
		if (operacion.equals("+")) {
			resultado=0;
			for(i=0;i<args.length;i++)
				resultado+=args[i];
		}
		else
			//RESTA
			if (operacion.equals("-")) {
				resultado=args[0];
				for(i=1;i<args.length;i++)
					resultado-=args[i];
			}
			else
				//MULTIPLICACION
				if (operacion.equals("*"))
					resultado=args[0]*args[1];
				else
				//DECIMAL A BINARIO
				    if (operacion.equals("dab"))
					    resultado=DecimalBinario(args[0]);

	return resultado;
	}

	//Metodos  para el calculo del tiempo

	private void recordTime() { timeini = System.nanoTime();}

	private void stopTime() { timefin = System.nanoTime();}

	private long getTime() { return (timefin-timeini);}


	// Funcion para el cálculo decimal a binario, binario a hexadecimal y division

	private int DecimalBinario (int num_dec) {
		 int rem,i,binary;
		    i=1;
		    binary=0;
		    while (num_dec!=0)
		    {
		        rem=num_dec%2;
		        num_dec/=2;
		        binary+=rem*i;
		        i*=10;
		    }
		    return binary;
	}
	private String BinarioHexadecimal(int num_bin) { /* Function to convert binary to hex.*/
		int[] hex = new int[1000];
    	int i = 1, j = 0, rem, dec = 0, bin;

    	bin = num_bin;
    	while (bin > 0) {
    		rem = bin % 2;
    		dec = dec + rem * i;
    		i = i * 2;
    		bin = bin / 10;
    	}
    	i = 0;
    	while (dec != 0) {
    		hex[i] = dec % 16;
    		dec = dec / 16;
    		i++;
    	}
    	String resuHex ="";
    	for (j = i - 1; j >= 0; j--){
    		if (hex[j] > 9){
    			resuHex += (char)(hex[j] + 55);
    		}
    		else{
    			resuHex +=hex[j];
    		}
    	}
    	return resuHex;
	}

    private float Division (int [] args){
        float resultdiv;
		resultdiv=(float)args[0]/args[1];
        return resultdiv;
   }

} // Fin Clase ClientHandler

