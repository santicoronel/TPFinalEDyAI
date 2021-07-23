#ifndef __OPERACIONES_H__
#define __OPERACIONES_H__

#include "historial.h"
#include "estructuras/tablahash.h"

/**
 * Estructura de entorno de sesion. Tenemos una agenda de contactos
 * y un historial de operaciones.
 */
typedef struct {
  TablaHash agenda;
  Historial historial;
} Entorno;
/**
 * Tipo de resultado obtenido luego de realizar una operacion.
 */
typedef enum {
  EXITO,                        // Exito
  BUSCAR_EXISTE,                // No existe el contacto requerido.
  AGREGAR_EXISTE,               // El contacto a agregar ya existe.
  ELIMINAR_EXISTE,              // No existe el contacto requerido.
  EDITAR_EXISTE,                // No existe el contacto requerido.
  ARCHIVO_ENCONTRADO,           // No se ha encontrado el archivo.
  ARCHIVO_ERROR,                // Error al intentar abrir el archivo.
  CARGAR_COMA,                  // Coma inesperada en el archivo.
  CARGAR_SALTO_LINEA,           // Salto de linea inesperado en el archivo.
  CARGAR_CABECERA,              // Cabecera invalida en el archivo.
  CARGAR_VACIO,                 // Archivo vacio.
  CARGAR_EOF,                   // Final del archivo inesperado.
  CARGAR_LARGO,                 // Dato inesperadamente largo.
  CARGAR_FORMATO,               // Formato de CSV incorrecto.
  DESHACER_VACIO,               // No hay operaciones para deshacer.
  REHACER_VACIO,                // No hay operaciones para rehacer.
  AND_OR_VACIO,                 // Todos los campos vacios.
  GUARDAR_ORDENADO_ATRUBUTO,    // Atributo invalido.
  SUMA_EDADES_CERO,             // La suma no puede ser cero.
  SUMA_EDADES_ENCONTRADO,       // Busqueda infructuosa.
} Resultado;

/**
 * Pide por entrada estandar nombre y apellido del contacto.
 * Busca el contacto en la agenda y lo imrime en pantalla.
 * Informa si no lo encontro, devuelve EXITO en caso contrario.
 */
Resultado buscar(Entorno entorno);
/**
 * Pide por entrada estandar los datos de un contacto nuevo.
 * Agrega el contacto a la agenda. Informa si el contacto ya existia,
 * devuelve EXITO en caso contrario y agrega la operacion al historial.
 */
Resultado agregar(Entorno entorno);
/**
 * Pide por entrada estandar nombre y apellido del contacto.
 * Busca el contacto en la agenda y lo elimina. Informa si no lo encontro,
 * devuelve EXITO en caso contrario y agrega la operacion al historial.
 */
Resultado eliminar(Entorno entorno);
/**
 * Pide por entrada estandar nombre y apellido del contacto.
 * Busca el contacto en la agenda y, de encontrarlo, pide el resto de los
 * datos, lo modifica, agrega la operacion al historial y devuelve EXITO.
 */
Resultado editar(Entorno entorno);
/**
 * Pide por entrada estandar ruta del archivo y carga los datos en la agenda.
 * Para cada contacto, informa si se ha insertado correctamente o si el contacto
 * ya se encontraba en la agenda. Informa si no pudo abrir el archivo o si se
 * interrumpio su lectura, o devuelve EXITO en caso contrario.
 */
Resultado cargar(Entorno entorno);
/**
 * Pide por entrada estandar ruta del archivo. Informa si no pudo abrir
 * el archivo. En caso contrario, guarda los datos de la agenda y devuelve
 * EXITO. 
 */
Resultado guardar(Entorno entorno);
/**
 * De no estar vacio el historial, deshace la ultima operacion realizada.
 */
Resultado deshacer(Entorno entorno);
/**
 * De no estar vacio el historial, rehace la ultima operacion deshecha.
 */
Resultado rehacer(Entorno entorno);
/**
 * Pide por entrada estandar un valor para cada atributo de contacto.
 * En caso de haber uno no vacio, imprime por pantalla los contactos
 * que cumplan con todos los valores requeridos.
 */
Resultado and(Entorno entorno);
 /**
  * Pide por entrada estandar un valor para cada atributo de contacto.
  * En caso de haber uno no vacio, imprime por pantalla los contactos
  * que cumplan con alguno de los valores requeridos.
  */
Resultado or(Entorno entorno);
/**
 * Pide por entrada estandar una ruta de archivo y un atributo de contacto.
 * Informa en caso de no poder abrir el archivo o de ser invalido el atributo.
 * En caso contrario, guarda los datos de la agenda en el archivo ordenados
 * crecientemente por el atributo ingresado.
 */
Resultado guardar_ordenado(Entorno entorno);
/**
 * Pide por entrada estandar un natural. Informa si el valor es 0.
 * En caso contrario, busca un subconjunto de contactos tal que sus edades
 * sumen el numero ingresado. Informa en caso de no encontrarlo, o imprime
 * por pantalla los contactos y devuelve EXITO en caso contrario.
 */
Resultado buscar_suma_edades(Entorno entorno);


#endif /* __OPERACIONES_H__ */