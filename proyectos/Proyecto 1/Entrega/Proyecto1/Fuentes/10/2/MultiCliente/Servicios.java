/*
	-Proyecto N°1 Sistemas Distribuidos Primer Cuatrimestre 2020
	-Ejercicio 10 -minioperaciones con sockets-Servicios-MultiCliente
	-Cerdá Gianni Lucas - Garat Manuel
*/
package MultiCliente;

import java.io.Serializable;


public class Servicios implements Serializable{

	private int[] argumentos;

	private String operacion;

	private int result;

    private float resultdiv;

	//Getters

	public String getOperacion(){ return operacion; }

	public int[] getArgumentos(){ return argumentos; }

	public int getResultado(){ return result; }
    
    public float getResultadoDiv(){ return resultdiv; }

	//Setters

	public void setArgumentos(int [] arg ){ argumentos = arg; }

	public void setResultado(int result){ this.result = result; }

    public void setResultadoDiv(float resultdiv){ this.resultdiv = resultdiv; }

	public void setOperacion(String operacion){ this.operacion = operacion; }
}
