/**
*  @Autores:  Juan Pereira 09-11173
*	      Marrero Erick 09-10981
*/

import java.io.*;
import java.lang.String;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Set;


/**
 * The Class Validar_InterfaceImpl.
 */
public class Validar_InterfaceImpl extends java.rmi.server.UnicastRemoteObject
    implements Validar_Interface {

    /** The Constant serialVersionUID. */
    public static final long serialVersionUID = 1L;
    
    /** The usuarios. */
    private Hashtable<String, String> usuarios;


    /**
     * Instantiates a new validar_ interface impl.
     *
     * @param usuarios the usuarios
     * @throws RemoteException the remote exception
     */
    public Validar_InterfaceImpl(Hashtable<String, String> usuarios)
        throws java.rmi.RemoteException {
	
	this.usuarios = usuarios;
	
    }
    
    
    
    /* (non-Javadoc)
     * @see Validar_Interface#Imprimir(java.lang.String)
     */
    public void Imprimir(String a)
        throws java.rmi.RemoteException {
	String str;
	Set<String> set = this.usuarios.keySet();
	Iterator<String> itr = set.iterator();
	while (itr.hasNext()) {
	  str = itr.next();
	  System.out.println(str + ":" + this.usuarios.get(str));
	}

	
    }
    
    /* 
     * @see Validar_Interface#validar(java.lang.String, java.lang.String)
     */
    public boolean validar(String login, String pass)
        throws java.rmi.RemoteException{
        String username;

	Set<String> set = this.usuarios.keySet();
	Iterator<String> itr = set.iterator();
	while (itr.hasNext()) {
	  username = itr.next();
	  if ((username.equals(login)) && (this.usuarios.get(username)).equals(pass)){	    
	    return true;
	  }
	}
        
        return false;
   }
  
  
}