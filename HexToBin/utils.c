#include "utils.h"

//=================================================================================================

/**
 * Obtiene una cadena delimitada por un carácter determinado a partir de otra
 *
 * @param src               La cadena fuente de la que deseamos extraer los tokens.
 * @param index             Indice del siguiente token.
 * @param separator         Carácter separador de los tokens.
 * @param token             El token obtenido.
 * @param trimSeparator     Indica si deseamos que se eliminen los caracteres separadores que precedan a un token.
 */
void Utils_strGetToken(char *src, int *index, char separator, char *token, int trimSeparator) {
    char c;
    int i;

    token[0] = '\0';
    i = 0;

    //cadena vacía?
    if (src[*index] == '\0') return;

    //si se desea eliminar los caracteres separadores de tokens, lo hacemos primero
    if (trimSeparator) {
        do {
            c = (char)src[*index];
            *index = *index + 1;
        } while ((c == separator) && (c != '\0'));

        //para apuntar al último caracter distinto del separador
        *index = *index - 1;
    }

    do{
        c = (char)src[*index];
        *index = *index + 1;

        token[i++] = c;
    } while((c != separator)&&(c != '\0'));

    if (c == '\0') //si habiamos alcanzado el null, no podemos incrementar index, pues nos saldriamos del limite de la cadena
        *index = *index - 1;

    token[i-1] = '\0';	//indicamos el final de la cadena
}

//=================================================================================================

/**
 * Convierte una cadena con digitos hexadecimales a entero
 *
 * @param hex               La cadena a convertir a entero
 * @return                  El número resultado de la cadena convertida
 */
unsigned int Utils_asciiHexToInt(char *hex) {
   char c;
   unsigned short index;
   unsigned int result;

   index = 0; result = 0;
   c = hex[index++];

   while(c != '\0') {
      if ((c >= '0') && (c <= '9')) {
    	  result = (result * 16) + (c - 48);
      }
      else if ((c >= 'A') && (c <= 'F')) {
    	  result = (result * 16) + (c - 55);
      }
      else if ((c >= 'a') && (c <= 'f')) {
    	  result = (result * 16) + (c - 87);
      }
      else {
    	  //caracter ilegal
    	  return 0;
      }
      c = hex[index++];
   }
   return result;
}

//=================================================================================================

/**
  * Calcula el CRC16-ITT de una trama de datos
  *
  * @param datos            Puntero con los datos a calcular el CRC.
  * @param len              Tamaúo de los datos en bytes.
  * @return                 El checksum.
  */
unsigned short Utils_calcCRC16ITT(unsigned char *datos, unsigned int len) {
     unsigned short crc;
     unsigned int i;
     unsigned char j;
     unsigned char bitValue;
     unsigned char c15;
     unsigned char data;
     
     crc = 0xFFFF;

     for(i = 0; i < len; i++) {
         data = *datos++;

         for(j = 0; j < 8; j++) {
             bitValue = ((data >> (7 - j)) & 0x01);
             c15 = ((crc >> 15) & 0x0001);
             crc = crc << 1;

             if ((c15 ^ bitValue) == 1) {
                 crc = crc ^ 0x1021;
             }
         }
     }
     
     return crc;
}

//=================================================================================================

