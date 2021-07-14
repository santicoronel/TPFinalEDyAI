#ifndef __PILA_H__
#define __PILA_H__

#include "tipos.h"

typedef struct _Pila* Pila;

Pila pila_crear(int capacidad, FuncionDestructora destr);
void pila_destruir(Pila pila);
void pila_push(Pila pila, void* dato);
void* pila_pop(Pila pila);
void pila_vaciar(Pila pila);

#endif /* __PILA_H__ */