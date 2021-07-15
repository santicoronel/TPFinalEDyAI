#ifndef __OPERACIONES_H__
#define __OPERACIONES_H__

#include "tipos.h"
#include "tablahash.h"
#include "historial.h"

typedef struct {
  TablaHash tabla;
  Historial historial;
} Entorno;

typedef enum {
  EXITO,
  BUSCAR_EXISTE,
  AGREGAR_EXISTE,
  ELIMINAR_EXISTE,
  EDITAR_EXISTE,
  CARGAR_COMA,
  CARGAR_SALTO_LINEA,
  CARGAR_CABECERA,
  CARGAR_VACIO,
  CARGAR_EOF,
  CARGAR_LARGO,
  CARGAR_FORMATO,
} Resultado;

Resultado buscar(Entorno entorno);
Resultado agregar(Entorno entorno);
Resultado eliminar(Entorno entorno);
Resultado editar(Entorno entorno);
Resultado cargar(Entorno entorno);
Resultado guardar(Entorno entorno);


#endif /* __OPERACIONES_H__ */