#include "historial.h"

#include <stdlib.h>
#include <assert.h>

#include "estructuras/pila.h"

#define N 20

struct _Historial {
  Pila hecho;
  Pila deshecho;
};

Operacion operacion_crear(OperacionTag tag, Contacto contacto) {
  Operacion operacion = malloc(sizeof(*operacion)); assert(operacion);
  operacion->tag = tag; operacion->contacto = contacto;
  return operacion;
}

static void hecho_destruir(Operacion operacion) {
  switch(operacion->tag) {
    case AGREGAR:
      break;
    case EDITAR:
    case ELIMINAR:
      contacto_destruir(operacion->contacto);
      break;
  }
  free(operacion);
}

static void deshecho_destruir(Operacion operacion) {
  switch(operacion->tag) {
    case AGREGAR:
    case EDITAR:
      contacto_destruir(operacion->contacto);
      break;
    case ELIMINAR:
      break;
  }
  free(operacion);
}

Historial historial_crear() {
  Historial historial = malloc(sizeof(*historial)); assert(historial);
  historial->hecho = pila_crear(N, hecho_destruir);
  historial->deshecho = pila_crear(N, deshecho_destruir);
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
  pila_push(historial->hecho, operacion);
}

Operacion historial_ultimo_hecho(Historial historial) {
  return pila_pop(historial->hecho);
}

Operacion historial_ultimo_deshecho(Historial historial) {
  return pila_pop(historial->deshecho);
}

void historial_vaciar_deshecho(Historial historial) {
  pila_vaciar(historial->deshecho);
}