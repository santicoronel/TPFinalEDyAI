/** Implementa una pila general. */

#ifndef __PILA_H__
#define __PILA_H__

#include "tipos.h"

typedef struct _Pila* Pila;

/**
 * Crea una pila con la capacidad y funcion destructoras dadas.
 * La capacidad es estatica y a medidad que se inserten nuevos elementos
 * se eliminaran los mas viejos.
 */
Pila pila_crear(int capacidad, FuncionDestructora destr);
/**
 * Destruye la pila.
 */
void pila_destruir(Pila pila);
/**
 * Inserta un elemento en el tope de la pila.
 */
void pila_push(Pila pila, void* dato);
/**
 * Quita el elemento del tope de la pila y lo devuelve.
 */
void* pila_pop(Pila pila);
/**
 * Vacia la pila, destruyendo todos sus elementos.
 */
void pila_vaciar(Pila pila);

#endif /** __PILA_H__ */