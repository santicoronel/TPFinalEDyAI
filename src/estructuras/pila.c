/** Implementa una pila como una lista circular, utilizando arreglos estaticos. */

#include "pila.h"

#include <stdlib.h>
#include <assert.h>

/**
 * Estructura de la pila. Tenemos un array de elementos, su capacidad,
 * la posicion del elemento del tope y una funcion destructora.
 * Representamos una posicion vacia con NULL.
 */
struct _Pila {
  void** elems;
  int capacidad;
  int top;
  FuncionDestructora destr;
};

/** Devuelve un puntero al tope de la pila. */  
static void** top(Pila pila) { return pila->elems + pila->top; }

Pila pila_crear(int capacidad, FuncionDestructora destr) {
  Pila pila = malloc(sizeof(*pila)); assert(pila);
  void** elems = calloc(capacidad, sizeof(*elems));
  // El ultimo es la posicion anterior a la inicial, modulo la capacidad.
  *pila = (struct _Pila) {elems, capacidad, capacidad - 1, destr};
  return pila;
}

void pila_destruir(Pila pila) {
  pila_vaciar(pila);
  free(pila->elems);
  free(pila);
}

void pila_push(Pila pila, void* dato) {
  pila->top = (pila->top + 1) % pila->capacidad;
  // Si la pila esta llena, borra el elemento mas viejo.
  if (*top(pila)) pila->destr(*top(pila));
  *top(pila) = dato;
}

void* pila_pop(Pila pila) {
  void* res = *top(pila);
  *top(pila) = NULL;
  pila->top = (pila->top + pila->capacidad - 1) % pila->capacidad;
  return res;
}

void pila_vaciar(Pila pila) {
  while (*top(pila)) {
    pila->destr(pila_pop(pila)); 
  }
}