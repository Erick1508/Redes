import java.io.*;

public interface Validar_Interface 
          extends java.rmi.Remote {
    
    public void Imprimir(String a)
        throws java.rmi.RemoteException;
    
    public boolean validar(String login, String pass)
        throws java.rmi.RemoteException;

}