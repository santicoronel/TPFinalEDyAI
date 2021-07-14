#include "contacto.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"

Contacto contacto_crear(char* nombre, char* apellido, unsigned int edad, 
  char* telefono) {
  Contacto contacto = malloc(sizeof(*contacto));
  assert(contacto);
  *contacto = (struct _Contacto) {nombre, apellido, edad, telefono};
  return contacto;
  }

Contacto contacto_destruir(Contacto contacto) {
  free(contacto->nombre);
  free(contacto->apellido);
  free(contacto->telefono);
  free(contacto);
}

int contacto_coincide(Contacto contacto, char* nombre_apellido[2]) {
  return (strcmp(contacto->nombre, nombre_apellido[0]) == 0) &&
    (strcmp(contacto->apellido, nombre_apellido[1]) == 0); 
}

int contacto_comparar_nombre(Contacto contacto1, Contacto contacto2) {
  return strcmp(contacto1, contacto2) == 0;
}

int contacto_comparar_apellido(Contacto contacto1, Contacto contacto2) {
  return strcmp(contacto1, contacto2) == 0;
}

int contacto_comparar_edad(Contacto contacto1, Contacto contacto2) {
  if (contacto1->edad == contacto2->edad) return 0;
  else return contacto1->edad < contacto2->edad ? -1 : 1; 
}

int contacto_comparar_telefono(Contacto contacto1, Contacto contacto2) {
  return strcmp(contacto1, contacto2) == 0;
}

unsigned int contacto_hashear(Contacto contacto) {
  return KRHash(contacto->nombre, contacto->apellido);
}

void contacto_imprimir(Contacto contacto) {
  printf("{%s,%s,%u,%s}", 
    contacto->nombre, 
    contacto->apellido, 
    contacto->edad, 
    contacto->telefono);
}