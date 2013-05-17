/**
 *      @file Socketslib.c
 *      @brief Este es la implementacion de una libreria de sockets
 *      y manejo de conexiones por el protocolo HTML
 *      
 *      @author Jesus Torres / Julio Lopez
 *      
 *      @date 19/10/2011
 * 
 */

#include "Socketslib.h"

/**
 * Funcion Publica.
 * Este Metodo Imprime por Pantalla la Sintaxys de Ejecucion del programa.
 * @date 19/10/2011
 */
void usage(){
        printf("Sintaxis, descripcion de parametros: \n");
        printf("verific [-t <num_segundos>][-d <directorio>][-a <archivo>] \n\n");
        printf("-t <num_segundos> : numero de segundos que deben transcurrir entre dos revisiones consecutivas de los directorios monitorizados. \n");
        printf("-d <directorio> : a direccion absoluta de un directorio que se desea monitorizar, para ver si en el ha ocurrido algun cambio desde la ultima vez que se efectuo la revision. \n");
        printf("-a <archivo> : archivo en formato de texto plano en el cual cada linea contendra un URL que correspondera a un directorio cuya informacion se desea verificar. \n");
}


/**
 * Funcion Publica.
 * Verifica si es un numero el parametro que recibe.
 * @param num  Lo que se desea verificar si es un numero
 * @return 1 en caso de que sea un numero, 0 en caso contrario
 */
int IsaNumber(int num){

    if (!num) {
        printf("Error en linea de argumentos \n");
        usage();
        return (0);
    }
    return (1);
}

/**
 * Funcion Publica.
 * Verifica si es un archivo el parametro que recibe.
 * @param arg El string que se comprobara si es un archivo valido
 * @return 1 en caso de que sea un archivo, 0 en caso contrario
 */
int IsaFile(char *arg){
    FILE *file;
    if((file = fopen(arg,"r")) == NULL){
        printf("Error en la linea de argumentos \n");
        usage();
        return (0);
    }
    return (1);
}


/**
 * Funcion Privada.
 * Crea el Socket de Conexion.
 * @return el entero que representa el Socket
 */
int create_tcp_socket(){
    int sock;
    if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("No se pudo crear el socket TCP");
        exit(1);
    }
    return sock;
}
 
/**
 * Funcion Privada.
 * Consigue la IP asociada al Host.
 * @param host String que representa el host al que se desea conseguir su ip 
 * @return La direccion IP asociada al host
 */
char *get_ip(char *host){
    struct hostent *hent;
    int iplen = 15;       //XXX.XXX.XXX.XXX
    char *ip = (char *)malloc(iplen+1);//+1 por el NULL
    
    if(ip==NULL)
    {
        perror("No Hay Memoria Disponible para el ip");
        exit(1);
    }
	
    memset(ip, 0, iplen+1);//llena el espacio con NULL
    
    if((hent = gethostbyname(host)) == NULL)
    {
        herror("No se Consigue IP");
        exit(1);
    }
	
	
    if(inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL)
    {
        perror("No se puede Identificar el host");
        exit(1);
    }
	
    return ip;
}
 
/**
 * Funcion Privada.
 * Construlle la solicitud del GET al servidor.
 * @param host String que representa el host al que se desea conseguir su ip 
 * @param page Ruta dentro del Host donde se quiere Acceder
 * @return La solicitud que se hara al Servidor
 */
char *build_get_query(char *host, char *page)
{
    char *query;
    char *getpage = page;
    char *tpl = "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
	
    if(getpage[0] == '/'){
        getpage = getpage + 1;
        fprintf(stderr,"Elimine el \"/\", Convierta %s en %s\n", page, getpage);
    }
	
    // -5 es por los %s %s %s en tpl y el \0 del final
    query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);
	
    if(query==NULL)
    {
        perror("No Hay Espacio Disponible para la asignacion del query");
        exit(1);
    }
	
    sprintf(query, tpl, getpage, host, USERAGENT);
    
    return query;
}

//
/**
 * Funcion Publica.
 * Realiza la conexion con el servidor, los requerimientos y termina la conexion
 * @param host La Direccion a la que se quiere Acceder
 * @param page La ruta dentro del Host al que se quiere Acceder
 * @return La Respuesta del Servidor
 */
char* Verific_URL(char * host, char * page)
{
    struct sockaddr_in *remote;
    int sock;
    int tmpres;
    char *ip;
    char *get;
    char buf[BUFSIZ+1];

 
    char *Res =(char *) malloc (sizeof(char)*1024*4) ; // Se asume un Tamaño maximo de 1024 caracteres para la respuesta del Servidor
    strcpy (Res,"");
	


    sock = create_tcp_socket();
    ip = get_ip(host);

  
    remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
  
    if(remote==NULL){
        perror("No Hay Espacio Disponible para el sockaddr_in");
        exit(1);
    }
	
    remote->sin_family = AF_INET;
	
	
    tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
    
    if( tmpres < 0)
    {
        perror("La direccion especifica no es soportada.");
        exit(1);
    }else if(tmpres == 0)
    {
        fprintf(stderr, "%s No es una Direccion IP Valida\n", ip);
        exit(1);
    }
	
    //Si tmpres > 0 Continua (No hay errores)
	
	
    remote->sin_port = htons(PORT);//Puerto 80 que es el html por defecto
 
    if(connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0){
        perror("No se puede establecer Conexion");
        exit(1);
    }
	
    get = build_get_query(host, page); // en get esta el query de GET HTML

    //Enviar el Query al Servidor
    int sent = 0;
    while(sent < strlen(get))
    {  
        tmpres = send(sock, get+sent, strlen(get)-sent, 0);
        if(tmpres == -1){
            perror("Error al enviar el query de GET");
            exit(1);
        }
        sent += tmpres;
    }

    //Recibe la Respuesta del Servidor
    memset(buf, 0, sizeof(buf));//Limpia el Buffer
    int htmlstart = 0;
    char * htmlcontent;
    while((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0){
        if(htmlstart == 0)
        {
            htmlcontent = strstr(buf, "\r\n\r\n");
            if(htmlcontent != NULL){
                htmlstart = 1;
                htmlcontent += 4;
            }
        }else{
            htmlcontent = buf;
        }
		
        if(htmlstart){
		    strcat (Res,htmlcontent);
        }
 
        memset(buf, 0, tmpres);//Limpia el Buffer
    }
		
    if(tmpres < 0)
    {
        perror("Error al recibir la Respuesta del Servidor");
		exit(1);
    }
	
    //Libera la Memoria Ocupada
    free(get);
    free(remote);
    free(ip);
    close(sock);
	
    return Res;
}

