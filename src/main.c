#include "contacto.h"
#include "interfaz.h"
#include "estructuras/tablahash.h"
#include "utils.h"

#define AGENDA_CAPACIDAD 100

static int comparar_contacto(void* contacto1, void* contacto2) {
  return (contacto_comparar_nombre(contacto1, contacto2) == 0)
    &&  (contacto_comparar_apellido(contacto1, contacto2) == 0);
}

static void destruir_contacto(void* contacto) {
  contacto_destruir((Contacto) contacto);
}

static unsigned int hashear_contacto(void* contacto) {
  return KRHash(((Contacto)contacto)->nombre, ((Contacto)contacto)->apellido);
}

int main() {
  TablaHash agenda = tablahash_crear(
    AGENDA_CAPACIDAD, 
    comparar_contacto, 
    destruir_contacto, 
    hashear_contacto);
  iniciar_interfaz(agenda);
  tablahash_destruir(agenda);
  
  return 0;
}