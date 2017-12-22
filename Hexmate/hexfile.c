#include "hexfile.h"
#include <string.h>
#include <stdio.h>
#include "utils.h"

// Tipo de registro
#define HEX_DATA_RECORD 			0x00
#define HEX_END_OF_FILE_RECORD 		0x01
#define HEX_EXTEND_ADDRESS_RECORD	0x04

//Indice donde comienzan los datos de un registro
#define HEX_DATA_INIT				9 

unsigned short HexFile_lastAddress = 0;
unsigned char HexFile_isFirstAddress = 1;

//=================================================================================================

/**
 * Lee un fichero hex.
 * 
 * @param fname			El nombre del fichero a leer.
 * @param addrLow		Dirección de inicio de lectura.
 * @param addrHi		Dirección de fin de lectura.
 * @param buffer		Buffer donde almacenar los datos.
 * @return			Código de error.
 */
unsigned char HexFile_read(char *fname, unsigned int addrLow, unsigned int addrHi, unsigned char *buffer, unsigned int *len) {
	FILE *f;
	char hex_line[256];
	char number[8];
	unsigned int hex_address_offset, hex_init_address;
	unsigned short hex_chkSum, hex_dataLen;
	unsigned char hex_regType, hex_dataByte;
	int i;
	
	f  = fopen(fname, "r");
	
	if (f != NULL) {
		hex_address_offset = 0;
		*len = 0;
		
		while (!feof(f)) {
			fscanf(f, "%s\n", hex_line);
			
			if (hex_line[0] == ':') {
				hex_chkSum = 0;
			
				//printf("%s\n", hex_line);
				
				//Longitud de los datos
				strncpy(number, (char *)&hex_line[1], 2);
				number[2] = '\0';
				hex_dataLen = (unsigned short)Utils_asciiHexToInt(number);
				hex_chkSum += hex_dataLen;
				
				//Dirección de inicio
				strncpy(number, (char *)&hex_line[3], 4);
				number[4] = '\0';
				hex_init_address = (unsigned int)Utils_asciiHexToInt(number);
				
				strncpy(number, (char *)&hex_line[3], 2);
				number[2] = '\0';
				hex_chkSum += (unsigned short)Utils_asciiHexToInt(number);
				
				strncpy(number, (char *)&hex_line[5], 2);
				number[2] = '\0';
				hex_chkSum += (unsigned short)Utils_asciiHexToInt(number);
				
				//Calculamos dirección real
				hex_init_address += hex_address_offset;
				
				strncpy(number, (char *)&hex_line[7], 2);
				number[2] = '\0';
				hex_regType = (unsigned char)Utils_asciiHexToInt(number);
				
				hex_chkSum += hex_regType;
				
				switch (hex_regType) {
					case HEX_DATA_RECORD:
						if (hex_dataLen > 0) {
							for (i = 0; i < hex_dataLen; i++) {
								strncpy(number, (char *)&hex_line[HEX_DATA_INIT + (i*2)], 2);
								number[2] = '\0';
								hex_dataByte = (unsigned char)Utils_asciiHexToInt(number);
								
								// Si los datos están dentro del rango especificado los guardamos en el buffer
								if (((hex_init_address + i) >= addrLow) && ((hex_init_address + i) <= addrHi)) {
									buffer[(hex_init_address + i) - addrLow] = hex_dataByte;
									
									if (((hex_init_address - addrLow) + hex_dataLen) > *len)
                                       *len = (hex_init_address - addrLow) + hex_dataLen;
								}
								hex_chkSum += hex_dataByte;
							} // fin for
						} // fin if
						break;
					case HEX_END_OF_FILE_RECORD:
						hex_chkSum = hex_chkSum;
						break;
					case HEX_EXTEND_ADDRESS_RECORD:
						strncpy(number, (char *)&hex_line[HEX_DATA_INIT], 4);
						number[4] = '\0';
						hex_address_offset = (unsigned short)Utils_asciiHexToInt(number);
						
						strncpy(number, (char *)&hex_line[HEX_DATA_INIT], 2);
						number[2] = '\0';
						hex_chkSum += (unsigned short)Utils_asciiHexToInt(number);

						strncpy(number, (char *)&hex_line[HEX_DATA_INIT + 2], 2);
						number[2] = '\0';
						hex_chkSum += (unsigned short)Utils_asciiHexToInt(number);
						
						hex_address_offset *= 0x10000;
						hex_dataLen = 2;
						break;
				} // fin switch
				
				// Calculamos el CRC definitivo y lo comprobamos
				hex_chkSum = (0x100 - (hex_chkSum & 0x00FF)) & 0x00FF;
				
				strncpy(number, (char *)&hex_line[HEX_DATA_INIT + (hex_dataLen * 2)], 2);
				number[2] = '\0';
				if (hex_chkSum != (unsigned short)Utils_asciiHexToInt(number)) {
					
					printf("HexFile_read: error CRC leido 0x%s, calculado 0x%x\n", number, hex_chkSum);
					
					fclose(f);
					return HEX_ERROR_CRC;
				}
			} else if (hex_line[0] != ';') {
				fclose(f);
				return HEX_ERROR_INICIO_LINEA;
			}
		}
	}
	
	fclose(f);
	
	return HEX_PROCESO_OK;
}

//=================================================================================================

/**
 * Calcula el checksum a un array de bytes.
 * 
 * @param data			Los datos.
 * @param len			La longitud de los datos.
 * @return				El checksum.
 */
unsigned char HexFile_calcCheckSum(unsigned char *data, int len) {
	int i;
	unsigned int result = 0;
	
	for (i = 0; i < len; i++) {
		result += data[i];
	}
	
	return (unsigned char)(0x0100 - (result & 0x00FF));
}

//=================================================================================================

/**
 * Escribe un registro de dirección extendida en el fichero.
 * 
 * @param f				El fichero donde guardarlo.
 * @param addr			La dirección.
 */
void HexFile_writeExtendedAddressRecord(FILE *f, unsigned int addr) {
	int i;
	unsigned short addrHi;
	unsigned char data[8];
	
	addrHi = (unsigned short)(addr >> 16);
	
	// Si hay cambio de dirección escribimos registro de dirección extendida o bien si hemos comenzado a escribir en el fichero
	if ((addrHi != HexFile_lastAddress) || (HexFile_isFirstAddress == 1)) { //if ((addr % 65536) == 0) {
		data[0] = 0x02;
		data[1] = 0x00;
		data[2] = 0x00;
		data[3] = 0x04;
		data[4] = (unsigned char)((addr & 0xFF000000) >> 24);
		data[5] = (unsigned char)((addr & 0x00FF0000) >> 16);
		data[6] = HexFile_calcCheckSum(data, 6);
		fputc(':', f);
		for (i = 0; i < 7; i++)
			fprintf(f, "%02X", data[i]);
		fprintf(f, "\n");
		
		// Nos guardamos la última dirección escrita
		HexFile_lastAddress = addrHi;
		HexFile_isFirstAddress = 0;
	}
}

//=================================================================================================

/**
 * Escribe un registro de datos.
 * 
 * @param f				El nombre del fichero.
 * @param addr			La dirección del registro.
 * @param data			Los datos.
 * @param len			La longitud de los datos.
 */
void HexFile_writeDataRecord(FILE *f, unsigned int addr, unsigned char *data, int len) {
	unsigned char dataRecord[64];
	int i, j;
	
	dataRecord[0] = (unsigned char)(len & 0x00FF);
	dataRecord[1] = (unsigned char)((addr & 0x0000FF00) >> 8);
	dataRecord[2] = (unsigned char)(addr & 0x000000FF);
	dataRecord[3] = 0x00;
	
	for (i = 0; i < len; i++) {
		dataRecord[4 + i] = data[i];
	}
	
	dataRecord[4 + i] = HexFile_calcCheckSum(dataRecord, 4 + i);
	
	fputc(':', f);
	for (j = 0; j <= (4 + i); j++)
		fprintf(f, "%02X", dataRecord[j]);
	
	fprintf(f, "\n");
}

//=================================================================================================

/**
 * Inicializa la escritura en fichero 
 * Nota: Solo se debe llamar una vez por fichero a escribir y justo antes de comenzar a escribir.
 */
void HexFile_initWrite(void) {
	HexFile_lastAddress = 0;	
	HexFile_isFirstAddress = 1;
}

//=================================================================================================

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
unsigned char HexFile_write(char *fname, unsigned int addr, unsigned int len, unsigned char *data, int endOfFile) {
	FILE *f;
	unsigned int i;
	
	f = fopen(fname, "a");
	
	if (f != NULL) {
		
		i = 0;
		while (len > 0) {
			// Escribimos registro de dirección extendida si es necesario
			HexFile_writeExtendedAddressRecord(f, addr);
			
			if (len >= 16) {
				// Escribimos los datos en bloques de 16 bytes
				HexFile_writeDataRecord(f, addr, (unsigned char *)&data[i], 16);
				len -= 16;
				i += 16;
				addr += 16;
			} else {
				HexFile_writeDataRecord(f, addr, (unsigned char *)&data[i], len);
				i += len;				
				addr += len;
				len = 0;
			}
		}
		
		if (endOfFile) {
			fprintf(f, ":00000001FF\n");
		}
		
		fclose(f);
	} /*else {
		return HEX
	}*/
	
	return HEX_PROCESO_OK;
}

//=================================================================================================
