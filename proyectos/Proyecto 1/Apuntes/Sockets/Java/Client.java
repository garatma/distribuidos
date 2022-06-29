import java.net.*;
import java.io.*;

// enviar el primer argumento al servidor e imprimir la respuesta
// en java args[0] es el primer argumento (no el nombre del programa)

// uso:
// java Client.java [mensaje] [IP/nombre del servidor]

public class Client {
	public static void main(String args[]) {

		// el socket
		DatagramSocket socket = null;
		try {
			// preparación request
			socket = new DatagramSocket();
			byte [] mensaje = args[0].getBytes();
			InetAddress aHost = InetAddress.getByName(args[1]);
			int serverPort = 6789;
			DatagramPacket request = new DatagramPacket(mensaje, mensaje.length, aHost, serverPort);

			socket.send(request); // enviar request al servidor

			byte[] buffer = new byte[1000];

			DatagramPacket reply = new DatagramPacket(buffer, buffer.length);
			socket.receive(reply); // recibir respuesta
			System.out.println("Reply: " + new String(reply.getData()));

		} catch (SocketException e) { 
			System.out.println("Socket: " + e.getMessage());
		} catch (IOException e) {
			System.out.println("IO: " + e.getMessage());
		} finally { 
			if(socket != null) socket.close();
		}
	}
}
