#include "operaciones.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "contacto.h"
#include "archivos.h"
#include "historial.h"
#include "utils.h"
#include "estructuras/heap.h"
#include "estructuras/pila.h"

#define STRLEN 50

//TODO: ver input de strings

char const* const atributosClave[CANT_ATRIBUTOS] = 
	{ "nombre", "apellido", "edad", "telefono" };

Resultado buscar(Entorno entorno) {
  printf("Ingrese %s:\n>", atributosClave[NOMBRE]);
  char* nombre = malloc(STRLEN); assert(nombre);
  input_string(nombre, STRLEN);
  printf("Ingrese %s:\n>", atributosClave[APELLIDO]);
  char* apellido = malloc(STRLEN); assert(apellido); 
  input_string(apellido, STRLEN);

  Contacto dummy = contacto_crear(nombre, apellido, -1, NULL);
  Contacto res = tablahash_buscar(entorno.tabla, dummy);
  contacto_destruir(dummy);

  if (res) contacto_imprimir(res); 
  else return BUSCAR_EXISTE;

  return EXITO;
}

Resultado agregar(Entorno entorno) {
  char* nombre = malloc(sizeof(STRLEN)); assert(nombre);
  printf("Ingrese %s:\n>", atributosClave[NOMBRE]);
  input_string(nombre, STRLEN);
  char* apellido = malloc(sizeof(STRLEN)); assert(apellido);
  printf("Ingrese %s:\n>", atributosClave[APELLIDO]);
  input_string(apellido, STRLEN);
  unsigned int edad;
  printf("Ingrese %s:\n>", atributosClave[EDAD]);
  scanf("%u", &edad); getchar();
  char* telefono = malloc(sizeof(STRLEN)); assert(telefono);
  printf("Ingrese %s:\n>", atributosClave[TELEFONO]);
  input_string(telefono, STRLEN);
  
  Contacto contacto = contacto_crear(nombre, apellido, edad, telefono);
  if (!tablahash_insertar(entorno.tabla, contacto)) {
    contacto_destruir(contacto); 
    return AGREGAR_EXISTE;
  }; 
  historial_hecho(entorno.historial, operacion_crear(AGREGAR, contacto));
  
  return EXITO;
}

Resultado eliminar(Entorno entorno) {
  printf("Ingrese %s:\n>", atributosClave[NOMBRE]);
  char* nombre = malloc(STRLEN); assert(nombre); 
  input_string(nombre, STRLEN);
  printf("Ingrese %s:\n>", atributosClave[APELLIDO]);
  char* apellido = malloc(STRLEN); assert(apellido); 
  input_string(apellido, STRLEN);

  Contacto dummy = contacto_crear(nombre, apellido, -1, NULL);
  Contacto eliminado = tablahash_eliminar(entorno.tabla, dummy);
  contacto_destruir(dummy);

  if (eliminado)
    historial_hecho(entorno.historial, operacion_crear(ELIMINAR, eliminado));
  else return ELIMINAR_EXISTE;

  return EXITO;
}

Resultado editar(Entorno entorno) {
  printf("Ingrese %s:\n>", atributosClave[NOMBRE]);
  char* nombre = malloc(STRLEN); assert(nombre); 
  input_string(nombre, STRLEN);
  printf("Ingrese %s:\n>", atributosClave[APELLIDO]);
  char* apellido = malloc(STRLEN); assert(apellido); 
  input_string(apellido, STRLEN);

  Contacto dummy = contacto_crear(nombre, apellido, -1, NULL);
  Contacto contacto = tablahash_buscar(entorno.tabla, dummy);
  if (contacto == NULL) {
    contacto_destruir(dummy);
    return EDITAR_EXISTE;
  }

  dummy->edad = contacto->edad;
  dummy->telefono = contacto->telefono;
  int edad;
  printf("Ingrese %s:\n>", atributosClave[EDAD]);
  scanf("%u", &edad); getchar();
  contacto->edad = edad;
  printf("Ingrese %s:\n>", atributosClave[TELEFONO]);
  char* telefono = malloc(STRLEN); assert(telefono);
  input_string(telefono, STRLEN);
  contacto->telefono = telefono;
  
  historial_hecho(entorno.historial, operacion_crear(EDITAR, dummy));

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
  FILE* fp = fopen(ruta, "r");
  if (fp == NULL) return ARCHIVO_ENCONTRADO;
  
  Resultado res = cabecera_valida(fp);
  if (res != EXITO) return res;

  char* input[CANT_ATRIBUTOS] = {NULL, NULL, NULL, NULL}; 
  unsigned int edad;
  int seguir = 1; Lectura lec; 
  while(seguir) {
    for (int i = 0; i < CANT_ATRIBUTOS && seguir; i++) {
      if (i == EDAD) lec = leer_numero(fp, &edad);
      else {
        input[i] = malloc(STRLEN); assert(input);
        lec = leer_palabra(fp, STRLEN, input[i]);
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
        res = i == 0 ? EXITO : CARGAR_EOF;
        seguir = 0;
        break;
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
      int insertado = tablahash_insertar(entorno.tabla, 
        contacto_crear(input[NOMBRE], input[APELLIDO], edad, input[TELEFONO]));
      if (!insertado) {
        printf("El contacto \"%s %s\" ya existe.\n",
          input[NOMBRE], input[APELLIDO]);
        free(input[NOMBRE]); free(input[APELLIDO]); free(input[TELEFONO]);
      }
      input[NOMBRE] = NULL; input[APELLIDO] = NULL; input[TELEFONO] = NULL;
    }
  }
  free(input[NOMBRE]); free(input[APELLIDO]); free(input[TELEFONO]);
  fclose(fp);

  return res;
}

static void escribir_contacto_general(void* contacto, void* fp) {
  escribir_contacto(contacto, fp);
}

Resultado guardar(Entorno entorno) {
  char ruta[STRLEN];
  printf("Ingrese ruta de entrada:\n>");
  scanf("%s", ruta);
  FILE* fp = fopen(ruta, "w");
  if (fp == NULL) return ARCHIVO_ERROR;
  escribir_cabecera(fp);
  tablahash_recorrer(entorno.tabla, escribir_contacto_general, fp);
  fclose(fp);
  return EXITO;
}

Resultado deshacer(Entorno entorno) {
  Operacion operacion = historial_ultimo_hecho(entorno.historial);
  if (operacion == NULL) return DESHACER_VACIO;

  Contacto dummy = contacto_crear(
    operacion->contacto->nombre,
    operacion->contacto->apellido,
    -1, NULL);
  switch (operacion->tag) {
    case AGREGAR:
      tablahash_eliminar(entorno.tabla, dummy);
      break;
    case EDITAR: {
      Contacto contacto = tablahash_buscar(entorno.tabla, dummy);
      unsigned int edad = operacion->contacto->edad; 
      char* telefono = operacion->contacto->telefono; 
      operacion->contacto->edad = contacto->edad;
      operacion->contacto->telefono = contacto->telefono;
      contacto->edad = edad;
      contacto->telefono = telefono;
      break; }
    case ELIMINAR:
      //TODO: chequear si el elemento se inserta; sino, elevar y borrar historial?
      tablahash_insertar(entorno.tabla, operacion->contacto);
      break; 
  }
  historial_deshecho(entorno.historial, operacion);
  dummy->nombre = NULL; dummy->apellido = NULL;
  contacto_destruir(dummy);

  return EXITO;
}

Resultado rehacer(Entorno entorno) {
  Operacion operacion = historial_ultimo_deshecho(entorno.historial);
  if (operacion == NULL) return REHACER_VACIO;

  Contacto dummy = contacto_crear(
    operacion->contacto->nombre,
    operacion->contacto->apellido,
    -1, NULL);
  switch (operacion->tag) {
    case AGREGAR:
      tablahash_insertar(entorno.tabla, operacion->contacto);
      historial_hecho(entorno.historial, operacion);
      break;
    case EDITAR: {
      Contacto contacto = tablahash_buscar(entorno.tabla, dummy);
      unsigned int edad = operacion->contacto->edad; 
      char* telefono = operacion->contacto->telefono; 
      operacion->contacto->edad = contacto->edad;
      operacion->contacto->telefono = contacto->telefono;
      contacto->edad = edad;
      contacto->telefono = telefono;
      historial_hecho(entorno.historial, operacion);
      break; }
    case ELIMINAR:
      tablahash_eliminar(entorno.tabla, operacion->contacto);
      historial_hecho(entorno.historial, operacion);
      break; 
  }
  dummy->nombre = NULL; dummy->apellido = NULL;
  contacto_destruir(dummy);  

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
    printf("Ingrese %s:\n>", atributosClave[i]);
    scanf("%s", input[i]);
    if (strcmp("vacio", input[i]) == 0)  {
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

static void and_contacto(void* contacto, void* valores) {
  if (contacto_and(contacto, valores)) contacto_imprimir(contacto);
}

Resultado and(Entorno entorno) {
  return and_or(entorno, and_contacto);
}

static void or_contacto(void* contacto, void* valores) {
  if (contacto_or(contacto, valores)) contacto_imprimir(contacto);
}

Resultado or(Entorno entorno) {
  return and_or(entorno, or_contacto);
}

static int comparar_nombre(void* contacto1, void* contacto2) {
  return contacto_comparar_nombre(contacto1, contacto2);
}
static int comparar_apellido(void* contacto1, void* contacto2) {
  return contacto_comparar_apellido(contacto1, contacto2);
}
static int comparar_edad(void* contacto1, void* contacto2) {
  return contacto_comparar_edad(contacto1, contacto2);
}
static int comparar_telefono(void* contacto1, void* contacto2) {
  return contacto_comparar_telefono(contacto1, contacto2);
}
static void insertar_heap(void* contacto, void* heap) {
  heap_insertar(contacto, heap);
}

Resultado guardar_ordenado(Entorno entorno) {
  char input[STRLEN];
  printf("Ingrese ruta de salida:\n>");
  scanf("%s", input);
  FILE* fp = fopen(input, "w");
  if (fp == NULL) return ARCHIVO_ERROR;
  printf("Ingrese nombre de atributo:\n>");
  scanf("%s", input);
  FuncionComparadora comp;
  if (strcmp(atributosClave[NOMBRE], input) == 0) 
    comp = comparar_nombre;
  else if (strcmp(atributosClave[APELLIDO], input) == 0) 
    comp = comparar_apellido;
  else if (strcmp(atributosClave[EDAD], input) == 0) 
    comp = comparar_edad;
  else if (strcmp(atributosClave[TELEFONO], input) == 0) 
    comp = comparar_telefono;
  else {
    fclose(fp); return GUARDAR_ORDENADO_ATRUBUTO;
  }
  
  escribir_cabecera(fp);
  Heap contactosOrdenados = heap_crear(tablahash_nelems(entorno.tabla), comp);
  tablahash_recorrer(entorno.tabla, insertar_heap, contactosOrdenados);
  while (!heap_vacio(contactosOrdenados)) 
    escribir_contacto(heap_extraer(contactosOrdenados), fp);
  heap_destruir(contactosOrdenados);
  fclose(fp);

  return EXITO;
}

static void sumar_edad(void* contacto, void* s) {
  *(int*)s += ((Contacto)contacto)->edad;
}
static void imprimir_contacto(void* c, void* x __attribute__((unused))) { 
  contacto_imprimir(c); 
}
static int comparar_edad_decr(void* contacto1, void* contacto2) {
  return -contacto_comparar_edad(contacto1, contacto2); 
}

Resultado buscar_suma_edades(Entorno entorno) {
  unsigned int suma;
  printf("Ingrese un natural:\n>");
  scanf("%u", &suma); if (suma == 0) return SUMA_EDADES_CERO;

  unsigned int ncontactos = tablahash_nelems(entorno.tabla);
  if (ncontactos == 0) return SUMA_EDADES_ENCONTRADO;

  unsigned int sumaEdades = 0;
  tablahash_recorrer(entorno.tabla, sumar_edad, &sumaEdades);
  if (suma > sumaEdades) return SUMA_EDADES_ENCONTRADO;
  else if (suma == sumaEdades) {
    tablahash_recorrer(entorno.tabla, imprimir_contacto, NULL);
    return EXITO;
  }

  Heap contactos = heap_crear(ncontactos, comparar_edad_decr);
  tablahash_recorrer(entorno.tabla, insertar_heap, contactos);
  
  Contacto** mem[2];
  mem[0] = malloc(sizeof(Contacto*) * (suma + 1)); assert(mem[0]);
  mem[1] = malloc(sizeof(Contacto*) * (suma + 1)); assert(mem[1]);
  unsigned int* cant[2];
  cant[0] = malloc(sizeof(int)* (suma + 1)); assert(cant[0]);
  cant[1] = malloc(sizeof(int)* (suma + 1)); assert(cant[0]);
  memset(mem[0], 0, (suma + 1) * sizeof(Contacto*)); 
  memset(cant[0], 0, (suma + 1) * sizeof(int));

  unsigned int sub;
  Contacto contacto;
  void* temp;
  int seguir = 1; unsigned int edad;
  while (seguir && !heap_vacio(contactos)) {
    contacto = heap_extraer(contactos);
    edad = contacto->edad;
    for (sub = 1; sub <= suma; sub++) {
      if (mem[0][sub]) {
        mem[1][sub] = mem[0][sub];
        cant[1][sub] = cant[0][sub];
      }
      else if (edad == sub) {
        mem[1][sub] = malloc(sizeof(Contacto*) * ncontactos);
        assert(mem[1][sub]); 
        mem[1][sub][0] = contacto; 
        cant[1][sub] = 1;
      }
      else if (sub > edad && mem[0][sub - edad]) {
        mem[1][sub] = malloc(sizeof(Contacto) * ncontactos);
        assert(mem[1][sub]);
        memcpy(mem[1][sub], mem[0][sub - edad], 
          cant[0][sub - edad] * sizeof(Contacto));
        mem[1][sub][cant[0][sub - edad]] = contacto;
        cant[1][sub] = cant[0][sub - edad] + 1;
      }
      else {
        mem[1][sub] = NULL; cant[1][sub] = 0;
      }
    }
    temp = mem[0]; mem[0] = mem[1]; mem[1] = temp;
    temp = cant[0]; cant[0] = cant[1]; cant[1] = temp;
    if (mem[0][suma]) seguir = 0;
  }
  heap_destruir(contactos);

  Resultado res = EXITO;
  if (!mem[0][suma]) res = SUMA_EDADES_ENCONTRADO;
  else for (unsigned int i = 0; i < cant[0][suma]; i++)
    contacto_imprimir(mem[0][suma][i]); 
  for (sub = 1; sub <= suma; sub++)
    free(mem[0][sub]);
  free(mem[0]); free(mem[1]);
  free(cant[0]); free(cant[1]);
  return res;
}