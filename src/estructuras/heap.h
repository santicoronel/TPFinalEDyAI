/** Implementa una cola de prioridad. */

#ifndef __HEAP_H__
#define __HEAP_H__

#include "tipos.h"

typedef struct _Heap* Heap;

/**
 * Crea una cola de prioridad con la capacidad y funcion comparadora dadas.
 * Siempre tendra mayor prioridad el menor elemento.
 */
Heap heap_crear(unsigned int capacidad, 
  FuncionComparadora comp);
/**
 * Destruye el heap.
 */
void heap_destruir(Heap heap);
/**
 * Determina si el heap esta vacio.
 */
int heap_vacio(Heap heap);
/**
 * Inserta un elemento en el heap. Devuelve 1 si se pudo realizar la operacion,
 * 0 en caso contrario.
 */
int heap_insertar(void* dato, Heap heap);
/**
 * Extrae el elemento de mayor prioridad del heap. 
 * Devuelve NULL si el heap esta vacio.
 */
void* heap_extraer(Heap heap);

#endif /** __HEAP_H__ */