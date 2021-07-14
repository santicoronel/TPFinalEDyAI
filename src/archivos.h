#ifndef __ARCHIVOS_H__
#define __ARCHIVOS_H__

#include <stdio.h>

typedef enum {
  COMA,
  SALTO_LINEA,
  FINAL,
  ERROR_EOF,
  ERROR_LARGO,
  ERROR_FORMATO,
} Resultado;

Resultado leer_palabra(FILE* fp, int strlen, char* palabra);
Resultado leer_numero(FILE* fp, unsigned int* n);

#endif /* __ARCHIVOS_H__ */