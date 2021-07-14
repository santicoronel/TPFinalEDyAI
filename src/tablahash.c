#include "tablahash.h"

#include <stdlib.h>
#include <assert.h>

#define CENTINELA -1
 
struct _TablaHash {
  void** elems;
  unsigned int capacidad;
  unsigned int nelems;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

static int dato_vacio(void* dato) { 
  return (dato == NULL) || (dato == CENTINELA); 
}

TablaHash tablahash_crear(unsigned int capacidad, FuncionComparadora comp, 
  FuncionDestructora destr, FuncionHash hash) {
  TablaHash tabla = malloc(sizeof(*tabla));
  void** elems = malloc(sizeof(*elems) * capacidad);
  assert(elems);
  *tabla = (struct _TablaHash) {elems, capacidad, 0, comp, destr, hash};
  return tabla;  
}

unsigned int tablahash_nelems(TablaHash tabla) { return tabla->nelems; }

unsigned int tablahash_capacidad(TablaHash tabla) { return tabla->capacidad; }

void tablahash_destruir(TablaHash tabla) {
  for (unsigned int i = 0; i < tabla->capacidad; i++) {
    if (!dato_vacio(tabla->elems[i]))
      tabla->destr(tabla->elems[i]);
  }
  free(tabla->elems);
  free(tabla);
}

static void tablahash_redimensionar(TablaHash tabla) {
  void* elems[tabla->nelems]; unsigned int it = 0;
  for (unsigned int i = 0; i < tabla->capacidad && it < tabla->nelems; i++) {
    if (!dato_vacio(tabla->elems[i])) {
      elems[it] = tabla->elems[i];
      it++;  
    }
  }
  free(tabla->elems);
  tabla->capacidad = (tabla->nelems * 100) / FACTOR_CARGA_INI;
  tabla->elems = malloc(sizeof(*tabla->elems) * tabla->capacidad);
  assert(tabla->elems);
  for (it = 0; it < tabla->nelems; it++) tablahash_insertar(tabla, elems[it]);
}

void tablahash_insertar(TablaHash tabla, void *dato) {
  if ((tabla->nelems * 100) / tabla->capacidad > FACTOR_CARGA_MAX)
    tablahash_redimensionar(tabla);

  unsigned int pos = tabla->hash(dato) % tabla->capacidad;
  while (!dato_vacio(tabla->elems[pos])) {
    if (tabla->comp(tabla->elems[pos], dato) == 0) {
      tabla->destr(tabla->elems[pos]);  
      tabla->elems[pos] = NULL;
    }
    else pos = (pos + 1) % tabla->capacidad;
  }
  tabla->elems[pos] = dato;
}

void* tablahash_buscar(TablaHash tabla, void *dato) {
  unsigned int pos = tabla->hash(dato) % tabla->capacidad;
  int encontrado = 0;
  void* elem;
  while(!encontrado && tabla->elems[pos] != NULL) {
    elem = tabla->elems[pos];
    if ((elem != CENTINELA) && tabla->comp(elem, dato) == 0) encontrado = 1;
    else pos = (pos + 1) % tabla->capacidad;
  }
  return tabla->elems[pos];
}

void tablahash_eliminar(TablaHash tabla, void *dato) {
  unsigned int pos = tabla->hash(dato) % tabla->capacidad;
  int encontrado = 0;
  void* elem;
  while (!encontrado && (tabla->elems[pos] != NULL)) {
    elem = tabla->elems[pos];
    if ((elem != CENTINELA) && (tabla->comp(elem, dato) == 0))
      tabla->destr(elem);
      tabla->elems[pos] = CENTINELA;
  }
}
