#ifndef HEX_FILE_H_
#define HEX_FILE_H_

// Códigos de error de procesado de ficheros hex
#define HEX_PROCESO_OK 					0
#define HEX_ERROR_INICIO_LINEA 			1
#define HEX_ERROR_CRC 					2
#define HEX_ERROR_DATOS_IMPAR 			3
#define HEX_ERROR_DIRECCION_INVALIDA	4
#define HEX_CANCELADO_USUARIO 			5

//=================================================================================================

/**
 * Lee un fichero hex.
 * 
 * @param fname			El nombre del fichero a leer.
 * @param addrLow		Dirección de inicio de lectura.
 * @param addrHi		Dirección de fin de lectura.
 * @param buffer		Buffer donde almacenar los datos.
 * @return				Código de error.
 */
unsigned char HexFile_read(char *fname, unsigned int addrLow, unsigned int addrHi, unsigned char *buffer, unsigned int *len);


/**
 * Inicializa la escritura en fichero 
 * Nota: Solo se debe llamar una vez por fichero a escribir y justo antes de comenzar a escribir.
 */
void HexFile_initWrite(void);

/**
 * Escribe en el fichero hex especificado los datos que se le pasan como parámetro.
 * 
 * @param fname			El nombre del fichero donde escribir los datos.
 * @param addr			Dirección de inicio de los datos.
 * @param len			Tamaño de los datos a escribir.
 * @param data			Los datos.
 * @param endOfFile		Indica si hay que escribir el registro de fin de fichero.
 * @return				Código de error.
 */
unsigned char HexFile_write(char *fname, unsigned int addr, unsigned int len, unsigned char *data, int endOfFile);

//=================================================================================================

#endif
