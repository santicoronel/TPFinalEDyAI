#ifndef __OPERACIONES_H__
#define __OPERACIONES_H__

#include "tipos.h"
#include "historial.h"
#include "estructuras/tablahash.h"

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
  DESHACER_VACIO,
  REHACER_VACIO,
  AND_OR_ATRIBUTOS,
  AND_OR_VACIO,
  GUARDAR_ORDENADO_ATRUBUTO,
  SUMA_EDADES_CERO,
  SUMA_EDADES_ENCONTRADO,
} Resultado;

Resultado buscar(Entorno entorno);
Resultado agregar(Entorno entorno);
Resultado eliminar(Entorno entorno);
Resultado editar(Entorno entorno);
Resultado cargar(Entorno entorno);
Resultado guardar(Entorno entorno);
Resultado deshacer(Entorno entorno);
Resultado rehacer(Entorno entorno);
Resultado and(Entorno entorno);
Resultado or(Entorno entorno);
Resultado guardar_ordenado(Entorno entorno);
Resultado buscar_suma_edades(Entorno entorno);


#endif /* __OPERACIONES_H__ */