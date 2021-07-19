#include "contacto.h"
#include "interfaz.h"
#include "estructuras/tablahash.h"

#define AGENDA_CAPACIDAD 100

static int comparar_contacto(Contacto contacto1, Contacto contacto2) {
  return (contacto_comparar_nombre(contacto1, contacto2) == 0)
    &&  (contacto_comparar_apellido(contacto1, contacto2) == 0);
} 

int main() {
  TablaHash agenda = tablahash_crear(
    AGENDA_CAPACIDAD, 
    comparar_contacto, 
    contacto_destruir, 
    contacto_hashear);
  iniciar_interfaz(agenda);
  tablahash_destruir(agenda);
  
  return 0;
}