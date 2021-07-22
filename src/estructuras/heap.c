#include "heap.h"

#include <stdlib.h>
#include <assert.h>

/**
 * Estructura del heap. Tenemos un array de elementos, la capacidad del heap,
 * la posicion del ultimo elemento y nuestra funcion comparadora.
 */
struct _Heap {
  void** elems;
  unsigned int capacidad;
  unsigned int ultimo;
  FuncionComparadora comp;
};

Heap heap_crear(unsigned int capacidad, 
  FuncionComparadora comp) {
  Heap heap = malloc(sizeof(*heap)); assert(heap);
  // Reservamos un espacio de mas para dejar el 0 vacio.
  void** elems = malloc(sizeof(*elems) * (capacidad + 1)); assert(elems);
  elems[0] = NULL;
  // Si no hay ningun elemento, decimos que el ultimo es 0.
  *heap = (struct _Heap) {elems, capacidad, 0, comp};
  return heap;
}

void heap_destruir(Heap heap) {
  free(heap->elems); free(heap);
}

int heap_vacio(Heap heap) {
  return !heap->ultimo;
}

int heap_insertar(void* dato, Heap heap) {
  if (heap->ultimo == heap->capacidad) return 0;  // El heap esta lleno
  // Aumentamos el ultimo en 1 e insertamos el nuevo elemento al final.
  heap->elems[++heap->ultimo] = dato;
  unsigned int i = heap->ultimo;
  // El nuevo elemento trepa hasta una posicion adecuada.
  while (i > 1 && heap->comp(dato, heap->elems[i / 2]) < 0) {
    heap->elems[i] = heap->elems[i / 2];  
    i /= 2; heap->elems[i] = dato;
  }
  return 1;
}

void* heap_extraer(Heap heap) {
  // Extraemos el primer elemento y lo reemplazamos con el ultimo.
  void* top = heap->elems[1];
  heap->elems[1] = heap->elems[heap->ultimo--];
  
  unsigned int j = 1; int esMayor = 1; void* temp;
  FuncionComparadora comp = heap->comp;
  void** elems = heap->elems;
  // El primer elemento se hunde hasta una posicion adecuada.
  while (j * 2 <= heap->ultimo && esMayor) {
    unsigned int hijo = j * 2;
    if (hijo + 1  <= heap->ultimo && comp(elems[hijo], elems[hijo + 1]) > 0)
      hijo++;
    if (comp(heap->elems[j], heap->elems[hijo]) > 0) {
      temp = elems[j];
      elems[j] = elems[hijo];
      elems[j = hijo] = temp;
    }
    else esMayor = 0;
  }
  return top;
}