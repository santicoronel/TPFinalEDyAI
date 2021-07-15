#ifndef __ARCHIVOS_H__
#define __ARCHIVOS_H__

#include <stdio.h>

#include "contacto.h"

typedef enum {
  COMA,
  SALTO_LINEA,
  FINAL,
  ERROR_EOF,
  ERROR_LARGO,
  ERROR_FORMATO,
} Lectura;

Lectura leer_palabra(FILE* fp, int strlen, char* palabra);
Lectura leer_numero(FILE* fp, unsigned int* n);
void escribir_cabecera(FILE* fp);
void escribir_contacto(Contacto contacto, FILE* fp);

#endif /* __ARCHIVOS_H__ */