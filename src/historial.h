#ifndef __HISTORIAL_H__
#define __HISTORIAL_H__

#include "contacto.h"

typedef struct _Historial* Historial;

typedef enum {
  AGREGAR,
  ELIMINAR,
  EDITAR,
} OperacionTag;

typedef struct {
  OperacionTag tag;
  Contacto contacto;
}* Operacion;


Historial historial_crear();

void historial_destruir(Historial historial);

void historial_hecho(Historial historial, OperacionTag tag, Contacto contacto);

void historial_deshecho(Historial historial, OperacionTag tag, Contacto contacto);

Operacion historial_ultimo_hecho(Historial historial);

Operacion historial_ultimo_deshecho(Historial historial);

void historial_vaciar_deshecho(Historial historial);

#endif /* __HISTORIAL_H__ */