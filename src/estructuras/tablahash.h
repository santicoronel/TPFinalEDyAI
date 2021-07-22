/** Implementa una tabla hash general. */

#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__

#include "tipos.h"

typedef struct _TablaHash *TablaHash;

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned int capacidad, FuncionComparadora comp,
  FuncionDestructora destr, FuncionHash hash);
/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla);
/**
 * Devuelve la cantidad de elementos de la tabla.
 */
unsigned int tablahash_nelems(TablaHash tabla);
/**
 * Inserta un dato en la tabla si este no se encontraba aun.
 * Devuelve 1 si se inserto elemento y 0 si el contacto ya existia. 
 */
int tablahash_insertar(TablaHash tabla, void *dato);
/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void* tablahash_buscar(TablaHash tabla, void *dato);
/**
 * Quita el elemento de la tabla que coincida con el dato dado y lo devuelve.
 */
void* tablahash_eliminar(TablaHash tabla, void *dato);
/**
 * Recorre la tabla hash, visitando cada elemento.
 */
void tablahash_recorrer(TablaHash tabla, FuncionVisitante visit, void* extra);

#endif /* __TABLA_HASH__ */
