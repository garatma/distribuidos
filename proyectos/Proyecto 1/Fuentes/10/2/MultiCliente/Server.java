/**
*-Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
*-Ejercicio 10 -minioperaciones con sockets-Server-MultiCliente
*-Cerdá Gianni Lucas - Garat Manuel
**/

package MultiCliente;


import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
	private ServerSocket socketserver;
	private Socket socketclient;
	private ObjectInputStream streamlectura;
	private ObjectOutputStream streamescritura;

	//Constructor
	public Server (int port) throws IOException{
		socketserver = new ServerSocket(port);
        System.out.println("");
		System.out.println("Servidor ejecutandose en el puerto: "+port);
        System.out.println("");
	}

	public void Accept() throws IOException{
		socketclient  = socketserver.accept();
		streamescritura = new ObjectOutputStream (socketclient.getOutputStream());
		streamlectura = new ObjectInputStream (socketclient.getInputStream());
		System.out.println("Cliente Conectado. ");
        System.out.println("");
	}

	// Cierro conexiones
	public void Close() throws IOException {
		streamlectura.close();
		streamescritura.close();
		socketclient.close();
		socketserver.close();
	}

    // Permito manejar varios clientes a la vez
    public void Ejecutar() throws IOException,ClassNotFoundException{
        while(true){        
        this.Accept();
        ClientHandler clnthandler = new ClientHandler(socketclient,streamlectura,streamescritura);
        clnthandler.start();
        }
    }
   
	//MAIN

	public static void main(String[] args) {
	// TODO Auto-generated method stub
		if (args.length < 1) {
			System.out.println("Faltan parametros");
			System.out.println("Usage: Server [Port] ");
			System.exit(0);
		}
		int port = Integer.parseInt(args[0]);
		try {
			Server svr = new Server(port);
				try {
					svr.Ejecutar();
				}catch(Exception e){
                    System.out.println("");
					System.out.println("Error al comunicarse con el Cliente");
                    System.out.println(" /Caracteres no soportados/ "+e.getMessage());
					svr.Close();
					System.exit(0);}
			}catch(IOException io){
                System.out.println("");
				System.out.println("No se pudo aceptar el Cliente");
				System.exit(0);}
    }
 } // Fin Clase Server  



