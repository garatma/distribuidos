/*
	-Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	-Ejercicio 10 -minioperaciones con sockets-Cliente-MultiCliente
	-Cerdá Gianni Lucas - Garat Manuel
*/
package MultiCliente;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

// Clase cliente
public class Cliente {
	private Socket socket;
	private ObjectInputStream streamlectura;
	private ObjectOutputStream streamescritura;

	// Operaciones para cada servicio aritmetico
	private Servicios servicio_arit;
	private Servicios2  servicio_arit2;

	//Para el calculo del tiempo
	private long timeini , timefin ;

	//Constructor del cliente recibe como parametro una ip y un puerto de comunicacion con el servidor

	public Cliente  (String ip, int port) throws UnknownHostException, IOException{
		socket = new Socket(ip,port);
		System.out.println("Conectando... El socket ha establecido conexión con el SERVER en el PUERTO: "+port+". ");
        System.out.println("");
	}

	// Procedimiento para la comunicacion lectura y escritura
	public void SetComunication() throws IOException{
		streamescritura= new ObjectOutputStream (socket.getOutputStream());
		streamlectura= new ObjectInputStream (socket.getInputStream());
	}

	// Cierro conexiones
	public void Close() throws IOException {
		streamlectura.close();
		streamescritura.close();
		socket.close();
	}

	// Ejecuto la entrada para los servicios aritmeticos
	public void EjecutarServicios() throws ClassNotFoundException, IOException{
		int i,opcion;
		int [] cant_operandos;
		String entrada;
		String [] arg;
		boolean continuar =true;
        boolean menu = false;

		//Entrada para elegir el servicio
		BufferedReader read = new BufferedReader (new InputStreamReader(System.in));
        if (!menu){
		    System.out.println("//---------------OPERACIONES VÁLIDAS| Se asumen operadores consistentes.--------------//");
            System.out.println("");		
		    System.out.println("//--------------------------------------------------------//");
		    System.out.println(" |Presione 1 para SUMAR;|"); 
            System.out.println(" |Presione 2 para RESTAR;|");
            System.out.println(" |Presione 3 para MULTIPLICAR;|"); 
            System.out.println(" |Presione 4 para DIVIDIR;|");
            System.out.println(" |Presione 5 para la conversión en DECIMAL A BINARIO;|"); 
            System.out.println(" |Presione 6 para la conversión en BINARIO A HEXADECIMAL;|"); 
            System.out.println(" |Presione 7 para SALIR|");
		    System.out.println("//--------------------------------------------------------//");
            System.out.println("");	
        }
		while (continuar) {
            if(menu){
            System.out.println("");		
		    System.out.println("//--------------------------------------------------------//");
		    System.out.println(" |Presione 1 para SUMAR;|"); 
            System.out.println(" |Presione 2 para RESTAR;|");
            System.out.println(" |Presione 3 para MULTIPLICAR;|"); 
            System.out.println(" |Presione 4 para DIVIDIR;|");
            System.out.println(" |Presione 5 para la conversión en DECIMAL A BINARIO;|"); 
            System.out.println(" |Presione 6 para la conversión en BINARIO A HEXADECIMAL;|"); 
            System.out.println(" |Presione 7 para SALIR|");
		    System.out.println("//--------------------------------------------------------//");
            System.out.println("");	            
            }
            menu=true;
			System.out.println("Ingrese el servicio aritmetico que desea ejecutar ");
  
			entrada =read.readLine();
			arg=entrada.split(" ");
			opcion=Integer.parseInt(arg[0]);

			switch (opcion) {

				//---------------------------------------SUMA---------------------------------------------------------------

				case 1: {
                    System.out.println("");
					System.out.println("----------------------SUMA--------------------- ");
                    System.out.println("");
					System.out.println("Ingrese operandos (entre 2 y 4), separados por espacios y presione la tecla Enter: ");
					entrada = read.readLine();
					arg = entrada.split(" ");
					cant_operandos = new int [arg.length];

					if (arg.length>1 && arg.length<5) {
						for(i=0;i<arg.length;i++)
							cant_operandos[i]=Integer.parseInt(arg[i]);

						servicio_arit = new Servicios();
						servicio_arit.setOperacion("+");
						servicio_arit.setArgumentos(cant_operandos);

						// Calculo el tiempo en nanosegundos

						recordTime();

						streamescritura.writeObject(servicio_arit);
						servicio_arit = (Servicios) streamlectura.readObject();

						stopTime();
                           
                        System.out.println("");
						System.out.println(" --> El resultado de la suma es: "+ servicio_arit.getResultado());
						System.out.println(" --> El tiempo de ejecucion fue de: "+ getTime() + " nanosegundos...");

					}
					else{
                        System.out.println("");
                        System.out.println("ERROR--> La cantidad de operandos es incorrecta...");
                        System.out.println("");
                    }
				}break;

				//---------------------------------------RESTA---------------------------------------------------------------

				case 2: {
                    System.out.println("");
					System.out.println("-------------------RESTA---------------------- ");
                    System.out.println("");
					System.out.println("Ingrese la cantidad de operandos (entre 2 y 4), separados por espacios y presione la tecla Enter: ");
					entrada = read.readLine();
					arg = entrada.split(" ");
					cant_operandos = new int [arg.length];

					if (arg.length>1 && arg.length<5) {
						for(i=0;i<arg.length;i++)
							cant_operandos[i]=Integer.parseInt(arg[i]);

						servicio_arit = new Servicios();
						servicio_arit.setOperacion("-");
						servicio_arit.setArgumentos(cant_operandos);

						// Calculo el tiempo en nanosegundos

						recordTime();

						streamescritura.writeObject(servicio_arit);
						servicio_arit = (Servicios) streamlectura.readObject();

						stopTime();
                        
                        System.out.println("");
						System.out.println(" --> El resultado de la resta es: "+ servicio_arit.getResultado());
						System.out.println(" --> El tiempo de ejecucion fue de: "+ getTime() + " nanosegundos...");

					}
					else{
                        System.out.println("");
                        System.out.println("ERROR--> La cantidad de operandos es incorrecta...");
                        System.out.println("");
                    }
				}break;// case

				//---------------------------------------MULTIPLICACION---------------------------------------------------------------

				case 3: {
                    System.out.println("");
					System.out.println("-------------------MULTIPLICACION---------------------- ");
                    System.out.println("");
					System.out.println("Ingrese 2 operandos separados por espacios y presione la tecla Enter: ");
					entrada = read.readLine();
					arg = entrada.split(" ");
					cant_operandos = new int [arg.length];

					if (arg.length==2) {
						for(i=0;i<arg.length;i++)
							cant_operandos[i]=Integer.parseInt(arg[i]);

						servicio_arit = new Servicios();
						servicio_arit.setOperacion("*");
						servicio_arit.setArgumentos(cant_operandos);

						// Calculo el tiempo en nanosegundos

						recordTime();

						streamescritura.writeObject(servicio_arit);
						servicio_arit = (Servicios) streamlectura.readObject();

						stopTime();
                        
                        System.out.println("");
						System.out.println(" --> El resultado de la multiplicacion es: "+ servicio_arit.getResultado());
						System.out.println(" --> El tiempo de ejecucion fue de: "+ getTime() + " nanosegundos...");

					}
					else{
                        System.out.println("");
                        System.out.println("ERROR--> La cantidad de operandos es incorrecta...");
                        System.out.println("");
                    }
				}break;// case

				//---------------------------------------DIVISION---------------------------------------------------------------

				case 4: {
                    System.out.println("");
					System.out.println("-------------------DIVISION---------------------- ");
                    System.out.println("");
					System.out.println("Ingrese 2 operandos separados por espacios y presione la tecla Enter: ");
					entrada = read.readLine();
					arg = entrada.split(" ");
					cant_operandos = new int [arg.length];

					if (arg.length==2) {
						for(i=0;i<arg.length;i++)
							cant_operandos[i]=Integer.parseInt(arg[i]);

						servicio_arit = new Servicios();
						servicio_arit.setOperacion("/");
						servicio_arit.setArgumentos(cant_operandos);

						// Calculo el tiempo en nanosegundos

						recordTime();

						streamescritura.writeObject(servicio_arit);
						servicio_arit = (Servicios) streamlectura.readObject();

						stopTime();
                    
                        System.out.println("");
						System.out.println(" --> El resultado de la division es: "+ servicio_arit.getResultadoDiv());
						System.out.println(" --> El tiempo de ejecucion fue de: "+ getTime() + " nanosegundos...");

					}
					else{
                        System.out.println("");
                        System.out.println("ERROR--> La cantidad de operandos es incorrecta...");
                        System.out.println("");
                    }
				}break;// case

				//---------------------------------------DECIMAL A BINARIO---------------------------------------------------------------

				case 5: {   
                    System.out.println("");
					System.out.println("-------------------DECIMAL A BINARIO---------------------- ");
                    System.out.println("");
					System.out.println("Ingrese un numero en decimal a convertir y presione la tecla enter");
					entrada = read.readLine();
					arg = entrada.split(" ");
					cant_operandos = new int [arg.length];

					if (arg.length==1) {
						for(i=0;i<arg.length;i++)
							cant_operandos[i]=Integer.parseInt(arg[i]);
                 
						servicio_arit = new Servicios();
						servicio_arit.setOperacion("dab");
						servicio_arit.setArgumentos(cant_operandos);

						// Calculo el tiempo en nanosegundos

						recordTime();

						streamescritura.writeObject(servicio_arit);
						servicio_arit = (Servicios) streamlectura.readObject();

						stopTime();
                           
                        System.out.println("");
						System.out.println(" --> El numero en binario es: "+ servicio_arit.getResultado());
						System.out.println("El tiempo de ejecucion fue de: "+ getTime() + " nanosegundos...");

					}
					else{
                        System.out.println("");
                        System.out.println("ERROR--> La cantidad de operandos es incorrecta...");
                        System.out.println("");
                    }
				}break;// case

				//---------------------------------------BINARIO a HEXADECIMAL---------------------------------------------------------------

				case 6: {
                    System.out.println("");
					System.out.println("-------------------BINARIO a HEXADECIMAL---------------------- ");
                    System.out.println("");
					System.out.println("Ingrese numero en binario a convertir y presione la tecla enter");
					entrada = read.readLine();
					arg = entrada.split(" ");
					cant_operandos = new int [arg.length];

					if (arg.length==1) {
						for(i=0;i<arg.length;i++)
							cant_operandos[i]=Integer.parseInt(arg[i]);
                        
                        if(!EsBinario(cant_operandos[0])){
                        System.out.println("");
					    System.out.println("El número ingresado no es un número binario");
                        break;     
                        }    
						servicio_arit2 = new Servicios2();
						servicio_arit2.setOperacion("bah");
						servicio_arit2.setArgumentos(cant_operandos);

						// Calculo el tiempo en nanosegundos

						recordTime();

						streamescritura.writeObject(servicio_arit2);
						servicio_arit2 = (Servicios2) streamlectura.readObject();

						stopTime();
                    
                        System.out.println("");
						System.out.println(" --> El numero en hexadecimal es: "+ servicio_arit2.getResultado2());
						System.out.println(" --> El tiempo de ejecucion fue de: "+ getTime() + " nanosegundos...");

					}
					else{
                        System.out.println("");
                        System.out.println("ERROR--> La cantidad de operandos es incorrecta...");
                        System.out.println("");
                    }
				}break;// case

				//---------------------------------------------SALIDA----------------------------------------------------------

				case 7:{
                    System.out.println("");
					servicio_arit = new Servicios();
					servicio_arit.setOperacion("Salir");
					servicio_arit.setArgumentos(null);

					streamescritura.writeObject(servicio_arit);
					this.Close();
                    System.out.println("");
					System.out.println("Conexion cerrada por el cliente");
					continuar=false;
				}break;
			}
		}
	}//Fin Clase Cliente

	//Metodos  para el calculo del tiempo

	private void recordTime() { timeini = System.nanoTime();}

	private void stopTime() { timefin = System.nanoTime();}

	private long getTime() { return (timefin-timeini);}

    // Verifico que el numero que pasan para la conversión de binario a decimal es realmente un numero binario

    private boolean EsBinario (int num_bin){
	   int d;
	   while(num_bin!=0){
		  d=num_bin%10;
		  if(d>1)
	        return false;
		  else
	        num_bin=num_bin/10;
	        }
     return true;
     }


	public static void main(String[] args) {

		if (args.length<2) {
			System.out.println("Faltan parametros.");
			System.out.println("Usage: Cliente [IP] [Port] ");
			System.exit(0);
		}

		String ip = args[0];
		int port =Integer.parseInt(args[1]);

		try {
			Cliente clnt = new Cliente (ip,port);
			try {
				clnt.SetComunication();
				clnt.EjecutarServicios();
				clnt.Close();
                System.exit(0);
			}catch(Exception e){
				System.out.println("ERROR en la comunicacion con el Server");
                System.out.println("Caracteres no soportados "+e.getMessage());
				clnt.Close();
                System.out.println("");
				System.out.println("El Socket Cliente fue cerrado...");
				System.exit(0);
			}
		}catch(IOException io){
            System.out.println("");
			System.out.println("Se produjo un ERROR en la creacion del CLIENTE...");
			System.exit(0);
		}
	}
} // main


