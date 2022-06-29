import java.net.*;
import java.io.*;

// espera requests de clientes y les responde con sus mismos mensajes.

// uso:
// java Server.java

public class Server {
	public static void main(String args[]) {
		DatagramSocket socket = null;
		try {
			socket = new DatagramSocket(6789);
			byte[] buffer = new byte[1000];
			while(true) {
				DatagramPacket request = new DatagramPacket(buffer, buffer.length);
				socket.receive(request); // recibir request

				DatagramPacket reply = new DatagramPacket(
						request.getData(), 
						request.getLength(), 
						request.getAddress(), 
						request.getPort()
				);
				socket.send(reply); // enviar reply
			}
		} catch (SocketException e) {
			System.out.println("Socket: " + e.getMessage());
		} catch (IOException e) {
			System.out.println("IO: " + e.getMessage());
		} finally {
			if (socket != null) socket.close();
		}
	}
}
