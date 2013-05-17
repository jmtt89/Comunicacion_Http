Comunicacion_http
==================
Proyecto 1 de Redes
---------------------
El proyecto consiste en verificar modificaciones, creaciones o eliminaciones de archivos que estan ubicados en internet y que se accesan por el protocolo Http

Estos Archivos se agregan a una lista de directorios a ser verificados, y cada vez que hay algun tipo de modificacion en el directorio la refleja el programa por consola

Funciona tanto en Windows como en Linux.

Desarrollo
----------

Fue implementado en C, mediante Sockets y protocolo HTTP para la comunicacion

Systaxis
--------

>> verific [-t <num_segundos>][-d <directorio>][-a <archivo>]

-t <num_segundos> : numero de segundos que deben transcurrir entre dos revisiones consecutivas de los directorios monitorizados.
-d <directorio> : a direccion absoluta de un directorio que se desea monitorizar, para ver si en el ha ocurrido algun cambio desde la ultima vez que se efectuo la revision.
-a <archivo> : archivo en formato de texto plano en el cual cada linea contendra un URL que correspondera a un directorio cuya informacion se desea verificar.

Integrantes
-----------
	Jesus Torres
	Julio Lopez

