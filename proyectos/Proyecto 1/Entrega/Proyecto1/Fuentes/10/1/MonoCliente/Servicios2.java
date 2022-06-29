/*
	-Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	-Ejercicio 10 -minioperaciones con sockets-Servicios2
	-Cerdá Gianni Lucas - Garat Manuel
*/

package MonoCliente;

//Extendemos los getters y setters para el servicio aritmetico binario a hexadecimal

public class Servicios2 extends Servicios{

	private int[] argumentos;

	private String operacion;

	private int result;

	private String result2;

	// Getters

	@Override
	public String getOperacion(){ return operacion; }

	@Override
	public int[] getArgumentos(){ return argumentos; }

	public int getResultado(){ return result; }

	public String getResultado2(){ return result2; }

	// Setters


	@Override
	public void setOperacion(String operacion){ this.operacion = operacion; }

	@Override
	public void setArgumentos(int [] arg ){ argumentos = arg; }

	public void setResultado(int result){ this.result = result; }

	public void setResultado2(String result2){ this.result2 = result2; }
}
