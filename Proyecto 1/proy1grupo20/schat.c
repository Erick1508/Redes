/********************************************************
 * Modulo de Servidor SCHAT				*
 * Es el modulo que se ejecuta para recibir conexiones 	*
 * de clientes para enviar mensajes cortos		*
 * Autor: Juan Pereira 09-11173				*
 * 	  Erick Marrero 09-10981			*
 ********************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
/**Declaracion de Constantes Globales*/
#define MAX_THREADS 40
#define MAX_CLIENTE 40

/**DECLARACION DE VARIABLES GLOBALES*/
/**Definicion de la estructura a utilizar para los clientes*/
typedef struct{
  char nombre_cliente[32][32];
  int socket[32];
  int tam;
  char sala[40];
}sala;
/**Arreglo de la estructura sala */
sala lista_usuario[MAX_CLIENTE];
char *salas[10];
/**La variable 'contador' es un contador para el arreglo de la
 * estructura. Mientras 'conexiones' es un contador para la 
 * 'lista_conexiones' que sera la lista de usuarios que se 
 * conectan al servidor */
int contador = 0, conexiones = 0;
int lista_sockets[1000];
char lista_conexiones[1000][1000];
/**Arreglo de hilos para manejar las conexiones*/
pthread_t hilos[MAX_THREADS];
/**Semaforo para mantener la concurrencia sin perder valores
 * de variables globales*/
pthread_mutex_t espera = PTHREAD_MUTEX_INITIALIZER;


void *conexion(void *socket_hilo){
    /**DECLARACION DE VARIABLES*/
    int sock = *(int *)socket_hilo;
    int buffer_tam, chequeo, i,j;
    char *mensaje = (char *)malloc(sizeof(char) * 256);
    char *nombre_cliente = (char *)malloc(sizeof(char) * 32);
    char *comando = (char *)malloc(sizeof(char) * 4);
    char *indicacion = (char *)malloc(sizeof(char) * 252);
    char *respuesta = (char *)malloc(sizeof(char) * 256);
  
    while(1){
      /**Se recibe un mensaje por el socket*/
      if((buffer_tam = recv(sock, mensaje, 100, 0)) > 0) {
	mensaje[buffer_tam] = '\0';  
	/**Los mensajes vienen de la forma <usuario> <comando> <indicacion>*/
	chequeo = sscanf(mensaje, "%s %s %[^\n]%*c", nombre_cliente, comando, indicacion);
	strcpy(respuesta, nombre_cliente);
	strcat(respuesta, " ");
	strcat(respuesta, comando);
	strcat(respuesta, " ");
	/**'respuesta' es la variable que almacena el mensaje de respuesta
	 * para el cliente*/
	    if (chequeo == 3){
	      /**Verificamos que comando se esta ingresando*/
	      /**Comando 'men'*/
 	      if(!strcmp(comando, "men")){			
 		pthread_mutex_lock(&espera);
		strcat(respuesta, indicacion);
		int i = 0, esta = 0;
		/**buscamos si el usuario esta suscrito en una sala*/
 		for(i=0; i <= contador; i++){
		  int j = 0, tam = lista_usuario[i].tam;
		  for (j=0;j<tam;j++){
		    if (!strcmp(nombre_cliente,lista_usuario[i].nombre_cliente[j])){
		      esta = 1;
		      break;
		    }
		  }
		  if (esta==1){
		    /**Pos cada sala en que este suscrito el usuario
		     * se enviara el mensaje deseado a cada usuario
		     * que tambien este suscrito a esta*/
		    for (j=0;j<tam;j++){
			if (send(lista_usuario[i].socket[j], respuesta, strlen(respuesta), 0) <= 0){
			  perror("Error al enviar informacion de respuesta");
			  exit(0);
			}
		    }
		  }
		}
		pthread_mutex_unlock(&espera);
	      /**Comando 'sus'*/
	      }else if(!strcmp(comando, "sus")){
		pthread_mutex_lock(&espera);
		int pos = -1;
		int i = 0;
		/**Se verifica que la sala indicada exista*/
		for (i=0;i<=contador;i++){
		  if (!strcmp(indicacion,lista_usuario[i].sala)){
		      pos = i;
		      break;
		  }
		}
		if (pos < 0){
		  strcat(respuesta, "La sala indicada NO existe");  
		}else{
		  int j = 0;
		  i = 0;
		  int tam = lista_usuario[pos].tam;
		  /**Se verifica si el usuario ya esta suscrito a la sala*/
		  for (j=0;j<tam;j++){
		    if (!strcmp(nombre_cliente,lista_usuario[pos].nombre_cliente[j])){
		      i = 1;
		      break;
		    }
		  }
		  if (i == 1){
		    strcat(respuesta, "El usuario ya esta suscrito en la sala");  
		  }else{
		    /**En caso de no estar suscrito, se suscribe a la sala*/
		    strcpy(lista_usuario[pos].nombre_cliente[tam],nombre_cliente);
		    lista_usuario[pos].socket[tam] = sock;
		    lista_usuario[pos].tam++;
		    strcat(respuesta, "El usuario se ha suscrito exitosamente");  
		  }
		}
		/**Se envia el mensaje al cliente con el resultado de la
		 * operacion deseada*/
		if (send(sock, respuesta, strlen(respuesta), 0) <= 0){
		  error("Error al enviar informacion de respuesta");
		}
		pthread_mutex_unlock(&espera);
	      /**Comando 'cre'*/
	      }else if(!strcmp(comando, "cre")){
		/**Se creara una sala en el servidor*/
		pthread_mutex_lock(&espera);
		int pos = -1;
		/**Se verifica si la sala ya existe*/
		for (i=0;i<=contador;i++){
		    if (!strcmp(indicacion,lista_usuario[i].sala)){
		      pos = i;
		      break;
		    }
		}
		if (pos >= 0){
		    strcat(respuesta, "La sala indicada ya existe");  
		}else{
		    /**Se crea la sala en el servidor*/
		    contador++;
		    strcpy(lista_usuario[contador].sala,indicacion);
		    lista_usuario[contador].tam = 0;
		    strcat(respuesta, "La sala fue creada satisfactoriamente");  
		}
		/**Se envia el mensaje al cliente con el resultado de la
		 * operacion deseada*/
		if (send(sock, respuesta, strlen(respuesta), 0) <= 0){
		  error("Error al enviar informacion de respuesta");
		}
		pthread_mutex_unlock(&espera);
	      /**Comando 'eli'*/
	      }else if(!strcmp(comando, "eli")){
		/**No se puede eliminar todas las salas del servidor,
		 * debe haber al menos 1*/
		pthread_mutex_lock(&espera);
		if (contador > 0) {
		  int pos = -1;
		  for (i=0;i<=contador;i++){
		    if (!strcmp(indicacion,lista_usuario[i].sala)){
		      pos = i;
		      break;
		    }
		  }
		  if (pos >= 0) {
		  /**Se elimina la sala del servidor*/
		    if (pos != contador) {
		      strcpy(lista_usuario[pos].sala,lista_usuario[contador].sala);
		      lista_usuario[pos].tam = lista_usuario[contador].tam;
		      for (j=0;j<32;j++){
			strcpy(lista_usuario[pos].nombre_cliente[j],lista_usuario[contador].nombre_cliente[j]);
			lista_usuario[pos].socket[j] = lista_usuario[contador].socket[j];
		      }
		      strcpy(lista_usuario[contador].sala, " ");
		      lista_usuario[contador].tam = 0;
		    }
		    /**Se des-suscriben los usuarios de la sala*/  
		    for (j=0;j<32;j++){
			strcpy(lista_usuario[contador].nombre_cliente[j], " ");
			lista_usuario[contador].socket[j] = 0;
		    }
		    contador--;
		    strcat(respuesta, "La sala fue eliminada satisfactoriamente");  
		  }else{
		    strcat(respuesta, "Sala NO encontrada");
		  }
		  
		} else {
		  strcat(respuesta, "Solo queda una (1) sala, No puede eliminarse");
		}  
		/**Se envia el mensaje al cliente con el resultado de la
		 * operacion deseada*/
		  if (send(sock, respuesta, strlen(respuesta), 0) <= 0){
		    error("Error al enviar informacion de respuesta");
		  }
		 pthread_mutex_unlock(&espera);
	      }else{
		/**El comando no es de la forma <Comando> <indicacion> validos*/
		strcat(respuesta,"Comando NO encontrado\n");
		if (send(sock, respuesta, strlen(respuesta), 0) <= 0){
		      error("Error al enviar informacion de respuesta");
		}
	      }
	      
	    }else if(chequeo == 2){
	      /**Comando 'sal'*/
	      if(!strcmp(comando, "sal")){
		pthread_mutex_lock(&espera);
		int i = 0, j = 0;
		int soc;
		/**Se alamacenan las salas que estan en el servidor y se 
		 * concatenan a 'respuesta' para enviarlo al usuario que 
		 * uso el comando 'sal'*/
		for (i = 0;i<=contador;i++){
		  strcat(respuesta, lista_usuario[i].sala);
		      if ((i+1) <= contador) {
			strcat(respuesta, ", ");
		      }
		}
		strcat(respuesta, ".");
		/**Se envia el mensaje al cliente con el resultado de la
		 * operacion deseada*/
		if (send(sock, respuesta, strlen(respuesta), 0) <= 0){
		  error("Error al enviar informacion de respuesta");
		}
		pthread_mutex_unlock(&espera);
	      /**Comando 'usu'*/
	      }else if(!strcmp(comando, "usu")){
		pthread_mutex_lock(&espera);
		int i = 0;
		/**Se concatenan los usuarios conectados a 'respuesta'
		 * para enviarselo al usuario que lo requiere*/
		for (i=0;i<conexiones;i++){
		  strcat(respuesta, lista_conexiones[i]);
		  strcat(respuesta, ". ");
		}
		/**Se envia el mensaje al cliente con el resultado de la
		 * operacion deseada*/
		if (send(sock, respuesta, strlen(respuesta), 0) <= 0){
		  error("Error al enviar informacion de respuesta");
		}
		pthread_mutex_unlock(&espera);
	      /**Comando 'fue' y 'des'*/
	      }else if ((!strcmp(comando, "fue")) || (!strcmp(comando, "des"))){
		  /**Aki el nombre de cliente es el usuario que se 
		   * desconectara o des-suscribira de las salas*/
		  pthread_mutex_lock(&espera);
		  int i =0;
		  /**Se busca el cliente y se eliminara de cada una de las
		   * salas en la que esta suscrito*/
		  for (i=0;i<=contador;i++){
		    int tam = lista_usuario[i].tam;
		    int pos = -1;
		    /**se busca la posicion en la que esta el cliente*/
		    for (j=0;j<tam;j++){
		      if (!strcmp(nombre_cliente,lista_usuario[i].nombre_cliente[j])){
		      pos = j;
		      break;
		      }
		    }
		    if (pos >= 0){
		      /**Se elimina de la sala*/
		      if (pos != tam-1){
			strcpy(lista_usuario[i].nombre_cliente[pos],lista_usuario[i].nombre_cliente[tam-1]);
			lista_usuario[i].socket[pos] = lista_usuario[i].socket[tam-1];
			
		      }
		      lista_usuario[i].tam--;
		    }
		    
		  }
		  /**Si el comando era 'des', se ejecuta hasta aqui, si por el
		   * contrario es 'fue' continua aca su ajecucion*/
		  if (!strcmp(comando, "fue")){
		    int pos = -1;
		    /**Se busca la posicion en la que esta en la lista de 
		     * conexiones*/
		    for (i=0;i<conexiones;i++){
		      if (!strcmp(nombre_cliente,lista_conexiones[i])){
			pos = i;
			break;
		      }
		    }
		    /**Se elimina de la lista de las conexiones*/
		    if (pos >= 0){
		      if (pos != (conexiones-1)){
		      strcpy(lista_conexiones[pos],lista_conexiones[conexiones-1]);
		      lista_sockets[pos] = lista_sockets[conexiones-1];
		      }
		      conexiones--;
		    }
		    strcat(respuesta,nombre_cliente);
		    /**Se envia el mensaje al cliente con el resultado de la
		      * operacion deseada*/
		    if (send(sock, respuesta, strlen(respuesta), 0) <= 0){
		      error("Error al enviar informacion de respuesta");
		    }
		    pthread_mutex_unlock(&espera);
		    break;  
		  }else{
		    /**Se envia el mensaje al cliente con el resultado de la
		    * operacion deseada*/
		    strcat(respuesta,"Cliente Des-suscrito de todas las salas");
		    if (send(sock, respuesta, strlen(respuesta), 0) <= 0){
		      error("Error al enviar informacion de respuesta");
		    }
		    pthread_mutex_unlock(&espera);
		  }
		  
	      }else{
		/**Se envia un mensaje al cliente indicando que hubo
		 * un error con el comando que envio*/
		pthread_mutex_lock(&espera);
		strcat(respuesta,"Comando NO encontrado\n");
		if (send(sock, respuesta, strlen(respuesta), 0) <= 0){
		      error("Error al enviar informacion de respuesta");
		}
		pthread_mutex_unlock(&espera);
	      }
	    }
  
      }
      /**se limpian las variables que se usan*/
      strcpy(indicacion," ");
      strcpy(mensaje," ");
      strcpy(comando," ");
      strcpy(nombre_cliente," ");
      strcpy(respuesta," ");
    }
  /**Se cierra el socket y se termina la ejecucion de la conexion*/
  free(mensaje);
  free(nombre_cliente);
  free(comando);
  free(indicacion);
  free(respuesta);
  close(sock);
  pthread_exit(NULL);
}
    
main(int argc, char *argv[]) {
  /**DECLARACION DE VARIABLES*/
  int sockfd, newsockfd, *socket_hilo;
  struct sockaddr_in clientaddr, serveraddr;
  int clientaddrlength, puerto, result = 0;
  char *p,*s,*no_es_entero;
  char aux[] = "", sala[] = "";
  char *buffer = (char *)malloc(sizeof(char) * 256);
  int pid;

  /**Se verifica si los argumentos pasados son validos*/
  if (argc != 5) {
    if (argc != 3) {
      perror("La sintaxis es: ./schat -p <puerto> -s <sala> \nLa sintaxis es: ./schat -s <sala> -p <puerto>");
      exit(1);
    }
  }
  /** se asigna los parametros -p y -s **/  
  p = argv[1];
  if (argc == 3) {
     s = "-s";
  }else{
    s = argv[3];
  }
  /** Se verifica el orden de los parametros */
  if ((strcmp(p,"-p") == 0) && (strcmp(s,"-s") == 0))  {
    
    puerto = strtol(argv[2], &no_es_entero, 10);
    if (*no_es_entero != '\0') {
      fprintf(stderr, "El puerto ingresado %s NO es un ENTERO.", no_es_entero);
      exit(1);
    }
    if ((puerto < 0) || (puerto < 1023)){
      fprintf(stderr, "El puerto ingresado %s es un ENTERO < 0\no esta entre los puerto bien conocidos (es menor que 1023)\n", no_es_entero);
      exit(1);
    }
    /**No se indico el parametro de sala por lo que se asigna la sala 
     * por default 'actual'*/
    if (argc == 3) {
      strcpy(sala,"actual");
    }else{
      strcpy(sala,argv[4]);
    }
  /**Se verifica si el orden de parametros esta invertido*/  
  }else if ((strcmp(p,"-s") == 0) && (strcmp(s,"-p") == 0)){
    s = p;
    p = "-p";
    /**Se verifica que el numero de puerto sea un entero positivo*/
    puerto = strtol(argv[4], &no_es_entero, 10);
    if (*no_es_entero != '\0') {
      fprintf(stderr, "El puerto ingresado %s NO es un ENTERO.", no_es_entero);
      exit(1);
    }
    if (puerto < 0) {
      fprintf(stderr, "El puerto ingresado %s es un ENTERO < 0\n", no_es_entero);
      exit(1);
    }
    strcpy(sala,argv[2]);
  }else{
    perror("La sintaxis de invocacion esta errada");
    exit(1);
  }
  
  strcpy(lista_usuario[contador].sala,sala);
  lista_usuario[contador].tam = 0;
  /** Abrimos un TCP socket. */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    perror("No se puedo abrir el socket");
    exit(0);
  }

  /** Asociamos la direccion al socket. */  
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(puerto);
  if (bind(sockfd, (struct sockaddr *) &serveraddr,sizeof(serveraddr)) != 0){
      perror("No se pudo asociar el socket");
      exit(0);
  }
  if (listen(sockfd, MAX_CLIENTE) < 0){
      perror("Error escuchando por el socket");
      exit(0);
  }

    printf("###################################\n");
    printf("####### BIENVENIDOS A SCHAT #######\n");
    printf("###################################\n\n");
    printf("puerto: %d\n",puerto);
  
   int i;
   /**Mientras no se llene el servidor esperaremos por conexiones*/
   for (i=0; i < MAX_THREADS; i++){
    /** Esperamos por una conexion */
    clientaddrlength = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *) &clientaddr, &clientaddrlength);
    if (newsockfd < 0){
	perror("No se puedo abrir el socket");
	exit(0);
    }
    if (recv(newsockfd, buffer, 32, 0) < 0){
	perror("Error al recibir nombre del cliente");
	exit(0);
    }

    int g = 0;
    char existe[] = "0";
    /**Verificamos que el usuarios a ingresar no este suscrito
     * al servidor*/
    pthread_mutex_lock(&espera);
    for (g=0;g<conexiones;g++){
      if (!strcmp(lista_conexiones[g],buffer)){
	strcpy(existe,"1");
	break;
      }
    }
    /**Escribimos en el buffer '1' si el usuario ya esta suscrito
     * en el servidor, y escribimos un '0' sino*/
    if (!strcmp("0",existe)){
	strcpy(lista_conexiones[conexiones],buffer);
	lista_sockets[conexiones] = newsockfd;
	conexiones++;
	strcpy(buffer,existe);
    }else{
	strcpy(buffer,existe);
    }
    
    /**Le enviamos un mensaje al cliente para indicarle si el estado del
     * buffer y este lo atajara y permitira su suscripcion al servidor 
     * o la rechazara si este ya esta suscrito */
    if(send(newsockfd, buffer, 1, 0) < 0){
	  error("Error al enviar informacion de inicio");
    }
    socket_hilo = malloc(1);
    *socket_hilo = newsockfd;
    /**Creamos el hilo que manejara la conexion del cliente entrante*/
    if ((pthread_create(&hilos[i], NULL, conexion, (void *)socket_hilo)) < 0){
            perror("ERROR en crear nuevo thread\n");
            exit(1);
    }
    pthread_mutex_unlock(&espera);
    
  }
  /**Se cierra el socket*/
  free(buffer);
  free(socket_hilo);
  close(newsockfd);
}