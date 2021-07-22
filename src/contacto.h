/** Modulo para estructura de Contacto. */

#ifndef __CONTACTO_H__ 
#define __CONTACTO_H__

#define CANT_ATRIBUTOS 4

/**
 * Tipo que representa los atributos del contacto.
 */
typedef enum {
  NOMBRE,
  APELLIDO,
  EDAD,
  TELEFONO,
} AtributosTag;
/**
 * Estrucuta del contacto. Tenemos strings para nombre, apellido y telefono,
 * y un entero no negativo para la edad.
 */
struct _Contacto {
  char* nombre;
  char* apellido;
  unsigned int edad;
  char* telefono;
};
/** Manejamos los contactos por referecia. */
typedef struct _Contacto* Contacto;

/**
 * Crea un contacto con los atributos dados.
 */
Contacto contacto_crear(char* nombre, char* apellido, unsigned int edad, 
  char* telefono);
/**
 * Libera el espacio de memoria ocupado por el contacto. 
 */
void contacto_destruir(Contacto contacto);
/**
 * Compara los nombres en orden alfabetico.
 */
int contacto_comparar_nombre(Contacto contacto1, Contacto contacto2);
/**
 * Compara los apellidos en orden alfabetico.
 */
int contacto_comparar_apellido(Contacto contacto1, Contacto contacto2);
/**
 * Compara las edades.
 */
int contacto_comparar_edad(Contacto contacto1, Contacto contacto2);
/**
 * Compara los telefonos en orden alfabetico.
 */
int contacto_comparar_telefono(Contacto contacto1, Contacto contacto2);
/**
 * Imprime el contacto en pantalla.
 */
void contacto_imprimir(Contacto contacto);
/**
 * Determina si todos los atributos de los contactos coinciden.
 */
int contacto_and(Contacto contacto1, Contacto contacto2);
/**
 * Determina si algun atributo de los contactos coincide.
 */
int contacto_or(Contacto contacto1, Contacto contacto2);

#endif /** __CONTACTO_H__ */