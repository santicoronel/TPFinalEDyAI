#include "operaciones.h"

#include <string.h>

#include "contacto.h"
#include "archivos.h"
#include "historial.h"

#define STRLEN 50

Resultado buscar(Entorno entorno) {
  printf("Ingrese nombre:\n>");
  char nombre[STRLEN]; fgets(nombre, STRLEN, stdin);
  printf("Ingrese nombre:\n>");
  char apellido[STRLEN]; fgets(apellido, STRLEN, stdin);
  char* nombre_apellido[2] = {nombre, apellido};
  Contacto res = tablahash_buscar(entorno.tabla, nombre_apellido);
  free(nombre); free(apellido);
  if (res) contacto_imprimir(res); 
  else return BUSCAR_EXISTE;

  return EXITO;
}

Resultado agregar(Entorno entorno) {
  char* nombre = malloc(sizeof(STRLEN)); assert(nombre);
  printf("Ingrese nombre:\n>");
  fgets(nombre, STRLEN, stdin);
  char* apellido = malloc(sizeof(STRLEN)); assert(apellido);
  printf("Ingrese apellido:\n>");
  fgets(apellido, STRLEN, stdin);
  char* nombre_apellido[2] = {nombre, apellido};
  if (tablahash_buscar(entorno.tabla, nombre_apellido))
    return AGREGAR_EXISTE;

  unsigned int edad;
  printf("Ingrese edad:\n>");
  scanf("%u", &edad);
  char* telefono = malloc(sizeof(STRLEN)); assert(telefono);
  printf("Ingrese telefono:\n>");
  fgets(telefono, STRLEN, stdin);
  
  Contacto contacto = contacto_crear(nombre, apellido, edad, telefono);
  tablahash_insertar(entorno.tabla, contacto);
  
  return EXITO;
}

Resultado eliminar(Entorno entorno) {
  printf("Ingrese nombre:\n>");
  char nombre[STRLEN]; fgets(nombre, STRLEN, stdin);
  printf("Ingrese apellido:\n>");
  char apellido[STRLEN]; fgets(apellido, STRLEN, stdin);
  char* nombre_apellido[2] = {nombre, apellido};
  Contacto eliminado = tablahash_eliminar(entorno.tabla, nombre_apellido);
  if (eliminado) { /*TODO: agregar a historial*/ }
  else return ELIMINAR_EXISTE;

  return EXITO;
}

Resultado editar(Entorno entorno) {
  printf("Ingrese nombre:\n>");
  char nombre[STRLEN]; fgets(nombre, STRLEN, stdin);
  printf("Ingrese apellido:\n>");
  char apellido[STRLEN]; fgets(apellido, STRLEN, stdin);
  char* nombre_apellido[2] = {nombre, apellido};
  Contacto contacto = tablahash_buscar(entorno.tabla, nombre_apellido);
  if (contacto) {
    // TODO: crear contacto nuevo
    printf("Ingrese edad:\n>");
    scanf("%u", &contacto->edad);
    printf("Ingrese nombre:\n>");
    fgets(contacto->telefono, STRLEN, stdin);
  }
  else return EDITAR_EXISTE;

  return EXITO;
}

static Resultado cabecera_valida(FILE* fp) {
  char keywords[] = {"nombre", "apellido", "edad", "telefono"};
  char input[STRLEN];
  for (int i = 0; i < 4; i++) {
    switch (leer_palabra(fp, STRLEN, input)) {
    case COMA:
      if (i == 3) return CARGAR_COMA;
      if ((strcmp(input, keywords[i]) != 0)) return CARGAR_CABECERA;
      break;
    case SALTO_LINEA:
      if (i != 3) return CARGAR_SALTO_LINEA;
      if ((strcmp(input, keywords[i]) != 0)) return CARGAR_CABECERA;
      break;
    case FINAL:
      if (i == 0) return CARGAR_VACIO;
      //fallthrough
    case ERROR_EOF:
      return CARGAR_EOF;
      break;
    case ERROR_LARGO:
      return CARGAR_LARGO;
      break;
    case ERROR_FORMATO:
      return CARGAR_FORMATO;
      break;
    }
  }
  return EXITO;
}

Resultado cargar(Entorno entorno) {
  char ruta[STRLEN];
  printf("Ingrese ruta de entrada:\n>");
  scanf("%s", ruta);
  FILE* fp = fopen(ruta, "r"); assert(fp);
  
  Resultado res = cabecera_valida(fp);
  if (res != EXITO) return res;

  char* input[4] = {NULL, NULL, NULL, NULL}; 
  unsigned int edad;
  int seguir = 1; Lectura lec; 
  while(seguir) {
    for (int i = 0; i < 4; i++) {
      if (i == 2) lec = leer_numero(fp, &edad);
      else {
        input[i] = malloc(STRLEN); assert(input);
        lec = leer_palabra(fp, STRLEN, leer_palabra);
      }
      switch(lec) {
      case COMA:
        if (i == 3) {
          res = CARGAR_COMA; seguir = 0;
        }
        break;
      case SALTO_LINEA:
        if (i != 3) {
          res = CARGAR_SALTO_LINEA; seguir = 0;
        }
        break;
      case FINAL:
        if (i == 0) {
          res = EXITO; seguir = 0; 
          break;
        }
      // falthrough
      case ERROR_EOF:
        res = CARGAR_EOF; seguir = 0;
        break;
      case ERROR_LARGO:
        res = CARGAR_LARGO; seguir = 0;
        break;
      case ERROR_FORMATO:
        res = ERROR_LARGO; seguir = 0;
        break;
      }
    }
    if (seguir) {
      tablahash_insertar(entorno.tabla, 
        contacto_crear(input[0], input[1], edad, input[3]));
        input[0] = NULL; input[1] = NULL; input[3] = NULL;
    }
  }
  free(input[0]); free(input[1]); free(input[3]);
  fclose(fp);

  return res;
}

Resultado guardar(Entorno entorno) {
  char ruta[STRLEN];
  printf("Ingrese ruta de entrada:\n>");
  scanf("%s", ruta);
  FILE* fp = fopen(ruta, "r"); assert(fp); //TODO: mejor error
  escribir_cabecera(fp);
  tablahash_recorrer(entorno.tabla, escribir_contacto, fp);
}