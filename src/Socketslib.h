/**
 *      @file Socketslib.h
 *      @brief Este es la cabecera de una libreria de sockets
 *      y manejo de conexiones por el protocolo HTML
 *      
 *      @author Jesus Torres / Julio Lopez
 *      
 *      @date 19/10/2011
 * 
 */
#ifndef SOCKETSLIB_H
#define	SOCKETSLIB_H


//Librerias
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

// Constantes
#define HOST "ldc.usb.ve"
#define PAGE "/"
#define PORT 80
#define USERAGENT "HTMLGET 1.1"

//Funciones de Sockets

void usage();

int IsaNumber(int num);

int IsaFile(char *arg);

char* Verific_URL(char * dir, char * path);

#endif	/* SOCKETSLIB_H */
