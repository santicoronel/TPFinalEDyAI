/** Implementacion de la tabla hash. Resuelve las colisones con linear probing,
 * usando valores centinelas para los elementos eliminados.
 */

#include "tablahash.h"

#include <stdlib.h>
#include <assert.h>

#define FACTOR_CARGA_MAX 0.7 // Si se supera este numero, debemos redimensionar.
#define CENTINELA ((void*)-1) // Indica un elemento eliminado.

/**
 * Estructura de la tabla hash. Tenemos un arreglo de elementos, su capacidad,
 * el numero de elementos, el factor de carga (teniendo en cuenta valores centinela),
 * y las funciones pertinentes.
 */
struct _TablaHash {
  void** elems;
  unsigned int capacidad;
  unsigned int nelems;
  double factor_carga;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

/** Determina si una casilla esta vacia. */
static int casilla_vacia(void* dato) { 
  return (dato == NULL) || (dato == CENTINELA); 
}

TablaHash tablahash_crear(unsigned int capacidad, FuncionComparadora comp, 
  FuncionDestructora destr, FuncionHash hash) {
  TablaHash tabla = malloc(sizeof(*tabla)); assert(tabla);
  // Representamos las casillas vacias con NULL.
  void** elems = calloc(capacidad, sizeof(*elems)); assert(elems);
  *tabla = (struct _TablaHash) {elems, capacidad, 0, 0, comp, destr, hash};
  return tabla;  
}

void tablahash_destruir(TablaHash tabla) {
  for (unsigned int i = 0, j = 0; j < tabla->nelems; i++)
    if (!casilla_vacia(tabla->elems[i])) {
      tabla->destr(tabla->elems[i]); j++;
    }
  free(tabla->elems);
  free(tabla);
}

unsigned int tablahash_nelems(TablaHash tabla) {
  return tabla->nelems;
}
/** 
 * Devuelve la posicon del dato dado en la tabla. En caso de no encontrarse,
 * devuelve -1. 
 */
static unsigned int encontrar(TablaHash tabla, void* dato) {
  unsigned int pos = tabla->hash(dato) % tabla->capacidad;
  void* elem = tabla->elems[pos];
  while (elem) {
    if ((elem != CENTINELA) && tabla->comp(elem, dato)) return pos;
    elem = tabla->elems[pos = (pos + 1) % tabla->capacidad];
  }
  return -1;
}
/**
 * Inserta el elemento en la tabla, en la primer casilla vacia que encuentre.
 */
static void insertar_elem(TablaHash tabla, void* dato) {
  unsigned int pos = tabla->hash(dato) % tabla->capacidad;
  while (!casilla_vacia(tabla->elems[pos])) pos = (pos + 1) % tabla->capacidad;
  // Si la casilla antes estaba vacia, actualizamos el factor de carga.
  if (tabla->elems[pos] == NULL) 
    tabla->factor_carga += 1.0 / tabla->capacidad;
  tabla->elems[pos] = dato;
}
/**
 * Redimensiona la tabla al doble de su capacidad. 
 * Remueve los elementos eliminados.
 */
static void redimensionar(TablaHash tabla) {
  tabla->capacidad *= 2; 
  void** elems = tabla->elems;
  tabla->elems = calloc(tabla->capacidad, sizeof(*tabla->elems));
  assert(tabla->elems);

  for (unsigned int i = 0, j = 0; j < tabla->nelems; i++) 
    if (!casilla_vacia(elems[i])) 
      insertar_elem(tabla, tabla->elems[j++]);
  
  free(elems);
  tabla->factor_carga = (double) tabla->nelems / tabla->capacidad;
}

int tablahash_insertar(TablaHash tabla, void *dato) {
  // Si el elemento ya se encuentra, devolvemos 0.
  if (encontrar(tabla, dato) != (unsigned)-1) return 0;
  insertar_elem(tabla, dato);
  tabla->nelems++;
  // Chequeamos si hace falta redimensionar la tabla.
  if (tabla->factor_carga > FACTOR_CARGA_MAX)
    redimensionar(tabla);
  return 1;
}

void* tablahash_buscar(TablaHash tabla, void *dato) {
  unsigned int pos = encontrar(tabla, dato);
  if (pos == (unsigned)-1) return NULL;
  return tabla->elems[pos];
}

void* tablahash_eliminar(TablaHash tabla, void *dato) {
  unsigned int pos = encontrar(tabla, dato);
  if (pos == (unsigned)-1) return NULL;
  void* res = tabla->elems[pos];
  tabla->elems[pos] = CENTINELA;
  tabla->nelems--;
  return res;
}

void tablahash_recorrer(TablaHash tabla, FuncionVisitante visit, void* extra) {
  for (unsigned int i = 0, j = 0; j < tabla->nelems; i++)
    if (!casilla_vacia(tabla->elems[i])) {
      visit(tabla->elems[i], extra); j++;
    }
}