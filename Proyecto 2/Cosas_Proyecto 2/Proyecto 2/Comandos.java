import java.io.*;

public interface Comandos 
          extends java.rmi.Remote {
    
    /*Muestra la lista de archivos disponibles en 
	en el servidor centralizado*/
    public String rls(String usu)
        throws java.rmi.RemoteException;
        
    public void sal(String usu,String instru, String archi) 
      throws java.rmi.RemoteException;

    public String info(String usu)
        throws java.rmi.RemoteException;

    public boolean sub(byte[] buffer, String nombre_archivo,String usu)
        throws java.rmi.RemoteException;
        
    public byte[] baj(String nombre_archivo,String usu)
        throws java.rmi.RemoteException;

    public String bor(String archivo,String usu)
        throws java.rmi.RemoteException;
        
    public boolean Chequeo_archivo_c(String archi)
	throws java.rmi.RemoteException;
	
    public boolean Autenticacion(String username,String pass)
	throws java.rmi.RemoteException;

    public boolean Verificar_propietario(String archivo, String persona)
	throws java.rmi.RemoteException;
	
}

