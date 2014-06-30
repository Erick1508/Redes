/**
*  @Autores:  Juan Pereira 09-11173
*	      Marrero Erick 09-10981
*/

import java.io.*;


/**
 * The Interface Comandos.
 */
public interface Comandos 
          extends java.rmi.Remote {
    
    /*Muestra la lista de archivos disponibles en 
	en el servidor centralizado*/
    /**
     * Rls.
     *
     * @param usu the usu
     * @return the string
     * @throws RemoteException the remote exception
     */
    public String rls(String usu)
        throws java.rmi.RemoteException;
        
    /**
     * Sal.
     *
     * @param usu the usu
     * @param instru the instru
     * @param archi the archi
     * @throws RemoteException the remote exception
     */
    public void sal(String usu,String instru, String archi) 
      throws java.rmi.RemoteException;

    /**
     * Info.
     *
     * @param usu the usu
     * @return the string
     * @throws RemoteException the remote exception
     */
    public String info(String usu)
        throws java.rmi.RemoteException;

    /**
     * Sub.
     *
     * @param buffer the buffer
     * @param nombre_archivo the nombre_archivo
     * @param usu the usu
     * @return true, if successful
     * @throws RemoteException the remote exception
     */
    public boolean sub(byte[] buffer, String nombre_archivo,String usu)
        throws java.rmi.RemoteException;
        
    /**
     * Baj.
     *
     * @param nombre_archivo the nombre_archivo
     * @param usu the usu
     * @return the byte[]
     * @throws RemoteException the remote exception
     */
    public byte[] baj(String nombre_archivo,String usu)
        throws java.rmi.RemoteException;

    /**
     * Bor.
     *
     * @param archivo the archivo
     * @param usu the usu
     * @return the string
     * @throws RemoteException the remote exception
     */
    public String bor(String archivo,String usu)
        throws java.rmi.RemoteException;
        
    /**
     * Chequeo_archivo_c.
     *
     * @param archi the archi
     * @return true, if successful
     * @throws RemoteException the remote exception
     */
    public boolean Chequeo_archivo_c(String archi)
	throws java.rmi.RemoteException;
	
    /**
     * Autenticacion.
     *
     * @param username the username
     * @param pass the pass
     * @return true, if successful
     * @throws RemoteException the remote exception
     */
    public boolean Autenticacion(String username,String pass)
	throws java.rmi.RemoteException;

    /**
     * Verificar_propietario.
     *
     * @param archivo the archivo
     * @param persona the persona
     * @return true, if successful
     * @throws RemoteException the remote exception
     */
    public boolean Verificar_propietario(String archivo, String persona)
	throws java.rmi.RemoteException;
	
}

