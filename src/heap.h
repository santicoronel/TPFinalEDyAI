#ifndef __HEAP_H__
#define __HEAP_H__

#include "tipos.h"

typedef struct _Heap* Heap;

Heap heap_crear(unsigned int capacidad, 
  FuncionComparadora comp);

void heap_destruir(Heap heap);

int heap_vacio(Heap heap);

int heap_insertar(void* dato, Heap heap);

void* heap_extraer(Heap heap);

#endif /* __HEAP_H__ */