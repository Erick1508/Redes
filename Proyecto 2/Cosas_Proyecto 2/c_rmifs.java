import java.rmi.Naming;
import java.rmi.RemoteException;
import java.net.MalformedURLException;
import java.rmi.NotBoundException;
import java.io.*;
import java.rmi.*;
import java.util.Scanner;



public class c_rmifs {

    public static void main(String[] args) {
	String host = "";
	String puer ="" ;
	String usuario = "";
	String clave = "";
	boolean Con_archivo = false;
	if ((args.length == 8) || (args.length == 4)){
	    int puerto = 0;
	    
	    if (args.length == 8){
	      if (!args[0].equals("-f") || (!args[2].equals("-m")) || (!args[4].equals("-p")) || (!args[6].equals("-c"))){
		System.out.println("Error en la sintaxis Intente con lo siguiente");
		System.out.println("java c_rmifs -f usuarios -m servidor -p <puerto> -c comandos;    ");
		System.out.println("java c_rmifs -m servidor -p <puerto>");
		System.exit(0);
	      }else {
		try{
		  System.out.println(" \nAbriendo archivo... ");
		  String[] aux;
		  aux = cargarUsuario(args[1]);
		  usuario = aux[0];
		  clave = aux[1];
		  Con_archivo = true;
		} catch (IOException p){
		  System.out.println("Error al cargar el archivo usuario ");
		  System.exit(0);
		}
		
	      }
	      try {
		
		puerto = Integer.parseInt(args[5]);
		puer = args[5];
		host = args[3];
		
	      } catch (Exception p){
		System.out.println("Error: El puerto debe ser solo numeros");
		System.exit(0);
	      }
	       
	    }else if (args.length == 4){
	    
	      if (!args[0].equals("-m") || (!args[2].equals("-p")) ){
		System.out.println("Error en la sintaxis Intente con lo siguiente");
		System.out.println("java c_rmifs -f usuarios -m servidor -p <puerto> -c comandos;");
		System.out.println("java c_rmifs -m servidor -p <puerto>");
		System.exit(0);
	      }
	      try {
		
		puerto = Integer.parseInt(args[3]);
		puer = args[3];
		host = args[1];
	      } catch (Exception p){
		System.out.println("Error: El puerto debe ser solo numeros");
		System.exit(0);
	      }	    
	    }
	
	}else {
	  
	  System.out.println("Error en la cantidad de los argumentos: intente con lo siguiente: ");
	  System.out.print("java c_rmifs -f usuarios -m servidor -p <puerto> -c comandos;    ");
	  System.out.println("java c_rmifs -m servidor -p <puerto>");
	  System.exit(0);
	}	
	
        try {
            //  Este programa asume que el rmiregistry estara en la maquina
            //    akarso y recibira peticiones por el puerto 21000
            Comandos c = (Comandos)
	    // el servidor nos brinda el CalculatorService
	    // Naming.lookup("rmi://remotehost/CalculatorService"); 
// 	    Naming.lookup("rmi://akarso.ldc.usb.ve:20982/CalculatorService"); 
	    Naming.lookup("rmi://"+host+":"+puer+"/ServicioArchivos");

	    if (c.Autenticacion(usuario,clave)){
	      System.out.println(" Cliente registrado");
	    }else{
		
		while (true){
		  usuario = "";
		  clave = "";
		  System.out.print(" \nUsuario y clave Incorrectos\nUsuario: ");
		  
		  BufferedReader lectura = new BufferedReader(new InputStreamReader(System.in));
		  BufferedReader lectura2 = new BufferedReader(new InputStreamReader(System.in));
		  try{
		    usuario = lectura.readLine();
		    System.out.print("Clave: "); 
		    clave = lectura2.readLine();
		    if (c.Autenticacion(usuario,clave)){
		      System.out.println(" Cliente Registrado ");
		      break; 
		    } 
		    
		  }catch (IOException e) {
		    System.out.println("Excepción: ");
		    e.printStackTrace();
		    System.exit(0);
		  }	  
		  
	      }
	      
	    }    
	    
	    String comand;
	    char[] pal1;
	    String[] aux_pal;
	    String pal3 = "";
	    BufferedReader lectura = new BufferedReader(new InputStreamReader(System.in));
	    System.out.println("   ########################################################");
	    System.out.println("   ################# BIENVENIDO " + usuario+ " ################");
	    System.out.println("   ########################################################");
	    System.out.println("\n");
	    if (Con_archivo){
	      try{
		cargarComandos(c, args[7],usuario);
	      }catch (IOException e) {
		      System.out.println("Excepción: ");
		      System.exit(0);
	      }	
	    }
	    while (true){
	      System.out.println(" Introduzca un comando:");
	      System.out.print(">> ");
	      try{
		comand = lectura.readLine();
		comand = comand.trim();
		pal1 = comand.toCharArray();
		aux_pal = Palabras(pal1);
		aux_pal[0] = aux_pal[0].trim();
		aux_pal[1] = aux_pal[1].trim();
			
		
		if (!ejecutar_comando(c, comand, aux_pal,usuario)){
		  break;
		};
	      }catch (IOException e) {
		System.out.println("Excepción: ");

		System.exit(0);
	      }
		    
	          
	    }
	    
	    System.out.println("   ");
	    System.out.println("   SALIENDO........ ");
	    System.out.println("   ");
	    System.out.println("\n");
	               
        }
        catch (MalformedURLException murle) {
            System.out.println();
            System.out.println(
              "MalformedURLException");
            System.out.println(murle);
        }
        catch (RemoteException re) {
            System.out.println();
            System.out.println(
                        "RemoteException");
            System.out.println(re);
        }
        catch (NotBoundException nbe) {
            System.out.println();
            System.out.println("NotBoundException, puerto Incorrecto");
            System.out.println(nbe);
        }
    }
    
  public static boolean ejecutar_comando(Comandos c, String comand, String[] aux_pal, String usuario){
	  int cantidad_pal = Integer.parseInt(aux_pal[2]);
	  try{
	      if (cantidad_pal == 0 ){
		  if (comand.equals("rls")){
		    System.out.println("\nArchivos disponibles en el servidor centralizado:\n");
		    System.out.println( c.rls(usuario) );
		  
		  }else if (comand.equals("lls")){
		    System.out.println("\nArchivos disponibles en el servidor local:\n");
		    System.out.println( lls_c());
		    
		  } else if (comand.equals("info")){
		    System.out.println("\nOpciones disponibles para el cliente:\n");
		    System.out.println( c.info(usuario) );
		  } else if (comand.equals("sal")){
		    c.sal(usuario,"sal",null);
		    return false;
		  } else {
		    System.out.println("\nError, comando NO encontrado \n");
		  }
	      } else if (cantidad_pal == 1){
		  
		  if (aux_pal[0].equals("sub")){
		      if (Chequeo_de_archivo(aux_pal[1])){
			  
			  File file = new File(aux_pal[1]);
			  byte buffer[] = new byte[(int)file.length()];
			  BufferedInputStream archivo = new 
					BufferedInputStream(new FileInputStream(file.getName()));
			  archivo.read(buffer,0,buffer.length);
			  archivo.close();
			  boolean subido = c.sub(buffer, aux_pal[1],usuario);
			  if (subido){
			    System.out.println("Archivo subido correctamente ");
			  } else {
			    System.out.println("Error al subir el archivo ");
			  }
		        
		      }else{
			  c.sal(usuario,"sub",aux_pal[1]);
			  System.out.println("El Archivo "+ aux_pal[1]+ " NO se encuentra disponible en el servidor local ");
		      }
		    
		  }else if (aux_pal[0].equals("baj")){
		  
		    if (c.Chequeo_archivo_c(aux_pal[1])){
			System.out.println("Descargando archivo..... ");
			byte[] archivo_bin = c.baj(aux_pal[1],usuario);
			if (archivo_bin == null) {
			  throw new NullPointerException();
			}
			
			  // se crea el nuevo archivo
			  BufferedOutputStream output = new 
				BufferedOutputStream(new FileOutputStream(aux_pal[1], false));
			  // se escribe en el archivo nuevo todo lo que retorno el servidor
			  output.write(archivo_bin, 0, archivo_bin.length);
			  output.flush();
			   
			  output.close();
			  System.out.println(" Archivo descargado ");
		    } else {
		      c.sal(usuario,"baj",aux_pal[1]);
		      System.out.println("El Archivo "+ aux_pal[1]+ " NO se encuentra disponible en el servidor centralizado ");
		    }		    
		    
		  }else if (aux_pal[0].equals("bor")){
		    System.out.println( c.bor(aux_pal[1],usuario));
		  		    
		  } else {
		    System.out.println("Error, comando NO encontrado \n");
		  }		  
	    } else {
		  System.out.println("Error, comando NO encontrado \n");
	      }
	  }catch (IOException e) {
		System.out.println("\nExcepción: Fallo la conexion\n");
		
		System.exit(0);
	  }
	  
	  return true;
      
 }
    
  public static String[] Palabras(char[] pal){
    int count = 0;
    String primera = "";
    String segunda= "";
    int i = 0;
    int j = 0;
    String[] palabra = new String[40];
    boolean pase = true;
    char[] aux = new char[40];
    
    while (i < pal.length){
      if (pase){
	  if (pal[i] == ' '){
	    count++;
	    primera = String.valueOf(aux);
	    pase = false;	    
	    aux = new char[40];	
	    j = 0;
	  }else{
	    aux[j] = pal[i];
	    j++;
	  }
      }else {
	  aux[j] = pal[i];

	  j++;
      }
      
      i++;
    }
    segunda = String.valueOf(aux);
    palabra[0] = primera;
    palabra[1] = segunda;
    palabra[2] = String.valueOf(count);
    return palabra;
 }
 
 public static String lls_c(){
    String comando = new String("");
    File fichero=new File(".");

    File[] listaArchivos=fichero.listFiles();
    for(int i=0; i<listaArchivos.length; i++){
	if (listaArchivos[i].isFile()){
	  comando+= listaArchivos[i].getName() + "\n";
	}
    }
    return comando;
 }
 
 public static boolean Chequeo_de_archivo(String archi){
    
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
 
 public static String[] cargarUsuario(String entrada) throws IOException{        
      Scanner lector= null;
      String[] usu_cla = new String[2]; 
      try {
	/*Declaracion de varibles a emplear */
	File f = new File(entrada);
	lector = new Scanner(f);
	String bound = "";
	boolean ver = true;
	while (lector.hasNextLine()){
	  
	  bound = lector.nextLine();
	  usu_cla = bound.split(":");  
	  if (ver){
	    ver = false;
	  }else{
	    System.out.println("Archivo de entrada tiene mas de un usuario");
	    System.out.println("Se toma en consideracion el ultimo ");
	  }
	  
	}	 

      }catch (FileNotFoundException e) {
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
      
      return usu_cla;
  }
 
 public static String[] cargarComandos(Comandos c,String entrada,String usuario) throws IOException{        
      Scanner lector= null;
      String[] aux_pal= new String[2];
      char[] pal1;
      try {
	/*Declaracion de varibles a emplear */
	File f = new File(entrada);
	lector = new Scanner(f);
	String bound = "";
	
	while (lector.hasNextLine()){
	  
	  bound = lector.nextLine();
	  bound = bound.trim();
	  pal1 = bound.toCharArray();
	  aux_pal = Palabras(pal1);
	  aux_pal[0] = aux_pal[0].trim();
	  aux_pal[1] = aux_pal[1].trim();
	  
	  if (!ejecutar_comando(c,aux_pal[1],aux_pal,usuario)){
	    System.out.println("   ");
	    System.out.println("   SALIENDO........ ");
	    System.out.println("   ");
	    System.exit(0);
	  };
	  
	}	 

      }catch (FileNotFoundException e) {

	  System.out.println("Archivo de entrada no esta en el directorio ");
	  System.exit(0);
      } finally {
	  try {
	    lector.close();
	  } catch(Exception e){
	     System.out.println("El lector del archivo de entrada no se puede cerrar correctamente");
	  }

      }
      
      return aux_pal;
  }
 
 
 
}





