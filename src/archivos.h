/** Modulo para el manejo de archivos. */

#ifndef __ARCHIVOS_H__
#define __ARCHIVOS_H__

#include <stdio.h>

#include "contacto.h"

/** 
 * Tipo que representa el ultimo caracter leido antes de terminar la lectura.
 */
typedef enum {
  COMA,
  SALTO_LINEA,
  FINAL,
  ERROR_EOF,
  ERROR_LARGO,
  ERROR_FORMATO,
} Lectura;

/**
 * Lee un string de largo strlen del archivo dado.
 */
Lectura leer_palabra(FILE* fp, int strlen, char* palabra);
/**
 * Lee un entero no negativo del archivo dado.
 */
Lectura leer_numero(FILE* fp, unsigned int* n);
/**
 * Escribe la cabecera en el archivo dado.
 */
void escribir_cabecera(FILE* fp);
/**
 * Escribe el contacto en el archivo dado.
 */
void escribir_contacto(Contacto contacto, FILE* fp);

#endif /* __ARCHIVOS_H__ */