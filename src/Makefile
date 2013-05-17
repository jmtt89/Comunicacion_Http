all: verific

verific: main.o Socketslib.o archivolib.o
	$(CC) main.o Socketslib.o archivolib.o -g -o verific

main.o: main.c Socketslib.h archivolib.h
	$(CC) -c main.c

Socketslib.o: Socketslib.c Socketslib.h
	$(CC) -c Socketslib.c

archivolib.o: archivolib.c archivolib.h
	$(CC) -c archivolib.c

clean:
	rm *.o verific
