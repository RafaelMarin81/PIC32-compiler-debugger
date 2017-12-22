#ifndef UTILS_H_
#define UTILS_H_

/**
 * Obtiene una cadena delimitada por un carácter determinado a partir de otra
 *
 * @param src               La cadena fuente de la que deseamos extraer los tokens.
 * @param index             Indice del siguiente token.
 * @param separator         Carácter separador de los tokens.
 * @param token             El token obtenido.
 * @param trimSeparator     Indica si deseamos que se eliminen los caracteres separadores que precedan a un token.
 */
void Utils_strGetToken(char *src, int *index, char separator, char *token, int trimSeparator);

/**
 * Convierte una cadena con digitos hexadecimales a entero
 *
 * @param hex               La cadena a convertir a entero
 * @return                  El número resultado de la cadena convertida
 */
unsigned int Utils_asciiHexToInt(char *hex);

/**
  * Calcula el CRC16-ITT de una trama de datos
  *
  * @param datos            Puntero con los datos a calcular el CRC.
  * @param len              Tamaúo de los datos en bytes.
  * @return                 El checksum.
  */
unsigned short Utils_calcCRC16ITT(unsigned char *datos, unsigned int len);

#endif
