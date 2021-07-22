/** Implementa historial de operaciones utilizando dos pilas estaticas. */

#include "historial.h"

#include <stdlib.h>
#include <assert.h>

#include "estructuras/pila.h"

#define CAPACIDAD 20 // Capacidad del historial.

/** 
 * Estructura del historial. 
 * Tenemos una pila para las operaciones realizadas y una para las deshechas.
 */
struct _Historial {
  Pila hecho;
  Pila deshecho;
};

Operacion operacion_crear(OperacionTag tag, Contacto contacto) {
  Operacion operacion = malloc(sizeof(*operacion)); assert(operacion);
  operacion->tag = tag; operacion->contacto = contacto;
  return operacion;
}
/**
 * Destruye el registro de la operacion realizada sin perjudicar la integridad
 * de los datos.
 */
static void hecho_destruir(void* operacion) {
  switch(((Operacion)operacion)->tag) {
    case AGREGAR: // Si el contacto fue agregado a la tabla, no lo destruimos.
      break;
    case EDITAR:
    case ELIMINAR:
      contacto_destruir(((Operacion)operacion)->contacto);
      break;
  }
  free(operacion);
}
/**
 * Destruye el registro de la operacion deshecha sin perjudicar la integridad
 * de los datos.
 */
static void deshecho_destruir(void* operacion) {
  operacion = (Operacion)operacion;
  switch(((Operacion)operacion)->tag) {
    case AGREGAR:
    case EDITAR:
      contacto_destruir(((Operacion)operacion)->contacto);
      break;
    case ELIMINAR: // Si el contacto fue agregado nuevamente, no lo destruimos.
      break;
  }
  free(operacion);
}

Historial historial_crear() {
  Historial historial = malloc(sizeof(*historial)); assert(historial);
  historial->hecho = pila_crear(CAPACIDAD, hecho_destruir);
  historial->deshecho = pila_crear(CAPACIDAD, deshecho_destruir);
  return historial;
}

void historial_destruir(Historial historial) {
  pila_destruir(historial->hecho);
  pila_destruir(historial->deshecho);
  free(historial);
} 

void historial_hecho(Historial historial, Operacion operacion) {
  pila_push(historial->hecho, operacion);
}

void historial_deshecho(Historial historial, Operacion operacion) {
  pila_push(historial->deshecho, operacion);
}

Operacion historial_ultimo_hecho(Historial historial) {
  return pila_pop(historial->hecho);
}

Operacion historial_ultimo_deshecho(Historial historial) {
  return pila_pop(historial->deshecho);
}

void historial_vaciar_hecho(Historial historial) {
  pila_vaciar(historial->hecho);
}

void historial_vaciar_deshecho(Historial historial) {
  pila_vaciar(historial->deshecho);
}