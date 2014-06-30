/**
*  @Autores:  Juan Pereira 09-11173
*	      Marrero Erick 09-10981
*/

import java.io.*;

/**
 * The Interface Validar_Interface.
 */
public interface Validar_Interface 
          extends java.rmi.Remote {
    
    /**
     * Imprimir.
     *
     * @param a the a
     * @throws RemoteException the remote exception
     */
    public void Imprimir(String a)
        throws java.rmi.RemoteException;
    
    /**
     * Validar.
     *
     * @param login the login
     * @param pass the pass
     * @return true, if successful
     * @throws RemoteException the remote exception
     */
    public boolean validar(String login, String pass)
        throws java.rmi.RemoteException;

}