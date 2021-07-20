#include "interfaz.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "operaciones.h"
#include "historial.h"
#include "estructuras/tablahash.h"

static Entorno entorno_crear(TablaHash tabla) {
  Entorno entorno = (Entorno) {tabla, historial_crear()};
  return entorno;
}

static void entorno_destruir(Entorno entorno) {
  historial_destruir(entorno.historial);
}

static void manejar_resultado(Resultado resultado) {
  switch(resultado) {
  case EXITO:
    break;
  case AGREGAR_EXISTE:
    puts("Ya existe un contacto con ese nombre.");
    break;
  case BUSCAR_EXISTE:
  case ELIMINAR_EXISTE:
  case EDITAR_EXISTE:
    puts("No se ha encontrado el contacto.");
    break;
  case  ARCHIVO_ENCONTRADO:
    puts("Archivo no encontrado");
    break;
  case ARCHIVO_ERROR:
    puts("Error al abrir el archivo.");
    break;
  case CARGAR_COMA:
    puts("Coma inesperada, se esperaba un salto de linea.");
    break;
  case CARGAR_SALTO_LINEA:
    puts("Salto de linea inesperado, se esperaba una coma.");
    break;
  case CARGAR_CABECERA:
    puts("No se pudo leer el archivo: error de cabecera.");
    break;
  case CARGAR_VACIO:
    puts("Archivo vacio.");
    break;
  case CARGAR_EOF:
    puts("Final del archivo inesperado.");
    break;
  case CARGAR_LARGO:
    puts("La palabra es demasiado larga. "
      "Deben cargarse nombres, apellidos o telefonos reales.");
  case CARGAR_FORMATO:
    puts("Error de formato.");
    break;
  case DESHACER_VACIO:
    puts("No hay operaciones para deshacer.");
    break;
  case REHACER_VACIO:
    puts("No hay operaciones para rehacer.");
    break;
  case AND_OR_ATRIBUTOS:
    puts("Atributo invalido.");
    break;
  case AND_OR_VACIO:
    puts("Debe haber al menos un valor no vacio.");
    break;
  case GUARDAR_ORDENADO_ATRUBUTO:
    puts("Atributo invalido.");
    break;
  case SUMA_EDADES_CERO:
    puts("El numero debe ser distinto a 0");
  case SUMA_EDADES_ENCONTRADO:
    puts("Busqueda infructuosa.");
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
    scanf("%d", &accion); getchar();
    switch (accion){
    case 1:
      manejar_resultado(buscar(entorno));
      break;
    case 2:
      manejar_resultado(agregar(entorno));
      break;
    case 3:
      manejar_resultado(eliminar(entorno));
      break;
    case 4:
      manejar_resultado(editar(entorno));
      break;
    case 5:
      manejar_resultado(cargar(entorno));
      break;
    case 6:
      manejar_resultado(guardar(entorno));
      break;
    case 7:
      manejar_resultado(deshacer(entorno));
      break;
    case 8:
      manejar_resultado(rehacer(entorno));
      break;
    case 9:
      manejar_resultado(and(entorno));
      break;
    case 10:
      manejar_resultado(or(entorno));
      break;
    case 11:
      manejar_resultado(guardar_ordenado(entorno));
      break;
    case 12:
      manejar_resultado(buscar_suma_edades(entorno));
      break;
    case 13:
      salir = 1;
      break;
    default:
      printf("\'%d\' no es un numero de accion valido\n");
      break;
    }
    if (accion != 7 && accion != 8) 
      historial_vaciar_deshecho(entorno.historial);
  }
  //entorno_destruir(entorno);
}