#include "interfaz.h"

#include <stdlib.h>
#include <stdio.h>

#include "contacto.h"
#include "utils.h"

#define STRLEN 50

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

static void buscar(TablaHash tabla) {
  printf("Ingrese nombre:\n>");
  char nombre[STRLEN]; fgets(nombre, STRLEN, stdin);
  printf("Ingrese nombre:\n>");
  char apellido[STRLEN]; fgets(apellido, STRLEN, stdin);
  char* nombre_apellido[2] = {nombre, apellido};
  Contacto res = tablahash_buscar(tabla, nombre_apellido);
  free(nombre); free(apellido);
  if (res) contacto_imprimir(res);
  else puts("No se ha encontrado el contacto.");
}

static void agregar(TablaHash tabla) {
  Contacto contacto = malloc(sizeof(*contacto));
  printf("Ingrese nombre:\n>");
  fgets(contacto->nombre, STRLEN, stdin);
  printf("Ingrese apellido:\n>");
  fgets(contacto->apellido, STRLEN, stdin);
  printf("Ingrese edad:\n>");
  scanf("%u", &contacto->edad);
  printf("Ingrese telefono:\n>");
  fgets(contacto->telefono, STRLEN, stdin);
  tablahash_insertar(tabla, contacto);
}

static void eliminar(TablaHash tabla) {
  printf("Ingrese nombre:\n>");
  char nombre[STRLEN]; fgets(nombre, STRLEN, stdin);
  printf("Ingrese apellido:\n>");
  char apellido[STRLEN]; fgets(apellido, STRLEN, stdin);
  char* nombre_apellido[2] = {nombre, apellido};
  tablahash_eliminar(tabla, nombre_apellido);
}

static void editar(TablaHash tabla) {
  printf("Ingrese nombre:\n>");
  char nombre = malloc(STRLEN);
  fgets(nombre, STRLEN, stdin);
  printf("Ingrese apellido:\n>");
  char apellido = malloc(STRLEN);
  fgets(apellido, STRLEN, stdin);
  char* nombre_apellido[2] = {nombre, apellido};
  Contacto res = tablahash_buscar(tabla, nombre_apellido);
  if (res) {
  printf("Ingrese edad:\n>");
  scanf("%u", &res->edad);
  printf("Ingrese telefono:\n>");
  fgets(res->telefono, STRLEN, stdin);
  }
  else puts("No se ha encontrado el contacto.");
}

static void cargar(TablaHash tabla) {
  
}

void iniciar_interfaz(TablaHash tabla) {
  print_menu();
  int salir = 0;
  int accion;
  while(!salir) {
    printf("\nSeleccione una accion:\n>");
    scanf("%d", accion);
    switch (accion)
    {
    case 1:
      buscar(tabla);
      break;
    case 2:
      agregar(tabla);
      break;
    case 3:
      eliminar(tabla);
      break;
    case 4:
      editar(tabla);
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      break;
    case 8:
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
  }
}