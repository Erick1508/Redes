import java.io.*;
import java.lang.String;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Set;


public class Validar_InterfaceImpl extends java.rmi.server.UnicastRemoteObject
    implements Validar_Interface {

    public static final long serialVersionUID = 1L;
    private Hashtable<String, String> usuarios;


    public Validar_InterfaceImpl(Hashtable<String, String> usuarios)
        throws java.rmi.RemoteException {
	
	this.usuarios = usuarios;
	
    }
    
    
    
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