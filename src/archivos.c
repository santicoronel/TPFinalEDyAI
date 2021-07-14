#include "archivos.h"

#include <stdlib.h>
#include <assert.h>

Resultado leer_palabra(FILE* fp, int strlen, char* palabra) { 
  int i, c = fgetc(fp);
  for (i = 0; (i < strlen) && (c != ',') && (c != '\n'); i++) {
    if (c == EOF) return i == 0 ? FINAL : ERROR_EOF; 
    palabra[i] = c;
    c = fgetc(fp);
  }
  if (i >= strlen) return ERROR_LARGO;
  palabra[i] = '\0';
  return c == ',' ? COMA : SALTO_LINEA; 
}

Resultado leer_numero(FILE* fp, unsigned int* n) {
  if (feof(fp)) return ERROR_EOF;
  fscanf(fp, "%u", n);
  char c = fgetc(fp);
  switch (c) {
  case ',':
    return COMA;
    break;
  case '\n':
    return SALTO_LINEA;
    break;
  case EOF:
    return ERROR_EOF;
  default:
    return ERROR_FORMATO;
  }
}

void escribir_cabecera(FILE* fp) {
  fprintf(fp, "nombre,apellido,edad,telefono\n");
}

void escribir_contacto(FILE* fp, Contacto contacto) {
  fprintf(fp, "%s,%s,%u,%s\n", 
    contacto->nombre, contacto->apellido, contacto->edad, contacto->telefono);
}