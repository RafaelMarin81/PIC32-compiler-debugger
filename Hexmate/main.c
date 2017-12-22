#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hexfile.h"
#include "utils.h"

struct HexFile {
	char 			*fname;
	unsigned int 	addrLo;
	unsigned int 	addrHi;
	unsigned char	*buffer;
	unsigned int 	size;
	struct HexFile  *next;
};

typedef struct HexFile HexFile;

//=================================================================================================

/**
 * Libera la memoria de la lista de ficheros hex.
 * 
 * @param list			La lista.
 */
void freeFileList(HexFile *list) {
	HexFile *p, *q;
	
	p = list;
	q = p;
	
	while (p != NULL) {
		p = p->next;
		
		if (q->buffer != NULL)
			free(q->buffer);
		
		free(q);
		q = p;
	}
	list = NULL;
}

//=================================================================================================

int main (int argc, char **argv) {
	int i, index;
	unsigned int dataLen;
	char cad[32];
	char *outputFile = NULL;
	HexFile *listFiles = NULL;
	HexFile *currentFile, *node, *list = NULL;

//	printf("Numero de argumentos = %d\n", argc);
//	for (i = 0; i < argc; i++) {
//		printf("argumento[%d] = \"%s\"\n", i, argv[i]);
//	}
	
	//Se necesitan al menos 4 argumentos como minimo de entrada para poder ejecutar la apliacación
	if (argc < 4) {
		printf("Crea fichero .hex a partir de otros dados, extrayendo la informacion que interese al usuario de cada uno de ellos\n");
		printf("USO: hexmate [fichero1.hex] [-rRange] [fichero2.hex] [-rRange]... [ficheroN.hex] [-rRange] -oFicheroSalida.hex\n");
		printf("[ficheroN.hex] Ficheros de entrada\n");
		printf("[range] Rango de direcciones a leer del fichero en hexadecimal, por ejemplo 1D0000-1DFFFF\n");
		printf("-o Especifica el fichero de salida\n");
	} else {
		for (i = 1; i < argc; i++) {
			//se trata de una opción
			if (argv[i][0] == '-') {
				index = 0;
				
				if (argv[i][1] == 'o') {
					outputFile = (char *)&argv[i][2];
					
				} else if (argv[i][1] == 'r') {
					
					Utils_strGetToken((char *)&argv[i][2], &index, '-', cad, 0);
					currentFile->addrLo = Utils_asciiHexToInt(cad);
					Utils_strGetToken((char *)&argv[i][2], &index, '-', cad, 0);
					currentFile->addrHi = Utils_asciiHexToInt(cad);
					
					if (currentFile->addrHi <= currentFile->addrLo) {
						printf("Fichero \"%s\", el rango inferior no puede ser mayor que el superior, saliendo...\n", currentFile->fname);
						return -1;
					}
					currentFile->size = (currentFile->addrHi - currentFile->addrLo) + 1;
					currentFile->buffer = (unsigned char *)malloc(currentFile->size);
					
					if (currentFile->buffer == NULL) {
						printf("Sin memoria, saliendo...\n");
						freeFileList(listFiles);
						return -1;						
					}
				}
			} else {	//se trata de un fichero
				node = (HexFile *)malloc(sizeof(HexFile));
				if (node == NULL) {
					printf("Sin memoria, saliendo...\n");
					freeFileList(listFiles);
					return -1;						
				}
				
				if (list == NULL) {
					list = node;
					currentFile = node;
				} else {
					currentFile->next = node;
					currentFile = node;
				}
				
				node->next = NULL;
				node->fname = argv[i];
				node->addrLo = 0;
				node->addrHi = 0;				
			}
		} // fin for
		
		// Procesamos todos los ficheros
		currentFile = list;
		
		// Inicializamos escritura en fichero de salida
		HexFile_initWrite();
		
		while (currentFile != NULL) {
			// Rellenamos con 0xFF el buffer
			memset(currentFile->buffer, 0xFF, currentFile->size);
			
			if (HexFile_read(currentFile->fname, currentFile->addrLo, currentFile->addrHi, currentFile->buffer, &dataLen) != HEX_PROCESO_OK) {
				printf("No se puede leer el fichero \"%s\"\n", currentFile->fname);
				freeFileList(listFiles);
				return -1;
			}
			
			//Actualizamos tamaño del fichero
			currentFile->size = dataLen;
			
			printf("hexmate -> Escribir del fichero \"%s\" al fichero \"%s\", %u bytes, rango direcciones 0x%08X-0x%08X\n", currentFile->fname, 
				   outputFile, currentFile->size, currentFile->addrLo, currentFile->addrLo + currentFile->size-1);
			
			if (currentFile->next == NULL)
				HexFile_write(outputFile, currentFile->addrLo, currentFile->size, currentFile->buffer, 1);
			else
				HexFile_write(outputFile, currentFile->addrLo, currentFile->size, currentFile->buffer, 0);
			
			currentFile = currentFile->next;
		}
		
		//Libera la lista de ficheros
		freeFileList(listFiles);
	}

	return 0;
}
