import java.rmi.Naming;
import java.rmi.*;
import java.rmi.registry.LocateRegistry;
import java.io.*;


public class s_rmifs {
  private ComandosImpl servicio;
  public s_rmifs(int puerto_rmi,String IP, int puerto_auten) {
    try {
      //  Usar la linea de abajo para no hacer la llamada a rmiregistry
      //     desde la linea de comandos, de esta forma el mismo programa 
      //     servidor crea su propio servicio de nombres antes de publicar 
      //     el objeto remoto c. 
      LocateRegistry.createRegistry(puerto_rmi);
      
	    
      this.servicio = new ComandosImpl((Validar_Interface)Naming.lookup("rmi://"+IP+":"+puerto_auten+"/ServicioAutenticacion"));
      //  Este programa asume que el rmiregistry estara activo en la maquina
      //     akarso, y en el puerto 21000
//       Naming.rebind("rmi://akarso.ldc.usb.ve:20982/CalculatorService", c);
      Naming.rebind("rmi://localhost:"+puerto_rmi+"/ServicioArchivos", this.servicio);
//       rmi://localhost/CalculatorService
    } catch (Exception e) {
      System.out.println("Trouble: " + e);
      System.exit(0);
    }
  }

  public static void main(String args[]) {
    int puertolocal=0;
    int puerto= 0;
    if (args.length == 6) {
      if (!args[0].equals("-l") || (!args[2].equals("-h")) || (!args[4].equals("-r"))){
	System.out.println("Error en la sintaxis, Intente con: ");
	System.out.println("java s_rmifs -l <puertolocal> -h <host> -r <puerto>");
	System.exit(0);
      }
      try {
	puertolocal = Integer.parseInt(args[1]);
	puerto = Integer.parseInt(args[5]);
	
      } catch (Exception p){
	System.out.println("Error: El puerto local y el puerto deben ser solo numeros");
	System.exit(0);
      }
    }else{
      
      System.out.println("Error en la cantidad de los argumentos, Intente con: ");
      System.out.println("java s_rmifs -l <puertolocal> -h <host> -r <puerto>");
      System.exit(0);
    }
  
     s_rmifs server = new s_rmifs(puertolocal,args[3],puerto);
     
     System.out.println("   ########################################################");
     System.out.println("   ################# SERVIDOR CENTRALIZADO ################");
     System.out.println("   ########################################################");
     System.out.println("\n");
     String comand;
     BufferedReader lectura = new BufferedReader(new InputStreamReader(System.in));
     while (true){
      System.out.print(">> ");
      try{
	comand = lectura.readLine();
	comand = comand.trim();
	  
	if (comand.equals("log")){
	  System.out.println("Historial de comandos: ");
	  System.out.println("Usuario  :  Comando usado ");
	  System.out.println(((ComandosImpl)server.servicio).Imprimir_comados());
	}else if (comand.equals("sal")){
	  break;
	}else{
	  System.out.println("Error, comando NO encontrado ");
	};
	
      }catch (IOException e) {
	System.out.println("Excepción: ");
	e.printStackTrace();
	System.exit(0);
      }
		    
     }
     System.out.println("   ");
     System.out.println("   SALIENDO........ ");
     System.out.println("   ");
     System.out.println("\n");
     System.exit(0);
  }
}

