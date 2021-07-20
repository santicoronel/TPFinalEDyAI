#include "pila.h"

#include <stdlib.h>
#include <assert.h>

struct _Pila {
  int capacidad;
  void** elems;
  int top;
  FuncionDestructora destr;
};

static void** top(Pila pila) { return pila->elems + pila->top; }

Pila pila_crear(int capacidad, FuncionDestructora destr) {
  Pila pila = malloc(sizeof(*pila)); assert(pila);
  void** elems = malloc(sizeof(*elems) * capacidad);
  *pila = (struct _Pila) {capacidad, elems, capacidad - 1, destr};
  return pila;
}

void pila_destruir(Pila pila) {
  pila_vaciar(pila);
  free(pila->elems);
  free(pila);
}

void pila_push(Pila pila, void* dato) {
  pila->top = (pila->top + 1) % pila->capacidad;
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