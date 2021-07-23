#include "operaciones.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "contacto.h"
#include "archivos.h"
#include "historial.h"
#include "utils.h"
#include "estructuras/heap.h"

#define STRLEN 50 // Macro para largo de strings.

/** Nombres de atributos de contacto. */
char const* const atributosClave[CANT_ATRIBUTOS] = 
	{"nombre", "apellido", "edad", "telefono"};


Resultado buscar(Entorno entorno) {
  // Crea un contacto auxiliar para hacer la busqueda.
  char* nombre = ingresar_string(atributosClave[NOMBRE], STRLEN); 
  char* apellido = ingresar_string(atributosClave[APELLIDO], STRLEN);
  Contacto dummy = contacto_crear(nombre,apellido, -1, NULL);
  Contacto res = tablahash_buscar(entorno.agenda, dummy);
  contacto_destruir(dummy);

  if (res) contacto_imprimir(res); 
  else return BUSCAR_EXISTE;

  return EXITO;
}


Resultado agregar(Entorno entorno) {
  char* nombre = ingresar_string(atributosClave[NOMBRE], STRLEN);
  char* apellido = ingresar_string(atributosClave[APELLIDO], STRLEN);
  unsigned int edad;
  printf("Ingrese %s:\n>", atributosClave[EDAD]);
  scanf("%u", &edad); while(getchar() != '\n');
  char* telefono = ingresar_string(atributosClave[TELEFONO], STRLEN);
  Contacto contacto = contacto_crear(nombre, apellido, edad, telefono);
  // Si el contacto ya existia, lo destruimos e informamos el error.
  if (!tablahash_insertar(entorno.agenda, contacto)) {
    contacto_destruir(contacto); 
    return AGREGAR_EXISTE;
  };
  // Agregamos la operacion realizada al historial.
  historial_hecho(entorno.historial, operacion_crear(AGREGAR, contacto));
  
  return EXITO;
}


Resultado eliminar(Entorno entorno) {
  // Crea un contacto auxiliar para hacer la busqueda.
  char* nombre = ingresar_string(atributosClave[NOMBRE], STRLEN); 
  char* apellido = ingresar_string(atributosClave[APELLIDO], STRLEN);
  Contacto dummy = contacto_crear(nombre,apellido, -1, NULL);
  Contacto eliminado = tablahash_eliminar(entorno.agenda, dummy);
  contacto_destruir(dummy);
  // Si el contacto existia, agregamos la operacion realizada al historial.
  if (eliminado)
    historial_hecho(entorno.historial, operacion_crear(ELIMINAR, eliminado));
  else return ELIMINAR_EXISTE; // Informamos que el contacto no fue encontrado.

  return EXITO;
}


Resultado editar(Entorno entorno) {
  // Crea un contacto auxiliar para hacer la busqueda.
  char* nombre = ingresar_string(atributosClave[NOMBRE], STRLEN); 
  char* apellido = ingresar_string(atributosClave[APELLIDO], STRLEN);
  Contacto dummy = contacto_crear(nombre,apellido, -1, NULL);
  Contacto contacto = tablahash_buscar(entorno.agenda, dummy);
  if (contacto == NULL) {
    contacto_destruir(dummy);
    return EDITAR_EXISTE;
  }
  // Si el contacto existe, reemplaza los datos y guarda los datos viejos
  // en el historial.
  dummy->edad = contacto->edad;
  dummy->telefono = contacto->telefono;
  int edad;
  printf("Ingrese %s:\n>", atributosClave[EDAD]);
  scanf("%u", &edad); while(getchar() != '\n');
  contacto->edad = edad;
  contacto->telefono = ingresar_string(atributosClave[TELEFONO], STRLEN);
  historial_hecho(entorno.historial, operacion_crear(EDITAR, dummy));

  return EXITO;
}


/** Chequea que la cabecera del archivo sea valida */
static Resultado cabecera_valida(FILE* fp) {
  char input[STRLEN];
  for (int i = 0; i < CANT_ATRIBUTOS; i++) {
    switch (leer_palabra(fp, STRLEN, input)) {
    case COMA:
      // Si es la ultima palabra esperamos un salto de linea, no una coma.
      if (i == CANT_ATRIBUTOS - 1) return CARGAR_COMA;
      if ((strcmp(input, atributosClave[i]) != 0)) return CARGAR_CABECERA;
      break;
    case SALTO_LINEA:
      // Si no es la ultima palabra, esperamos una coma, no un salto de linea.
      if (i != CANT_ATRIBUTOS - 1) return CARGAR_SALTO_LINEA;
      if ((strcmp(input, atributosClave[i]) != 0)) return CARGAR_CABECERA;
      break;
    case FINAL:
      if (i == 0) return CARGAR_VACIO;
      // fallthrough
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
  char* ruta = ingresar_string("ruta de entrada", STRLEN);  
  FILE* fp = fopen(ruta, "r"); free(ruta);
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
        // Si es la ultima palabra esperamos un salto de linea, no una coma.
        if (i == CANT_ATRIBUTOS - 1) {
          res = CARGAR_COMA; seguir = 0;
        }
        break;
      case SALTO_LINEA:
        // Si no es la ultima palabra, esperamos una coma, no un salto de linea.
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
      int insertado = tablahash_insertar(entorno.agenda, 
        contacto_crear(input[NOMBRE], input[APELLIDO], edad, input[TELEFONO]));
      if (!insertado) {
        printf("El contacto \"%s %s\" ya existe.\n",
          input[NOMBRE], input[APELLIDO]);
        free(input[NOMBRE]); free(input[APELLIDO]); free(input[TELEFONO]);
      }
      else printf("Contacto \"%s %s\" agregado.\n", input[NOMBRE], input[APELLIDO]);
      input[NOMBRE] = NULL; input[APELLIDO] = NULL; input[TELEFONO] = NULL;
    }
  }
  free(input[NOMBRE]); free(input[APELLIDO]); free(input[TELEFONO]);
  fclose(fp);

  return res;
}


/** Funcion visitante para escribir contacto en archivo. */
static void escribir_contacto_general(void* contacto, void* fp) {
  escribir_contacto(contacto, fp);
}

Resultado guardar(Entorno entorno) {
  char* ruta = ingresar_string("ruta de salida", STRLEN);  
  FILE* fp = fopen(ruta, "w"); free(ruta);
  if (fp == NULL) return ARCHIVO_ERROR;
  escribir_cabecera(fp);
  tablahash_recorrer(entorno.agenda, escribir_contacto_general, fp);
  fclose(fp);
  return EXITO;
}


Resultado deshacer(Entorno entorno) {
  Operacion operacion = historial_ultimo_hecho(entorno.historial);
  if (operacion == NULL) return DESHACER_VACIO;
  switch (operacion->tag) {
    case AGREGAR:
      // Eliminamos el contacto de la agenda.
      tablahash_eliminar(entorno.agenda, operacion->contacto);
      break;
    case EDITAR: {
      // Intercambiamos los datos de los contactos.
      Contacto contacto = tablahash_buscar(entorno.agenda, operacion->contacto);
      unsigned int edad = operacion->contacto->edad; 
      char* telefono = operacion->contacto->telefono; 
      operacion->contacto->edad = contacto->edad;
      operacion->contacto->telefono = contacto->telefono;
      contacto->edad = edad;
      contacto->telefono = telefono;
      break; }
    case ELIMINAR:
      // Agregamos el contacto eliminado a la agenda.
      tablahash_insertar(entorno.agenda, operacion->contacto);
      break; 
  }
  // Agregamos la operacion deshecha al historial
  historial_deshecho(entorno.historial, operacion);

  return EXITO;
}


Resultado rehacer(Entorno entorno) {
  Operacion operacion = historial_ultimo_deshecho(entorno.historial);
  if (operacion == NULL) return REHACER_VACIO;

  switch (operacion->tag) {
    case AGREGAR:
      // Volvemos a agregar el contacto a la agenda.
      tablahash_insertar(entorno.agenda, operacion->contacto);
      break;
    case EDITAR: {
      // Intercambiamos los datos de los contactos.
      Contacto contacto = tablahash_buscar(entorno.agenda, operacion->contacto);
      unsigned int edad = operacion->contacto->edad; 
      char* telefono = operacion->contacto->telefono; 
      operacion->contacto->edad = contacto->edad;
      operacion->contacto->telefono = contacto->telefono;
      contacto->edad = edad;
      contacto->telefono = telefono;
      break; }
    case ELIMINAR:
      // Volvemos a eliminar el contacto de la agenda.
      tablahash_eliminar(entorno.agenda, operacion->contacto);
      break; 
  }
  // Agregamos la operacion realizada al historial.
  historial_hecho(entorno.historial, operacion);

  return EXITO;
}


/** Funcion auxiliar que implementa operaciones AND/OR. */
static Resultado and_or(Entorno entorno, FuncionVisitante imprimir_contacto) {
  char* input[CANT_ATRIBUTOS];
  unsigned int edad;
  int vacio = 1;
  for (int i = 0; i < CANT_ATRIBUTOS; i++) {
    input[i] = ingresar_string(atributosClave[i], STRLEN);
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
  tablahash_recorrer(entorno.agenda, imprimir_contacto, valores);
  contacto_destruir(valores);

  return EXITO;
} 
/** Funcion auxiliar que imprime el contacto si todos los valores coinciden. */
static void and_contacto(void* contacto, void* valores) {
  if (contacto_and(contacto, valores)) contacto_imprimir(contacto);
}

Resultado and(Entorno entorno) {
  return and_or(entorno, and_contacto);
}


/** Funcion auxiliar que imprime el contacto si algun valor coincide. */
static void or_contacto(void* contacto, void* valores) {
  if (contacto_or(contacto, valores)) contacto_imprimir(contacto);
}

Resultado or(Entorno entorno) {
  return and_or(entorno, or_contacto);
}


/** Funciones comparadoras genericas por atributo. */
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
/** Funcion visitante generica para insertar contacto en el heap. */
static void insertar_heap(void* contacto, void* heap) {
  heap_insertar(contacto, heap);
}

Resultado guardar_ordenado(Entorno entorno) {
  char* ruta = ingresar_string("ruta de salida", STRLEN);  
  FILE* fp = fopen(ruta, "w"); free(ruta);
  if (fp == NULL) return ARCHIVO_ERROR;
  char* atributo = ingresar_string("nombre de atributo", STRLEN);
  FuncionComparadora comp;
  // Determinamos que funcion comparadora usar.
  if (strcmp(atributosClave[NOMBRE], atributo) == 0) 
    comp = comparar_nombre;
  else if (strcmp(atributosClave[APELLIDO], atributo) == 0) 
    comp = comparar_apellido;
  else if (strcmp(atributosClave[EDAD], atributo) == 0) 
    comp = comparar_edad;
  else if (strcmp(atributosClave[TELEFONO], atributo) == 0) 
    comp = comparar_telefono;
  else {
    // El atributo ingresado es invalido.
    fclose(fp); free(atributo); 
    return GUARDAR_ORDENADO_ATRUBUTO;
  }
  free(atributo);
  escribir_cabecera(fp);
  Heap contactosOrdenados = heap_crear(tablahash_nelems(entorno.agenda), comp);
  tablahash_recorrer(entorno.agenda, insertar_heap, contactosOrdenados);
  while (!heap_vacio(contactosOrdenados)) 
    escribir_contacto(heap_extraer(contactosOrdenados), fp);
  heap_destruir(contactosOrdenados);
  fclose(fp);

  return EXITO;
}


/** Funcion visitante generica para sumar las edades de los contactos. */
static void sumar_edad(void* contacto, void* s) {
  *(int*)s += ((Contacto)contacto)->edad;
}
/** Funcion visitante generica para imprimir contacto. */
static void imprimir_contacto(void* c, void* x __attribute__((unused))) { 
  contacto_imprimir(c); 
}
/** Funcion comparadora generica para comparar contacto por edad decreciente. */
static int comparar_edad_decr(void* contacto1, void* contacto2) {
  return -contacto_comparar_edad(contacto1, contacto2); 
}

Resultado buscar_suma_edades(Entorno entorno) {
  unsigned int suma;
  printf("Ingrese un natural:\n>");
  scanf("%u", &suma); while(getchar() != '\n');
  // La suma debe ser mayor a 0.
  if (suma == 0) return SUMA_EDADES_CERO;

  unsigned int ncontactos = tablahash_nelems(entorno.agenda);
  // Si la agenda esta vacia, informamos que no se encontraron resultados.
  if (ncontactos == 0) return SUMA_EDADES_ENCONTRADO;

  unsigned int sumaEdades = 0;
  // Sumamos las edades de todos los contactos.
  // Si es menor a la suma, no hay resultado posible.
  // Si es igual, imprimos todos los contactos.
  tablahash_recorrer(entorno.agenda, sumar_edad, &sumaEdades);
  if (suma > sumaEdades) return SUMA_EDADES_ENCONTRADO;
  else if (suma == sumaEdades) {
    tablahash_recorrer(entorno.agenda, imprimir_contacto, NULL);
    return EXITO;
  }

  // Optimizacion: ordenamos los contactos de mayor a menor edad
  Heap contactos = heap_crear(ncontactos, comparar_edad_decr);
  tablahash_recorrer(entorno.agenda, insertar_heap, contactos);
  
  // Para cada subsuma entre 1 y suma, guardamos en mem la lista de contactos
  // tal que sus edades suman la subsuma.
  // Solo requerimos dos filas en la tabla: una para recordar los resultados
  // anteriores, y una para guardar los calculos nuevos.
  Contacto** mem[2];
  mem[0] = malloc(sizeof(Contacto*) * (suma + 1)); assert(mem[0]);
  mem[1] = malloc(sizeof(Contacto*) * (suma + 1)); assert(mem[1]);
  // Dejamos la primera tabla vacia.
  memset(mem[0], 0, (suma + 1) * sizeof(Contacto*)); 
  // En cant guardamos la cantidad de contactos en cada entrada de mem.
  unsigned int* cant[2];
  cant[0] = malloc(sizeof(int)* (suma + 1)); assert(cant[0]);
  cant[1] = malloc(sizeof(int)* (suma + 1)); assert(cant[0]);

  unsigned int sub;
  Contacto contacto;
  void* temp;
  int seguir = 1; unsigned int edad;
  // Resolvemos el problema con estrategia bottom-up.
  // Desde el mayor contacto al menor, chequeamos para cada subsuma:
  //  - si ya tenemos una lista de contactos para esta subsuma, la copiamos
  //  - si la edad del contacto es exactamente la subsuma, creamos una lista
  //    con dicho contacto como unico elemento.
  //  - si tenemos una lista de contactos para la subsuma - la edad,
  //    la copiamos y agregamos el nuevo contacto.
  // Luego de cada paso, chequeamos si ya tenemos una lista para la suma final.
  // En ese caso, detenemos el proceso.
  while (seguir && !heap_vacio(contactos)) {
    contacto = heap_extraer(contactos);
    edad = contacto->edad;
    for (sub = 1; sub <= suma; sub++) {
      // Ya existe una lista para esta subsuma: la copiamos.
      if (mem[0][sub]) {
        mem[1][sub] = mem[0][sub];
        cant[1][sub] = cant[0][sub];
      }
      // La edad del contacto coincide, creamos una lista nueva.
      else if (edad == sub) {
        mem[1][sub] = malloc(sizeof(Contacto*) * ncontactos);
        assert(mem[1][sub]); 
        mem[1][sub][0] = contacto; 
        cant[1][sub] = 1;
      }
      // Tenemos una subsuma menor: copiamos la lista y agregamos el contacto.
      else if (sub > edad && mem[0][sub - edad]) {
        mem[1][sub] = malloc(sizeof(Contacto) * ncontactos);
        assert(mem[1][sub]);
        // Copiamos la lista.
        memcpy(mem[1][sub], mem[0][sub - edad], 
          cant[0][sub - edad] * sizeof(Contacto));
        // Agregamos el contacto nuevo.
        mem[1][sub][cant[0][sub - edad]] = contacto;
        // Actualizamos la cantidad.
        cant[1][sub] = cant[0][sub - edad] + 1;
      }
      // No hay coincidencias, dejamos la entrada vacia.
      else mem[1][sub] = NULL;
    }
    temp = mem[0]; mem[0] = mem[1]; mem[1] = temp;
    temp = cant[0]; cant[0] = cant[1]; cant[1] = temp;
    // Encontramos la lista que estabamos buscando: salimos del loop.
    if (mem[0][suma]) seguir = 0;
  }
  heap_destruir(contactos);

  Resultado res = EXITO;
  // Si no hubo resultados, informamos la situacion.
  if (!mem[0][suma]) res = SUMA_EDADES_ENCONTRADO;
  // Imprimimos el resultado encontrado.
  else for (unsigned int i = 0; i < cant[0][suma]; i++)
    contacto_imprimir(mem[0][suma][i]); 
  // Liberamos la memoria utilizada.
  for (sub = 1; sub <= suma; sub++)
    free(mem[0][sub]);
  free(mem[0]); free(mem[1]);
  free(cant[0]); free(cant[1]);
  
  return res;
}