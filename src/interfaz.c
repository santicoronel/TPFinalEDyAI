#include "interfaz.h"

#include <stdlib.h>
#include <stdio.h>

#include "operaciones.h"
#include "tablahash.h"
#include "historial.h"

static Entorno entorno_crear(TablaHash tabla) {
  Entorno entorno = (Entorno) {tabla, historial_crear()};
  return entorno;
}

static void entorno_destruir(Entorno entorno) {
  historial_destruir(entorno.historial);
}

static void manejar_resultado(Resultado resultado) {
  switch(resultado) {
  case CARGAR_COMA:
    puts("Coma inesperada, se esperaba un salto de linea");
    break;
  case CARGAR_SALTO_LINEA:
    puts("Salto de linea inesperado, se esperaba una coma");
    break;
  case CARGAR_CABECERA:
    puts("Error de cabecera");
    break;
  case CARGAR_VACIO:
    puts("Archivo vacio");
    break;
  case CARGAR_EOF:
    puts("Final del archivo inesperado.");
    break;
  case CARGAR_LARGO:
    puts("La palabra es demasiado larga. "
      "Deben cargarse nombres, apellidos o telefonos reales");
  case CARGAR_FORMATO:
    puts("Error de formato.");
    break;
  case AGREGAR_EXISTE:
    puts("El contacto ya existe. Pruebe con \'Editar\'");
    break;
  case EDITAR_EXISTE:
    puts("El contacto no existe. Pruebe con \'Agregar\'");
    break;
  default:
    assert(0);
  }
}

static void print_menu() {
  printf("Menu de acciones:\n"
    "1. Buscar\n"
    "2. Agregar\n"
    "3. Eliminar\n"
    "4. Editar\n"
    "5. Cargar\n"
    "6. Guardar\n"
    "7. Deshacer\n"
    "8. Rehacer\n"
    "9. AND\n"
    "10. OR\n"
    "11. Guardar ordenado\n"
    "12. Buscar por suma de edades\n"
    "13. Salir\n");
}

void iniciar_interfaz(TablaHash tabla) {
  Entorno entorno = entorno_crear(tabla);
  print_menu();
  int salir = 0;
  int accion;
  while(!salir) {
    printf("\nSeleccione una accion:\n>");
    scanf("%d", accion);
    switch (accion){
    case 1:
      buscar(entorno);
      break;
    case 2:
      agregar(entorno);
      break;
    case 3:
      eliminar(entorno);
      break;
    case 4:
      editar(entorno);
      break;
    case 5:
      cargar(entorno);
      break;
    case 6:
      guardar(entorno);
      break;
    case 7:
      deshacer(entorno);
      break;
    case 8:
      rehacer(entorno);
      break;
    case 9:
      break;
    case 10:
      break;
    case 11:
      break;
    case 12:
      break;
    case 13:
      salir = 1;
      break;
    default:
      printf("\'%d\' no es un numero de accion valido\n");
      break;
    }
    //TODO: gestionar historial
  }
  entorno_destruir(entorno);
}