#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__

#define FACTOR_CARGA_INI 30
#define FACTOR_CARGA_MAX 70

#include "tipos.h"

struct _TablaHash {
  void** elems;
  unsigned int capacidad;
  unsigned int nelems;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

typedef struct _TablaHash *TablaHash;

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned int capacidad, FuncionComparadora comp, 
  FuncionDestructora destr, FuncionHash hash);

/**
 * Retorna el numero de elementos de la tabla.
 */
unsigned int tablahash_nelems(TablaHash tabla);

/**
 * Retorna la capacidad de la tabla.
 */
unsigned int tablahash_capacidad(TablaHash tabla);

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
void* tablahash_buscar(TablaHash tabla, void *dato);

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void* tablahash_eliminar(TablaHash tabla, void *dato);

void tablahash_recorrer(TablaHash tabla, FuncionVisitante visit, void* extra);

#endif /* __TABLA_HASH__ */
