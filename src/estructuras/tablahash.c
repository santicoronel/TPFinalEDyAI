#include "tablahash.h"

#include <stdlib.h>
#include <assert.h>

#define FACTOR_CARGA_MAX 0.7
#define CENTINELA -1

//NICETOHAVE: tomar funcion void* clave(void* dato) y fijarse antes de insertar

struct _TablaHash {
  void** elems;
  unsigned int capacidad;
  unsigned int nelems;
  double factor_carga;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

static int casilla_vacia(void* dato) { 
  return (dato == NULL) || (dato == CENTINELA); 
}

TablaHash tablahash_crear(unsigned int capacidad, FuncionComparadora comp, 
  FuncionDestructora destr, FuncionHash hash) {
  TablaHash tabla = malloc(sizeof(*tabla)); assert(tabla);
  void** elems = malloc(sizeof(*elems) * capacidad); assert(elems);
  *tabla = 
    (struct _TablaHash) {elems, capacidad, 0, 0, comp, destr, hash};
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

static unsigned int encontrar(TablaHash tabla, void* dato) {
  unsigned int pos = tabla->hash(dato) % tabla->capacidad;
  void* elem = tabla->elems[pos];
  while (elem) {
    if ((elem != CENTINELA) && tabla->comp(elem, dato) == 0) return pos;
    elem = tabla->elems[pos = (pos + 1) % tabla->capacidad];
  }
  return -1;
}

static void redimensionar(TablaHash tabla) {
  void* elems[tabla->nelems];
  for (unsigned int i = 0, j = 0; j < tabla->nelems; i++)
    if (!casilla_vacia(tabla->elems[i])) elems[j++] = tabla->elems[i];
  
  free(tabla->elems);
  tabla->capacidad *= 2; 
  tabla->elems = malloc(sizeof(*tabla->elems) * tabla->capacidad);
  assert(tabla->elems);

  for (unsigned int j = 0; j < tabla->nelems; j++) 
    tablahash_insertar(tabla, elems[j]);
  tabla->factor_carga = (double) tabla->capacidad / tabla->nelems;
}

int tablahash_insertar(TablaHash tabla, void *dato) {
  if (encontrar(tabla, dato) != -1) return 0;
  unsigned int pos = tabla->hash(dato) % tabla->capacidad;
  while (!casilla_vacia(tabla->elems[pos])) pos = (pos + 1) % tabla->capacidad;
  if (tabla->elems[pos] == NULL) 
    tabla->factor_carga += 1.0 / tabla->capacidad; 
  tabla->elems[pos] = dato;
  tabla->nelems++;
  if (tabla->factor_carga > FACTOR_CARGA_MAX)
    redimensionar(tabla);
  return 1;
}

void* tablahash_buscar(TablaHash tabla, void *dato) {
  unsigned int pos = encontrar(tabla, dato);
  if (pos == -1) return NULL;
  return tabla->elems[pos];
}

void* tablahash_eliminar(TablaHash tabla, void *dato) {
  unsigned int pos = encontrar(tabla, dato);
  if (pos == -1) return NULL;
  void* res = tabla->elems[pos];
  tabla->elems[pos] = CENTINELA;
  tabla->nelems--;
  return res;
}

void tablahash_recorrer(TablaHash tabla, FuncionVisitante visit, void* extra) {
  for (unsigned int i = 0, j = 0; j < tabla->elems; i++)
    if (!casilla_vacia(tabla->elems[i])) {
      visit(tabla->elems[i], extra); j++;
    }
}