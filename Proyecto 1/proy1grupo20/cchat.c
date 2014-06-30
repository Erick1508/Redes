/********************************************************
 * Modulo de Cliente CCHAT				*
 * Es el modulo que se ejecuta para pedir conexiones 	*
 * al servidor y enviar mensajes cortos a usuarios	*
 * suscritos a salas comunes				*
 * Autor: Juan Pereira 09-11173				*
 * 	  Erick Marrero 09-10981			*
 ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

char *nombre="";
pthread_mutex_t espera = PTHREAD_MUTEX_INITIALIZER;;

/**Se maneja el hilo a traves de esta funcion, el cual 
 * recibe toda la informacion enviada por el servidor
 * a traves del socket.*/
void *conexion(void *socket_uss){
  /**DECLARACION DE VARIABLES*/
  int sock = *(int *)socket_uss;
  int buffer_tam, chequeo, i;
  char *trama_respuesta = (char *)malloc(sizeof(char) * 256);
  char *nombre_cliente = (char *)malloc(sizeof(char) * 30);
  char *comando = (char *)malloc(sizeof(char) * 4);
  char *mensaje = (char *)malloc(sizeof(char) * 224);
  char *respuesta = (char *)malloc(sizeof(char) * 256);
  buffer_tam = 0;
  i = 0;
	bzero((char *)nombre_cliente, strlen(nombre_cliente));
	bzero((char *)trama_respuesta, sizeof(trama_respuesta));
	bzero((char *)mensaje, strlen(mensaje));
	/**Espera por alguna respuesta por parte del servidor*/
	while((buffer_tam = recv(sock, trama_respuesta, 100, 0))>0) {
	  trama_respuesta[buffer_tam] = '\0';
	  /**Se verifica que como viene la trama*/
	  chequeo = sscanf(trama_respuesta, "%s %s %[^\n]%*c", nombre_cliente, comando, mensaje);
	  
	  if(buffer_tam > 0) {
	      /**Se ejecuta el comando requerido segun lo deseado
	       * y se muestra un msj al usuario*/
	      if(!strcmp(comando, "men")){			
		printf("mensaje recibido de %s:\n%s\n",nombre_cliente,mensaje);
		
	      }else if(!strcmp(comando, "sus")){
		printf("%s\n",mensaje);
		
	      }else if(!strcmp(comando, "cre")){
		printf("%s\n",mensaje);
		
	      }else if(!strcmp(comando, "eli")){
		printf("%s\n",mensaje);
		
	      }else if(!strcmp(comando, "sal")){
		printf("Las salas que tiene el servidor son las siguientes:\n");
		printf("%s\n",mensaje);
		
	      }else if(!strcmp(comando, "usu")){
		printf("Los usuarios que estan suscritos en el servidor son:\n");
		printf("%s\n",mensaje);
		
	      }else if(!strcmp(comando, "fue")){
		printf("Se ha desconetado el usuario:\n");
		printf("%s\n",mensaje);
		break;
	      }else if(!strcmp(comando, "des")){
		printf("%s\n",mensaje);
	      }else{
		printf("%s\n",mensaje);
	      }
	    }
	    
	    if (buffer_tam  = 0){
	      fprintf(stderr, "El servidor se ha desconectado\n");
	      break;
	    }
	    if (buffer_tam < 0) {
	      printf("ERROR: LECTURA DE SOCKETS");
	      break;
	    }
	    
	}
  free(trama_respuesta);
  free(nombre_cliente);
  free(comando);
  free(mensaje);
  free(respuesta);
  close(sock);
  pthread_exit(NULL);
}

/**Funcion para imprimir los mensajes de error del sistema*/
void error(const char *msg){
    fprintf(stderr,"%s\n",msg);
    exit(0);
}

/**Esta funcion es para la introduccion de comandos*/
void comandos(void *socket_uss){
  /**DECLARACION DE VARIABLES*/
  int sock = *(int *) socket_uss;
  char *comando = (char *)malloc(sizeof(char) * 4); 
  char *indicacion = (char *)malloc(sizeof(char) * 252);
  char *buffer = (char *)malloc(sizeof(char) * 256);
  int chequeo = 0;
  
  while (1) {
    /**Se espera por un comando en pantalla*/
    fgets(buffer, 256, stdin);
    /**Si pasan un enter directo da error*/
    /**Se verifica cuantos elementos va a tener la trama*/
    chequeo = sscanf(buffer, "%s %[^\n]%*c", comando, indicacion);
    if (chequeo == 2){
      /**Se enviara el comando 'men'*/
      if (!strcmp(comando, "men")){
	/**Se arma la trama*/
	strcpy(buffer, nombre);
	strcat(buffer, " ");
	strcat(buffer, "men");
	strcat(buffer, " ");
	strcat(buffer, indicacion);
	/**Envio el msj <nombre> <comando> <indicacion>*/
	    if (write(sock, buffer, strlen(buffer)) <= 0){
		error("Error: Servidor no encontrado\n");
		break;
	    }
      /**Se enviara el comando 'sus'*/
      }else if (!strcmp(comando, "sus")){
	/**Se arma la trama*/
	strcpy(buffer, nombre);
	strcat(buffer, " ");
	strcat(buffer, "sus");
	strcat(buffer, " ");
	/**Indicacion es la sala a la cual se va a suscribir*/
	strcat(buffer, indicacion);
	/**Envio el msj <nombre> <comando> <indicacion>*/
	    if (write(sock, buffer, strlen(buffer)) <= 0){
		error("Error: Servidor no encontrado\n");
		break;
	    }
      /**Se enviara el comando 'cre'*/
      }else if (!strcmp(comando, "cre")){
	/**Se arma la trama*/
	strcpy(buffer, nombre);
	strcat(buffer, " ");
	strcat(buffer, "cre");
	strcat(buffer, " ");
	strcat(buffer, indicacion);
	/**Envio el msj <nombre> <comando> <indicacion>*/
	    if (write(sock, buffer, strlen(buffer)) <= 0){
		error("Error: Servidor no encontrado\n");
		break;
	    }
      /**Se enviara el comando 'eli'*/
      }else if (!strcmp(comando, "eli")){
	/**Se arma la trama*/
	strcpy(buffer, nombre);
	strcat(buffer, " ");
	strcat(buffer, "eli");
	strcat(buffer, " ");
	strcat(buffer, indicacion);
	/**Envio el msj <nombre> <comando> <indicacion>*/
	    if (write(sock, buffer, strlen(buffer)) <= 0){
		error("Error: Servidor no encontrado\n");
		break;
	    }
	
      }else{
	printf("Error comando NO encontrado.\n");
      }
      
    }else if (chequeo == 1){
      /**Se enviara el comando 'sal'*/
      if (!strcmp(comando, "sal")){
	/**Se arma la trama*/
	strcpy(buffer, nombre);
	strcat(buffer, " ");
	strcat(buffer, "sal");
	/**Envio el msj <nombre> <comando>*/
	     if (write(sock, buffer, strlen(buffer)) <= 0){
		error("Error: Servidor no encontrado\n");
		break;
	      }
	
      /**Se enviara el comando 'usu'*/
      }else if (!strcmp(comando, "usu")){
	/**Se arma la trama*/
	strcpy(buffer, nombre);
	strcat(buffer, " ");
	strcat(buffer, "usu");
	/**Envio el msj <nombre> <comando>*/
	     if (write(sock, buffer, strlen(buffer)) <= 0){
		error("Error: Servidor no encontrado\n");
		break;
	      }
      /**Se enviara el comando 'fue'*/
      }else if (!strcmp(comando, "fue")){
	/**Se arma la trama*/
	strcpy(buffer, nombre);
	strcat(buffer, " ");
	strcat(buffer, "fue");
	/**Envio el msj <nombre> <comando>*/
	     if (write(sock, buffer, strlen(buffer)) <= 0){
		error("Error: Servidor no encontrado\n");
		break;
	      }
	break;
      /**Se enviara el comando 'des'*/
      }else if (!strcmp(comando, "des")){
	/**Se arma la trama*/
	strcpy(buffer, nombre);
	strcat(buffer, " ");
	strcat(buffer, "des");
	/**Envio el msj <nombre> <comando>*/
	     if (write(sock, buffer, strlen(buffer)) <= 0){
		error("Error: Servidor no encontrado\n");
		break;
	      }
      }else{
	printf("Error comando NO encontrado.\n");
      }
      
    }else{
      printf("Error comando NO encontrado.\n");
    }
    
  }
  free(comando); 
  free(indicacion);
  free(buffer);
}

/**Esta funcion se encargara del procesamiento del archivo indicado*/
int Leer_Archivo(FILE *archivo,void *socket) {
  int sock = *(int *) socket;
  int long_linea = 128;
  int chequeo = 0,buffer_tam=0;
  char *linea = (char *)malloc(sizeof(char) * long_linea);
  char *comando = (char *)malloc(sizeof(char) * 4);
  char *comando1 = (char *)malloc(sizeof(char) * 4);
  char *nombre_cliente = (char *)malloc(sizeof(char) * 30);
  char *indicacion = (char *)malloc(sizeof(char) * 80);
  char *trama_respuesta = (char *)malloc(sizeof(char) * 256);
  char *respuesta = (char *)malloc(sizeof(char) * 256);
  char *mensaje = (char *)malloc(sizeof(char) * 224);
  
  /**Leemos linea a linea el archivo hasta la ultima linea,
   * a la par de esto se iran ejecutando cada uno de los comandos
   * indicados en el archivo*/
  while(feof(archivo) == 0){
        fgets(linea, long_linea, archivo);
	/**Se verifica que la linea no sea vacia*/
        if (strlen(linea) > 0) {
	  chequeo = sscanf(linea, "%s %[^\n]s", comando, indicacion);
	  strcpy(respuesta, nombre);
	  strcat(respuesta, " ");
	  strcat(respuesta, comando);
	  strcat(respuesta, " ");
	  strcat(respuesta, indicacion);	  
	  /**Si el comando no es 'fue', se escribe en el socket*/
	  if (strcmp(comando, "fue")){
	    if (write(sock, respuesta, strlen(respuesta)) <= 0){
		error("Error: Servidor no encontrado\n");
		break;
		
	    }
	  
	  }
	  /**Se hace Sleep para que el usuario vea paso a paso
	   * que es lo que se esta ejecutando*/
	  sleep(1);
	}
	strcpy(indicacion," ");
	strcpy(comando," ");
	strcpy(linea," ");
	if (buffer_tam  = 0){
	      fprintf(stderr, "El servidor se ha desconectado\n");
	      break;
	}   
	if (buffer_tam < 0) {
	      printf("ERROR: LECTURA DE SOCKETS");
	      break;
	}
    }
  /**se libera la memoria de las variables usadas*/  
  free(linea);
  free(comando);
  free(comando1);
  free(nombre_cliente);
  free(indicacion);
  free(trama_respuesta);
  free(respuesta);
  free(mensaje);  
  return 0;
}

main(int argc, char *argv[]) {
  /**DECLARACION DE VARIABLES*/
  int sockfd, *socket_hilo;
  struct sockaddr_in serveraddr;
  struct in_addr inaddr;
  struct hostent *host;
  char *server, *no_es_entero;
  int puerto = 0,ent=0;
  char *buffer = (char *)malloc(sizeof(char) * 256);
  char *h = "",*p = "",*n="",*a="",*IP="";
  FILE *nombre_Archivo;
  pthread_t hilo;

  /** Se verifica que la sintaxis sea la correcta */
  if (argc != 9){
    if (argc != 7){
      error("La sintaxis es: ./schat -h <host> -p <puerto> -n <nombre> -a <archivo>");
    } 
  }

  /**Esto es para verificar si esta ausente la -a, que pde o no ser
   pasada como parametro **/
  if (argc == 7){
     a = "-a";
     ent = 7;
  }else{
     ent = 9; 
  }  

  /** Se verifican los parametros pasados por la consola*/
  int i = 1;
  for (i;i < ent;i++){
    if (strcmp(argv[i],"-h") == 0){
      h = argv[i];
      IP = argv[i+1];
    }else if (strcmp(argv[i],"-p") == 0){
      p = argv[i];
      puerto = strtol(argv[i+1], &no_es_entero, 10);
	if (*no_es_entero != '\0') {
 	  error("El puerto ingresado %s es un ENTERO < 0");
	}
	if (puerto < 0) {
 	  error("El puerto ingresado %s es un ENTERO < 0");
	}
    }else if (strcmp(argv[i],"-n") == 0){
      n = argv[i];
      nombre = argv[i+1];
    }else if (strcmp(argv[i],"-a") == 0){
      a = argv[i];
      nombre_Archivo = fopen(argv[i+1],"r");
    }
    i++;
  }
  
 
  /**Se verifica que no falte ninguno de los parametros requeridos*/
  if ((strcmp(h,"-h") != 0) || (strcmp(p,"-p") != 0) || (strcmp(n,"-n") != 0) || (strcmp(a,"-a") != 0)){
    error("Falta la especificacion de alguno de los parametros <-a,-n,-p,-h>");
  }
  
  if (argc == 7){
     a = " ";  
  }

  /** Se abre el socket */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    perror("No se puedo abrir el socket");
    exit(0);
  }
    
  /** OBTENEMOS EL SERVIDOR */
  if (inet_aton(IP, &inaddr))
      host = gethostbyaddr((char *) &inaddr, sizeof(inaddr), AF_INET);
  else
      host = gethostbyname(IP);
  
  /**Si el servidor esta erroneo se imprime un mensaje de error*/
  if (!host) {
      error("Error con el host indicado");
    }
  

  /**Se obtiene la direccion del servidor. */
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(puerto);
  bcopy((char *)host->h_addr,(char *)&serveraddr.sin_addr.s_addr,host->h_length);

  /**Se conecta al servidor */
    if (connect(sockfd, (struct sockaddr *) &serveraddr,sizeof(serveraddr)) < 0){
	  error("No Se puede establecer conexion con el servidor");
    }
    strcpy(buffer, nombre);
    /**Se envia el nombre de cliente al servidor para que verifique 
     * si hay algun otro usuario conectado con el mismo nombre*/
    if(send(sockfd, buffer, 32, 0) < 0){
	  error("Error al enviar informacion de inicio");
    }
    strcpy(buffer," ");
    /**Recibe la respuesta del servidor*/
    if (recv(sockfd, buffer, 1, 0) < 0){
	  error("Error al recibir nombre del cliente");
    }
    
    if (!strcmp("1",buffer)){
      printf("El usuario %s ya esta suscrito en el servidor\n", nombre);
      exit(0);
    }
    
  printf("###################################\n");
  printf("####### BIENVENIDOS A CCHAT #######\n");
  printf("###################################\n");
      
  /**hilo*/
  socket_hilo = malloc(1);
  *socket_hilo = sockfd;
  int salio = 1;
  /** Aqui crearemos un hilo para que el programa ejecute de manera
   * paralela la recepcion de informacion mediante el socket
   * mientras que se ejecuta la lectura del archivo y luego la
   * introduccion de comandos del cchat*/  
  if ((pthread_create(&hilo, NULL,conexion, (void *)socket_hilo)) < 0){
      error("ERROR en al crear nuevo hilo\n");
      exit(1);
  }
  /**Si no hay archivo evitamos realizar un llamado a la funcion
   * innecesariamente*/
  if (!(strcmp(a,"-a"))){
    if (nombre_Archivo == NULL) {
	printf("ERROR: Archivo NO encontrado\n");
    }else{
	salio = Leer_Archivo(nombre_Archivo,(void *)socket_hilo);
    } 
  }  
  /**Ejecucion de la introduccion de comandos*/
  comandos((void *)socket_hilo);
  free(socket_hilo);
  free(buffer);
  /**Se cierra el socket*/
  close(sockfd);
  exit(EXIT_SUCCESS);
}