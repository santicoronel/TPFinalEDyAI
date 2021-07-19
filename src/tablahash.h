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
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 */
void tablahash_insertar(TablaHash tabla, void *dato);

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void* tablahash_buscar(TablaHash tabla, void *clave);

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void* tablahash_eliminar(TablaHash tabla, void *clave);

void tablahash_recorrer(TablaHash tabla, FuncionVisitante visit, void* extra);

#endif /* __TABLA_HASH__ */
