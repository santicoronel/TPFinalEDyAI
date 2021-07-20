#include "contacto.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

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

int contacto_comparar_nombre(Contacto contacto1, Contacto contacto2) {
  return strcmp(contacto1->nombre, contacto2->nombre);
}

int contacto_comparar_apellido(Contacto contacto1, Contacto contacto2) {
  return strcmp(contacto1->apellido, contacto2->apellido);
}

int contacto_comparar_edad(Contacto contacto1, Contacto contacto2) {
  if (contacto1->edad == contacto2->edad) return 0;
  else return contacto1->edad < contacto2->edad ? -1 : 1; 
}

int contacto_comparar_telefono(Contacto contacto1, Contacto contacto2) {
  return strcmp(contacto1->telefono, contacto2->telefono);
}

unsigned int contacto_hashear(Contacto contacto) {
  return KRHash(contacto->nombre, contacto->apellido);
}

void contacto_imprimir(Contacto contacto) {
  printf("{%s,%s,%u,%s}\n", 
    contacto->nombre, 
    contacto->apellido, 
    contacto->edad, 
    contacto->telefono);
}

int contacto_and(Contacto contacto1, Contacto contacto2) {
  if ((contacto2->nombre != NULL) 
    && (strcmp(contacto1->nombre, contacto2->nombre) != 0)) return 0; 
  if ((contacto2->apellido != NULL) 
    && (strcmp(contacto1->apellido, contacto2->apellido) != 0)) return 0;
  if ((contacto2->edad != -1) 
    && (contacto1->edad != contacto2->edad)) return 0; 
  if ((contacto2->telefono != NULL) 
    && (strcmp(contacto1->telefono, contacto2->telefono != 0))) return 0; 

  return 1;
}

int contacto_or(Contacto contacto1, Contacto contacto2) {
  if ((contacto2->nombre != NULL) 
    && (strcmp(contacto1->nombre, contacto2->nombre) == 0)) return 1; 
  if ((contacto2->apellido != NULL) 
    && (strcmp(contacto1->apellido, contacto2->apellido) == 0)) return 1;
  if ((contacto2->edad != -1) 
    && (contacto1->edad == contacto2->edad)) return 1; 
  if ((contacto2->telefono == NULL) 
    && (strcmp(contacto1->telefono, contacto2->telefono) == 0)) return 1; 
    
  return 0;
}