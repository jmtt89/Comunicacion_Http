/**
 *      @file main.c
 *      @brief Este es el main del proyecto
 *      
 *      @author Jesus Torres / Julio Lopez
 *      
 *      @date 19/10/2011
 * 
 */
#include <stdio.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h> 

 
//Librerias de usuario
#include "Socketslib.h"
#include "archivolib.h"

//Variables globales
int t = 30;         // numero de Segundos a esperar Por Defecto
char** arrD = NULL; // Arreglo que guarda todos los URL que seran Verificados
int tam_arrD;       //Longitud del Arreglo de Directorios
int EstadoA=0;      //Booleano que Verifica cuando entra en -d y anula a -a

/**
 * Funcion que espera cierta cantidad de segundos 
 * @param Seconds segundos a esperar por la funci�n
 */
void espera (int Seconds){
  clock_t endwait;
  endwait = clock () + Seconds * CLOCKS_PER_SEC ;
  
  while(clock() < endwait){
  }
}

/**
 * Funcion que parsea la respuesta del servidor y agrega en una lista 
 * los Archivos y su tiempo de comoficacion.
 * @param Pagina el SRC de la Pagina Web que se desea consultar
 * @param Res Lista Con los Resultados anteriormente encontrados en esta pagina
 * @return La Lista con Las Actualizaciones Pertinentes
 */
Lista* Procesar_Respuesta(char *Pagina, Lista *Res)
{
    char tmp1[1024];//Temporal que Guarda el Nombre del Archivo
    char tmp2[12];  //Temporal que Guarda La Fecha de la ultima Actualizacion 
    char tmp3[6];   //Temporal que Guarda LA Hora de la Ultima Actualizacion
    
    char * ptr2; //Este solo funciona para recordar por donde iba el string que esta cortando
    char * ptr = strtok_r (Pagina,"<>",&ptr2); //Lleva la palabra que se esta parsiando
    
    
    while( (ptr = strtok_r( NULL,"<>",&ptr2)) != NULL ){
        
        if(ptr[0]=='a' && strcmp(ptr,"address")!=0){//Localiza los ANCHOR en la pagina
            ptr = strtok_r( NULL,"><",&ptr2);//Consigue el Nombre del Archivo
            memcpy(tmp1,ptr,strlen(ptr)+1);
             
             if(strcmp(ptr,"Name")!=0 && strcmp(ptr,"Last modified")!=0 && strcmp(ptr,"Size")!=0 && strcmp(ptr,"Description")!=0 && strcmp(ptr,"Parent Directory")!=0) //Ignora la Cabezera
             {
                ptr = strtok_r( NULL,">",&ptr2);
                
                ptr = strtok_r( NULL," ",&ptr2);//Consigue la Fecha
                memcpy(tmp2,ptr,strlen(ptr)+1);
                ptr = strtok_r( NULL," ",&ptr2);//Consigue la Hora
                memcpy(tmp3,ptr,strlen(ptr)+1);
                
                Res = agregar(Res,tmp1,tmp2,tmp3); //Se Agrega a la Lista de Resultados
             }
        }
    }
    
    return Res;

}

/**
 * VerificarEntrada, funcion que verifica todos los parametros
 * que se reciben por consola y que inicializa todas las variables
 * para realizar la ejecucion del programa
 * @param argc Cantidad de Argumentos por Consola
 * @param argv Contenido de Cada Argumento por Consola
 * @return EXIT_SUCCESS en caso de que todo este bien,EXIT_FAILURE en caso del algun error
 */
int VerificarEntrada(int argc, char** argv){
    int pos = 1;

    // verifica el numero de argumentos maximos
    if (argc > 7){
        perror("Error en linea de argumentos: \n Mas argumentos de los permitidos \n");
        return(EXIT_FAILURE);
    }

    while(pos < argc){

        //Verificar El Tiempo de Espera
        if(!strcmp(argv[pos], "-t")){
            t = atoi(argv[++pos]);
            if (!IsaNumber(t)){
                return(EXIT_FAILURE);
            }
            if(t < 0){
                perror("Error en linea de argumentos \n n debe ser mayor que 0");
                return(EXIT_FAILURE);
            }
            pos = pos+1;
            continue;
        }

        //Verificamos el Directorio
        if(!strcmp(argv[pos],"-d")){
            
            char* dir = argv[++pos];

            tam_arrD = 1;
            arrD = (char **)malloc(sizeof(char*) * tam_arrD);
            arrD[0] = (char *)malloc(strlen(dir)+1);
            strcpy (arrD[0],dir);
            
            EstadoA = 1;
            pos = pos+1;
            continue;
        }

        //Verificamos El Archivo de Directorios
        if(!strcmp(argv[pos],"-a")){
            char* arch = argv[++pos];
            if(!EstadoA)
            {

                if (!IsaFile(arch)){
                    return(EXIT_FAILURE);
                }

                FILE *archivo1;
                FILE *archivo2;
                archivo1 = fopen(arch,"r");
                archivo2 = fopen(arch,"r");

                if(archivo1 == NULL){
                    perror("Error fopen, No se puede Abrir el Archivo");
                    return(EXIT_FAILURE);
                }

                char str[500];
                tam_arrD = 0;
                while((fscanf(archivo1, "%s", &str) != EOF)){
                    tam_arrD++;
                }

                arrD = (char **)malloc(sizeof(char*) * tam_arrD);

                int i;
                for(i=0;i<tam_arrD;i++){
                    fscanf(archivo2, "%s", &str);
                    arrD[i] = (char *)malloc(sizeof(char) * 500);
                    strcpy (arrD[i],str);
                }

                if(fclose(archivo1)){
                    perror("Error fclose,Error en cerrar el Archivo");
                    return(EXIT_FAILURE);
                }
            }
            pos = pos+1;
            continue;
        }

        //Default
        perror("Error Con la Sintaxis");
        return(EXIT_FAILURE);
    }
    return(EXIT_SUCCESS);
}

/**
 * Funcion que obtiene y almacena los URLs y hace las llamadas respectivas para su procesamiento
 * para obtener la informaci�n de los directorios alojados en el servidor
 * @param argc Cantidad de Argumentos del main()
 * @param argv Contenido de Cada Argumento del main()
 * @return EXIT_SUCCESS en caso de que todo este bien,EXIT_FAILURE en caso del algun error
 */
void RevisarDir(Lista *Res[]){

    char *url;
    char *t1;
    char *path;
    char *protocolo;
    int i;
    
    for(i=0;i<tam_arrD;i++)
    {	
		printf("%s\n",arrD[i]);
        char aux[strlen(arrD[i])+1];
        memcpy(aux,arrD[i],strlen(arrD[i])+1);
        //Se separa el URL en protocolo, ip y path
        url = aux;
        t1 = strtok(url,"://");//obtengo el protocolo
        protocolo = t1;
        t1 = strtok(NULL, "//");//obtengo el URL
        url = t1;
        t1 = strtok(NULL, "");//obtengo el path
        path = t1;
        Res[i] = Procesar_Respuesta(Verific_URL(url,path),Res[i]);
    }
}


/**
 * Funcion principal que se llama en la Corrida del Programa
 * @param argc Cantidad de Argumentos por Consola
 * @param argv Contenido de Cada Argumento por Consola
 * @return EXIT_SUCCESS en caso de que todo este bien,EXIT_FAILURE en caso del algun error
 */
int main(int argc, char** argv) {
    
    int h = 0;
    int cic = 0;
    int u = 0;
	int k = 0;
	int pid;
	char cara = 'j';
	int pc = 0;
	
    if(VerificarEntrada(argc,argv)==EXIT_FAILURE){
        perror("Error en la Sintaxys");
        exit(EXIT_FAILURE);
    }
    	
    Lista *Res[tam_arrD];
        
    for(k=0;k<tam_arrD;k++){
        Res[k]=NULL;
    }
	
	if ((pid=fork()) == -1) { 
		printf ("error en creacion de proceso hijo\n"); 
		exit(1); 
	} 
	else {
		
		while(1){
			if(pid != 0){ 
				cara = getchar();

				if(cara=='s'){
				printf("Salida\n");
				 exit(0);
				}
				if(cara=='c'){
				printf("Continua\n");
				kill(pid,SIGCONT);
				}
				if(cara=='p'){
				printf("Pausado\n");
				kill(pid,SIGSTOP);
				}
			}
			else{
				for (h=0;h<t;h++){		
					cic++;
					if(h==t-1){
						cic=0;
						RevisarDir(Res);
						printf("\n");
					}
					espera(1);
				}
			}
		}		 
	}
     
	 return u;
}
