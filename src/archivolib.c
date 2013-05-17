/**
 *      @file archivolib.c
 *      @brief Este es la implementacion de una libreria de archivos que
 *      verifica si un archivo ha sido modificado, es nuevo o no ha sido tocado
 *      
 *      @author Jesus Torres / Julio Lopez
 *      
 *      @date 19/10/2011
 * 
 */

#include "archivolib.h"

/**
 * Funcion que Dada una Fecha y una Hora consigue un Entero unico que representa
 * esta lo consigue desglozando la fecha y la hora en sus componentes y operando
 * sobre estas
 * @param Date Fecha en formato DD-MES-AÑO
 * @param Hour Hora en formato HH:MM
 * @return Un Entero que representa esta fecha univocamente
 */
int Convert(char * Date, char *Hour)
{
    
    char Str[strlen(Date)+1];
    memcpy(Str,Date,strlen(Date)+1);// Se Utiliza una Copia para no destruir los originales

    char Str2[strlen(Hour)+1];
    memcpy(Str2,Hour,strlen(Hour)+1);// Se Utiliza una Copia para no destruir los originales
    
     int Aux[5]; //Arreglo en que se Guardan Las partes convertidas a numero para su posterior suma
     
     //Dia(X100)
     Aux[0]= atoi(strtok (Str," -")) * 100;
     
     //Mes(x1000)
     char *Mes = strtok (NULL," -");
     
     int Ms = 0;
     
     //Identifica el Valor Numerico de cada mes
     if(strcmp(Mes,"Ene")==0)
         Ms = 1;
     if(strcmp(Mes,"Feb")==0)
         Ms = 2;
     if(strcmp(Mes,"Mar")==0)
         Ms = 3;
     if(strcmp(Mes,"Abr")==0)
         Ms = 4;
     if(strcmp(Mes,"May")==0)
         Ms = 5;
     if(strcmp(Mes,"Jun")==0)
         Ms = 6;
     if(strcmp(Mes,"Jul")==0)
         Ms = 7;
     if(strcmp(Mes,"Ago")==0)
         Ms = 8;
     if(strcmp(Mes,"Sep")==0)
         Ms = 9; 
     if(strcmp(Mes,"Oct")==0)
         Ms = 10;
     if(strcmp(Mes,"Nov")==0)
         Ms = 11;
     if(strcmp(Mes,"Dic")==0)
         Ms = 12;     
   
     Aux[1]= Ms * 1000;

     
     //Ano(X10000)
     Aux[2]= atoi(strtok (NULL," -")) * 10000;

     
     //Horas (X10)
     Aux[3]= atoi(strtok (Str2,":")) * 10;

     
     //Minutos (X1)
     Aux[4]= atoi(strtok (NULL,":")) * 1;
     
     //Se Consigue el Valor Unico
     int i;
     int Sum=0;
     for(i=0;i<5;i++)
     {
          Sum += Aux[i];
     }
     
     return Sum;
  
}

/**
 * Esta funcion Agrega un Elemento a una lista, si el elemento existe verifica
 * si ha sido actualizado, de ser asi actualiza el entero unico que representa
 * la fecha de actualizacion e imprime por pantalla si es nuevo o hay
 * alguna modificacion
 * @param Li Lista en la que se Agregara/Modificara el elemento
 * @param Nombre Nombre del Elemento que se Agregara/Modificara
 * @param Date   Fecha del Elemento que se Agregara/Modificara
 * @param Hour   Hora del Elemento que se Agregara/Modificara
 * @return La lista con el elemento Agregado/Modificado
 */
Lista* agregar(Lista *Li,char *Nombre,char *Date, char *Hour)
{
    Lista *Aux = Li; // Se pone un Apuntador al inicio de la lista
    
    // Revisar si el Archivo ya se encuentra
    while(Aux != NULL)
    {
        if(strcmp(Aux->Archivo,Nombre)==0)
        {
            int tmp = Convert(Date,Hour);
            if(Aux->Tiempo==tmp)     // No hay Actualizaciones
                printf("%s\n",Nombre);
            else if(Aux->Tiempo<tmp) // Si hay Actualizaciones
            {
                Aux->Tiempo=tmp;
                printf("**Actualizado** %s\n",Nombre);
            }
            return Li;
        }
        Aux = Aux->Siguiente;
    }
    
    // Si Hay un Nuevo Archivo
    Archivo *nuevo = (Archivo *)malloc(sizeof(Archivo));
    
    if(nuevo==NULL){
        perror("No Hay Espacio Disponible para la Lista");
        exit(1);
    }
    
    nuevo->Archivo = (char *)malloc (sizeof(Nombre));
    
    if(nuevo->Archivo==NULL){
        perror("No Hay Espacio Disponible para la Lista");
        exit(1);
    }
    
    strcpy(nuevo->Archivo, Nombre);
    nuevo->Tiempo = Convert(Date,Hour);
    nuevo->Siguiente = Li;
    
    printf("**NUEVO** %s\n",nuevo->Archivo);
    
    return nuevo;
}

/**
 * Se Imprime el Contenido de una lista
 * @param Li Lista que se desea Imprimir
 */
void Imprimir(Lista *Li)
{
    Lista *Aux = Li;
    while(Aux != NULL)
    {
        printf("%s\n%d\n",Aux->Archivo,Aux->Tiempo);
        Aux = Aux->Siguiente;
    }
    
}
