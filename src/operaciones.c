#include "operaciones.h"

#include <string.h>

#include "contacto.h"
#include "archivos.h"
#include "historial.h"
#include "utils.h"

#define STRLEN 50

Resultado buscar(Entorno entorno) {
  printf("Ingrese %s:\n>", atributosClave[NOMBRE]);
  char nombre[STRLEN]; fgets(nombre, STRLEN, stdin);
  printf("Ingrese %s:\n>", atributosClave[APELLIDO]);
  char apellido[STRLEN]; fgets(apellido, STRLEN, stdin);

  char* nombre_apellido[2] = {nombre, apellido};
  Contacto res = tablahash_buscar(entorno.tabla, nombre_apellido);

  if (res) contacto_imprimir(res); 
  else return BUSCAR_EXISTE;

  return EXITO;
}

Resultado agregar(Entorno entorno) {
  char* nombre = malloc(sizeof(STRLEN)); assert(nombre);
  printf("Ingrese %s:\n>", atributosClave[NOMBRE]);
  fgets(nombre, STRLEN, stdin);
  char* apellido = malloc(sizeof(STRLEN)); assert(apellido);
  printf("Ingrese %s:\n>", atributosClave[APELLIDO]);
  fgets(apellido, STRLEN, stdin);

  char* nombre_apellido[2] = {nombre, apellido};
  if (tablahash_buscar(entorno.tabla, nombre_apellido))
    return AGREGAR_EXISTE;

  unsigned int edad;
  printf("Ingrese %s:\n>", atributosClave[EDAD]);
  scanf("%u", &edad);
  char* telefono = malloc(sizeof(STRLEN)); assert(telefono);
  printf("Ingrese %s:\n>", atributosClave[TELEFONO]);
  fgets(telefono, STRLEN, stdin);
  
  Contacto contacto = contacto_crear(nombre, apellido, edad, telefono);
  tablahash_insertar(entorno.tabla, contacto);
  historial_hecho(entorno.historial, operacion_crear(AGREGAR, contacto));
  
  return EXITO;
}

Resultado eliminar(Entorno entorno) {
  printf("Ingrese %s:\n>", atributosClave[NOMBRE]);
  char nombre[STRLEN]; fgets(nombre, STRLEN, stdin);
  printf("Ingrese %s:\n>", atributosClave[APELLIDO]);
  char apellido[STRLEN]; fgets(apellido, STRLEN, stdin);

  char* nombre_apellido[2] = {nombre, apellido};
  Contacto eliminado = tablahash_eliminar(entorno.tabla, nombre_apellido);

  if (eliminado)
    historial_hecho(entorno.historial, operacion_crear(ELIMINAR, eliminado));
  else return ELIMINAR_EXISTE;

  return EXITO;
}

Resultado editar(Entorno entorno) {
  printf("Ingrese %s:\n>", atributosClave[NOMBRE]);
  char* nombre = malloc(STRLEN); assert(nombre);
  fgets(nombre, STRLEN, stdin);
  printf("Ingrese %s:\n>", atributosClave[APELLIDO]);
  char* apellido = malloc(STRLEN); assert(apellido); 
  fgets(apellido, STRLEN, stdin);

  char* nombre_apellido[2] = {nombre, apellido};
  Contacto contacto = tablahash_buscar(entorno.tabla, nombre_apellido);
  if (contacto == NULL) {
    free(nombre); free(apellido);
    return EDITAR_EXISTE;
  }

  Contacto contacto_viejo = 
    contacto_crear(nombre, apellido, contacto->edad, contacto->telefono);
  int edad;
  printf("Ingrese %s:\n>", atributosClave[EDAD]);
  scanf("%u", &edad);
  contacto->edad = edad;
  printf("Ingrese %s:\n>", atributosClave[TELEFONO]);
  char* telefono = malloc(STRLEN); assert(telefono);
  fgets(telefono, STRLEN, stdin);
  contacto->telefono = telefono;
  
  historial_hecho(entorno.historial, operacion_crear(EDITAR, contacto_viejo));

  return EXITO;
}

static Resultado cabecera_valida(FILE* fp) {
  char input[STRLEN];
  for (int i = 0; i < CANT_ATRIBUTOS; i++) {
    switch (leer_palabra(fp, STRLEN, input)) {
    case COMA:
      if (i == CANT_ATRIBUTOS - 1) return CARGAR_COMA;
      if ((strcmp(input, atributosClave[i]) != 0)) return CARGAR_CABECERA;
      break;
    case SALTO_LINEA:
      if (i != CANT_ATRIBUTOS - 1) return CARGAR_SALTO_LINEA;
      if ((strcmp(input, atributosClave[i]) != 0)) return CARGAR_CABECERA;
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

  char* input[CANT_ATRIBUTOS] = {NULL, NULL, NULL, NULL}; 
  unsigned int edad;
  int seguir = 1; Lectura lec; 
  while(seguir) {
    for (int i = 0; i < CANT_ATRIBUTOS; i++) {
      if (i == EDAD) lec = leer_numero(fp, &edad);
      else {
        input[i] = malloc(STRLEN); assert(input);
        lec = leer_palabra(fp, STRLEN, leer_palabra);
      }
      switch(lec) {
      case COMA:
        if (i == CANT_ATRIBUTOS - 1) {
          res = CARGAR_COMA; seguir = 0;
        }
        break;
      case SALTO_LINEA:
        if (i != CANT_ATRIBUTOS - 1) {
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
        contacto_crear(input[NOMBRE], input[APELLIDO], edad, input[TELEFONO]));
      input[NOMBRE] = NULL; input[APELLIDO] = NULL; input[TELEFONO] = NULL;
    }
  }
  free(input[NOMBRE]); free(input[APELLIDO]); free(input[TELEFONO]);
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

Resultado deshacer(Entorno entorno) {
  Operacion operacion = historial_ultimo_hecho(entorno.historial);
  if (operacion == NULL) return DESHACER_VACIO;

  char* nombre_apellido[2] = 
    {operacion->contacto->nombre, operacion->contacto->apellido};
  switch (operacion->tag) {
    case AGREGAR:
      tablahash_eliminar(entorno.tabla, nombre_apellido);
      historial_deshecho(entorno.historial, operacion);
      break;
    case EDITAR:
      Contacto contacto = tablahash_buscar(entorno.tabla, nombre_apellido);
      unsigned int edad = operacion->contacto->edad; 
      char* telefono = operacion->contacto->telefono; 
      operacion->contacto->edad = contacto->edad;
      operacion->contacto->telefono = contacto->telefono;
      contacto->edad = edad;
      contacto->telefono = telefono;
      historial_deshecho(entorno.historial, operacion);
      break;
    case ELIMINAR:
      tablahash_insertar(entorno.tabla, operacion->contacto);
      historial_deshecho(entorno.historial, operacion);
      break; 
  }
  return EXITO;
}

Resultado rehacer(Entorno entorno) {
  Operacion operacion = historial_ultimo_deshecho(entorno.historial);
  if (operacion == NULL) return REHACER_VACIO;

  char* nombre_apellido[2] = 
    {operacion->contacto->nombre, operacion->contacto->apellido};
  switch (operacion->tag) {
    case AGREGAR:
      tablahash_insertar(entorno.tabla, nombre_apellido);
      historial_hecho(entorno.historial, operacion);
      break;
    case EDITAR:
      Contacto contacto = tablahash_buscar(entorno.tabla, nombre_apellido);
      unsigned int edad = operacion->contacto->edad; 
      char* telefono = operacion->contacto->telefono; 
      operacion->contacto->edad = contacto->edad;
      operacion->contacto->telefono = contacto->telefono;
      contacto->edad = edad;
      contacto->telefono = telefono;
      historial_deshecho(entorno.historial, operacion);
      break;
    case ELIMINAR:
      tablahash_eliminar(entorno.tabla, operacion->contacto);
      historial_deshecho(entorno.historial, operacion);
      break; 
  }
  return EXITO;
}

static Resultado and_or(Entorno entorno, FuncionVisitante imprimir_contacto) {
    char* input[CANT_ATRIBUTOS];
  for (int i = 0; i < CANT_ATRIBUTOS; i++) {
    input[i] = malloc(STRLEN); assert(input[i]);
  }
  unsigned int edad;
  int vacio = 1;
  for (int i = 0; i < CANT_ATRIBUTOS; i++) {
    scanf("%s", input[i]);
    if (strcmp(atributosClave[i], input[i]) != 0) return AND_OR_ATRIBUTOS;
    scanf("%s", input[i]);
    if (strcmp("vacio", input[i]) != 0)  {
      free(input[i]); input[i] = NULL;
      if (i == EDAD) edad = -1;  
    }
    else {
      vacio = 0;
      if (i == EDAD) edad = string_to_uint(input[EDAD]);
    }
  }
  free(input[EDAD]);
  if (vacio) return AND_OR_VACIO;
  
  Contacto valores = 
    contacto_crear(input[NOMBRE], input[APELLIDO], edad, input[TELEFONO]);
  puts("");
  tablahash_recorrer(entorno.tabla, imprimir_contacto, valores);
  contacto_destruir(valores);

  return EXITO;
} 

static void and_contacto(Contacto contacto, Contacto valores) {
  if (contacto_and(contacto, valores)) contacto_imprimir(contacto);
}

Resultado and(Entorno entorno) {
  and_or(entorno, and_contacto);
}

static void or_contacto(Contacto contacto, Contacto valores) {
  if (contacto_or(contacto, valores)) contacto_imprimir(contacto);
}

Resultado or(Entorno entorno) {
  and_or(entorno, or_contacto);
}