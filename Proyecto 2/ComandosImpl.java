/**
*  @Autores:  Juan Pereira 09-11173
*	      Marrero Erick 09-10981
*/

import java.io.*;
import java.lang.String;
import java.util.ArrayList;
import java.util.Hashtable;

// TODO: Auto-generated Javadoc
/**
 * The Class ComandosImpl.
 */
public class ComandosImpl extends java.rmi.server.UnicastRemoteObject
    implements Comandos {

    /** The Constant serialVersionUID. */
    private static final long serialVersionUID = 1L;
    
    /** The vi. */
    private static Validar_Interface vi;
    
    /** The comand_log. */
    private ArrayList<String> comand_log = new ArrayList<String>();
    
    /** The archivos_cargados. */
    private Hashtable<String, String> archivos_cargados = new Hashtable<String, String>(); 

    /**
     * Instantiates a new comandos impl.
     *
     * @param val_in the val_in
     * @throws RemoteException the remote exception
     */
    public ComandosImpl(Validar_Interface val_in)
        throws java.rmi.RemoteException {
        super();
        vi = val_in;
    }

    /**
     * Imprimir_comados.
     *
     * @return the string
     * @throws RemoteException the remote exception
     */
    public String Imprimir_comados()
      throws java.rmi.RemoteException {
	String comandos = "";
	for(String comando: comand_log){
	  comandos += comando+ " \n";
	}
	return comandos;
    }
    /*Muestra la lista de archivos disponibles en 
	en el servidor centralizado**/

    public String rls(String usu) 
      throws java.rmi.RemoteException {
	String comando = new String("");
	String s = "";
	File fichero=new File(".");
	File[] listaArchivos=fichero.listFiles();
	for(int i=0; i<listaArchivos.length; i++){
	  if (listaArchivos[i].isFile()){
	    comando+= listaArchivos[i].getName() + "\n";
	  }
	}
	comand_log.add(usu+"  :  rls ");
	if (comand_log.size() > 20){
	  comand_log.remove(0);
	}
	
        return comando;
    }

    /* (non-Javadoc)
     * @see Comandos#sal(java.lang.String, java.lang.String, java.lang.String)
     */
    public void sal(String usu, String instr,String archi) 
      throws java.rmi.RemoteException {
      if (archi == null){
	comand_log.add(usu+"  :  "+instr);
      } else {
	comand_log.add(usu+"  :  "+instr+ " "+archi);
      }
      if (comand_log.size() > 20){
	 comand_log.remove(0);
      }
      
    }
    
    /* (non-Javadoc)
     * @see Comandos#info(java.lang.String)
     */
    public String info(String usu)
        throws java.rmi.RemoteException {
        String infor = new String("");
	infor += "    rls         muestra la lista de archivos disponibles en servidor centralizado \n\n";
	infor += "    lls         muestra la lista de archivos disponibles localmente (en el cliente)\n\n";
	infor += "sub archivo     Sube un archivo al servidor remoto, debe estar disponible en los \n";
	infor += "                archivos que se encuentran localmente en el cliente \n\n";
	infor += "baj archivo     Baja un archivo del servidor remoto, debe estar disponible en los \n";
	infor += "                archivos que se encuentran en elservidor \n\n";
	/*Solo lo puede hacer el cliente que lo subio*/
	infor += "bor archivo     Borra el archivo del servidor(Solo si fue el cliente que lo subio)\n\n";
	infor += "   info         Muestra informacion detallada de los comandos que puede usar\n\n";
	infor += "   sal          Salir del sistema \n\n";
        
	comand_log.add(usu+"  :  info ");
	if (comand_log.size() > 20){
	  comand_log.remove(0);
	}
        return infor;
    }

    /* (non-Javadoc)
     * @see Comandos#sub(byte[], java.lang.String, java.lang.String)
     */
    public boolean sub(byte[] buffer, String nombre_archivo,String usu)
        throws java.rmi.RemoteException {
	try{
	    BufferedOutputStream output = new 
	    BufferedOutputStream(new FileOutputStream(nombre_archivo, false));
	    output.write(buffer, 0, buffer.length);
	    output.flush();
	    output.close();	
	    
	    return true;
	} catch(IOException ioe){
	    System.out.println("ServicioArchivosImpl: " + ioe.getMessage());
	    ioe.printStackTrace();
	    return false;
	}finally{
	  this.archivos_cargados.put(nombre_archivo,usu);
	  comand_log.add(usu+"  :  sub "+ nombre_archivo);
	  if (comand_log.size() > 20){
	    comand_log.remove(0);
	  }
	}
    }
    
    /* (non-Javadoc)
     * @see Comandos#baj(java.lang.String, java.lang.String)
     */
    public byte[] baj(String nombre_archivo,String usu)
        throws java.rmi.RemoteException {
	
	try{
	    File file = new File(nombre_archivo);
	    byte buffer[] = new byte[(int)file.length()];
	    BufferedInputStream input = new 
		    BufferedInputStream(new FileInputStream(file.getName()));
	    input.read(buffer,0,buffer.length);
	    input.close();
	    return(buffer);
	} catch(Exception e){
	    System.out.println("ServicioArchivosImpl: " + e.getMessage());
	    e.printStackTrace();
	    return(null);
	}finally{
	  comand_log.add(usu+"  :  baj "+ nombre_archivo);
	  if (comand_log.size() > 20){
	    comand_log.remove(0);
	  }
	
	}
    
    
    }

    /* (non-Javadoc)
     * @see Comandos#bor(java.lang.String, java.lang.String)
     */
    public String bor(String archivo,String usu)
        throws java.rmi.RemoteException {
        comand_log.add(usu+"  :  bor "+ archivo);
	if (comand_log.size() > 20){
	  comand_log.remove(0);
	}
        if (this.Chequeo_archivo_c(archivo)){	   
		  File file = new File(archivo);
		  if (this.Verificar_propietario(archivo,usu)){
		      if (file.delete()){
			return "El archivo ha sido borrado exitosamente \n";
		      }else{
			return "El archivo no pudo borrarse\n";
		      }
		  } else {
		      return "El archivo no pudo borrarse porque NO eres el propietario\n";
		  }
        }else{
	  return "El archivo indicado no se encuentra en el servidor centralizado\n";
        }
    }
    
    /* (non-Javadoc)
     * @see Comandos#Chequeo_archivo_c(java.lang.String)
     */
    public boolean Chequeo_archivo_c(String archi)
	throws java.rmi.RemoteException{
	
	File fichero=new File(".");

	File[] listaArchivos=fichero.listFiles();
	for(int i=0; i<listaArchivos.length; i++){
	    if (listaArchivos[i].isFile()){
	      if (archi.equals(listaArchivos[i].getName())){
		return true;
	      }
	    }
	}
	return false;
    }

    /* (non-Javadoc)
     * @see Comandos#Autenticacion(java.lang.String, java.lang.String)
     */
    public boolean Autenticacion(String username,String pass)
	throws java.rmi.RemoteException{
	
	try{
	  return vi.validar(username,pass);
	} catch (Exception e){
	  return false;
	}
	
    }
    
    /* (non-Javadoc)
     * @see Comandos#Verificar_propietario(java.lang.String, java.lang.String)
     */
    public boolean Verificar_propietario(String archivo, String persona)
	throws java.rmi.RemoteException{
	String aux = this.archivos_cargados.get(archivo);
	if (aux == null){
	  return false;
	}
	if (aux.equals(persona)){
	  return true;	
	}
	return false;
	
	
    }
 }


