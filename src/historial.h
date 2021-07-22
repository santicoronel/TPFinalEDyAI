/** Modulo de historial de operaciones sobre agenda de contactos */

#ifndef __HISTORIAL_H__
#define __HISTORIAL_H__

#include "contacto.h"

typedef struct _Historial* Historial;

/** Tag de operacion */
typedef enum {
  AGREGAR,
  ELIMINAR,
  EDITAR,
} OperacionTag;

/** Estructura de operacion. Tenemos tag y el contacto involucrado. */
typedef struct {
  OperacionTag tag;
  Contacto contacto;
}* Operacion;

/**
 * Crea una operacion con el tag y el contacto dados.
 */
Operacion operacion_crear(OperacionTag tag, Contacto contacto);
/**
 * Crea un historial vacio.
 */
Historial historial_crear();
/**
 * Destruye el historial.
 */
void historial_destruir(Historial historial);
/**
 * Da cuenta de una operacion realizada y la agrega al historial.
 */
void historial_hecho(Historial historial, Operacion operacion);
/**
 * Da cuenta de una operacion deshecha y la agrega al historial.
 */
void historial_deshecho(Historial historial, Operacion operacion);
/**
 * Devuelve la ultima operacion realizada.
 */
Operacion historial_ultimo_hecho(Historial historial);
/**
 * Devuelve la ultima operacion deshecha.
 */
Operacion historial_ultimo_deshecho(Historial historial);
/**
 * Vacia el historial de operaciones realizadas.
 */
void historial_vaciar_hecho(Historial historial);
/**
 * Vacia el historial de operaciones deshechas.
 */
void historial_vaciar_deshecho(Historial historial);

#endif /** __HISTORIAL_H__ */