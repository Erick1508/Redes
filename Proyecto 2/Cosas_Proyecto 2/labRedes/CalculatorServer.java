import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;


public class CalculatorServer {

  public CalculatorServer() {
    try {
      //  Usar la linea de abajo para no hacer la llamada a rmiregistry
      //     desde la linea de comandos, de esta forma el mismo programa 
      //     servidor crea su propio servicio de nombres antes de publicar 
      //     el objeto remoto c. 
      LocateRegistry.createRegistry(21000);
      Calculator c = new CalculatorImpl();
      //  Este programa asume que el rmiregistry estara activo en la maquina
      //     akarso, y en el puerto 21000
      Naming.rebind("rmi://localhost/CalculatorService", c);
//       rmi://localhost/CalculatorService
    } catch (Exception e) {
      System.out.println("Trouble: " + e);
    }
  }

  public static void main(String args[]) {
    new CalculatorServer();
  }
}

