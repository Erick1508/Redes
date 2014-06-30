import java.rmi.Naming;
import java.rmi.*;
import java.rmi.registry.LocateRegistry;
import java.util.Hashtable;
import java.io.*;
import java.util.Scanner;


public class a_rmifs {

  public a_rmifs(String archi, int puerto) {
    try {
      //  Usar la linea de abajo para no hacer la llamada a rmiregistry
      //     desde la linea de comandos, de esta forma el mismo programa 
      //     servidor crea su propio servicio de nombres antes de publicar 
      //     el objeto remoto c. 
      LocateRegistry.createRegistry(puerto);
      

      Validar_Interface c = cargarArchivo(archi);

      
      Naming.rebind("rmi://localhost:"+puerto+"/ServicioAutenticacion", c);

    } catch (Exception e) {
      System.out.println("Trouble: " + e);
    }
  }

  public static void main(String args[]) {
    int puerto= 0;
    if (args.length == 4) {
      if (!args[0].equals("-f") || (!args[2].equals("-p"))){
	System.out.println("Error en la sintaxis, Intente con: ");
	System.out.println("java a_rmifs -f usuarios -p <puerto>");
	System.exit(0);
      }
      try {
	
	puerto = Integer.parseInt(args[3]);
	if (puerto < 0){
	  System.out.println("Error: El puerto debe ser mayor que cero (0)");
	  System.exit(0);
	}
      } catch (Exception puertolocal){
	System.out.println("Error: El puerto deben ser solo numeros");
	System.exit(0);
      }
    }else{
      
      System.out.println("Error en la cantidad de los argumentos, Intente con: ");
      System.out.println("java a_rmifs -f usuarios -p <puerto>");
      System.exit(0);
    }
    
    new a_rmifs(args[1], puerto);
  }
  
  
  public static Validar_Interface cargarArchivo(String entrada) throws IOException{        
      Scanner lector= null;
      Validar_Interface c = null;
      Hashtable<String,String> usu = new Hashtable<String,String>();;
      try {
	/*Declaracion de varibles a emplear */
	File f = new File(entrada);
	lector = new Scanner(f);
	String bound = "";
	String[] usu_cla; 
	while (lector.hasNextLine()){

	  bound = lector.nextLine();
	  usu_cla = bound.split(":");

	  if (usu_cla.length == 2){
	    usu.put(usu_cla[0],usu_cla[1]);
	  }
	}	 

      }
	catch (FileNotFoundException e) {
	  e.printStackTrace();
	  System.out.println("Archivo de entrada no esta en el directorio ");
	  System.exit(0);
	} finally {
	  try {
	    lector.close();
	  } catch(Exception e){
	     System.out.println("El lector del archivo de entrada no se puede cerrar correctamente");
	  }

	}
	c = new Validar_InterfaceImpl(usu);
	return c;
  }
  
  
  
  
}