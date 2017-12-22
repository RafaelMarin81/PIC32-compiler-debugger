#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hexfile.h"
#include "utils.h"

#define OFFSET_FIRMWARE_VERSION		0x00020000

struct HexFile {
	char 			*fname;
	unsigned int 	addrLo;
	unsigned int 	addrHi;
	unsigned char	*buffer;
	unsigned int 	size;
	struct HexFile  *next;
};

typedef struct HexFile HexFile;

typedef struct {
		unsigned short version;
		unsigned short options;
		unsigned short crc;
} __attribute__ ((packed)) FileHeader;

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
	FILE *f;
	int includeHeader = 0;
	FileHeader header;
	
//	printf("Numero de argumentos = %d\n", argc);
//	for (i = 0; i < argc; i++) {
//		printf("argumento[%d] = \"%s\"\n", i, argv[i]);
//	}
	
	//Se necesitan al menos 4 argumentos como minimo de entrada para poder ejecutar la apliacación
	if ((argc < 4) || (argc > 5)) {
		printf("Crea fichero .bin a partir de un fichero .hex dado\n");
		printf("USO: hex2bin [fichero.hex] [-rRange] [-hOptions] -oFicheroSalida.bin\n");
		printf("[fichero.hex] Fichero de entrada\n");
		printf("[range] Rango de direcciones a leer del fichero en hexadecimal, por ejemplo 1D0000-1DFFFF\n");
		printf("[Options] Incluir cabecera con alguna(s operacion OR) de las las opciones siguientes:\n");
		printf("\t\tSET FIRMWARE AS FACTORY = 0x0001\n");
		printf("\t\tCLEAR CONFIGURATION 	= 0x0100\n");
		printf("-o Especifica el fichero de salida\n");
	} else if (argc >= 4) {
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
				} else if (argv[i][1] == 'h') {	// Incluir cabecera
					Utils_strGetToken((char *)&argv[i][2], &index, '\0', cad, 0);
					header.options = (unsigned short)Utils_asciiHexToInt(cad);
					printf("Opciones = 0x%04X\n", header.options);
					includeHeader = 1;
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
		
		// Procesamos todos los ficheros (en realidad será uno solamente)
		currentFile = list;
				
		if (currentFile != NULL) {
			// Rellenamos con 0xFF el buffer
			memset(currentFile->buffer, 0xFF, currentFile->size);
			
			if (HexFile_read(currentFile->fname, currentFile->addrLo, currentFile->addrHi, currentFile->buffer, &dataLen) != HEX_PROCESO_OK) {
				printf("No se puede leer el fichero \"%s\"\n", currentFile->fname);
				freeFileList(listFiles);
				return -1;
			}
			
			//Actualizamos tamaño del fichero
			currentFile->size = dataLen;
			
			printf("hex2bin -> Escribir del fichero \"%s\" al fichero \"%s\", %u bytes, rango direcciones 0x%08X-0x%08X\n", currentFile->fname, 
				   outputFile, currentFile->size, currentFile->addrLo, currentFile->addrLo + currentFile->size-1);
			
			f = fopen(outputFile, "w");
			if (f != NULL) {
				if (includeHeader == 1) {
					// Extraemos la versión del firmware
					if (currentFile->size <= OFFSET_FIRMWARE_VERSION) {
						header.version = 0;	
					} else {
						header.version = (currentFile->buffer[OFFSET_FIRMWARE_VERSION + 1] << 8) | currentFile->buffer[OFFSET_FIRMWARE_VERSION];
					}
					printf("Firmware version = 0x%04X\n", header.version);
					header.crc = Utils_calcCRC16ITT((unsigned char *)&header, sizeof(FileHeader) - sizeof(unsigned short));
					
					fwrite(&header, 1, sizeof(FileHeader), f);
				}
				fwrite(currentFile->buffer, 1, currentFile->size, f);
				fclose(f);
			}				
		}
		
		//Libera la lista de ficheros
		freeFileList(listFiles);
	} 

	return 0;
}
