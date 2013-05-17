/**
 *      @file archivolib.h
 *      @brief Este es la cabecera de una libreria de archivos que es la que
 *      verifica si un archivo ha sido actualizado desde la visita anterior
 *      funciona con una lista simplemente enlazada donde se agrega 
 *      el tiempo y el nombre del archivo
 *      
 *      @author Jesus Torres / Julio Lopez
 *      
 *      @date 19/10/2011
 * 
 */

#ifndef ARCHIVOLIB_H
#define	ARCHIVOLIB_H

//Librerias
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

// Estructuras
struct NodoL{
    char *Archivo;
    int Tiempo;
    struct NodoL *Siguiente;
};

typedef struct NodoL Lista;
typedef struct NodoL Archivo;

// Constantes

//Funciones de Lista
Lista *agregar(Lista *Li,char *Nombre,char *Date,char *Hour);
int Convert(char * Date,char *Hour);
void Imprimir(Lista *Li);

#endif	/* ARCHIVOLIB_H */

