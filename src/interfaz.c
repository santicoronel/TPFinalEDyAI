#include "interfaz.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "contacto.h"
#include "utils.h"
#include "archivos.h"

#define STRLEN 50

typedef enum {
  AGREGAR_EXISTE,
  EDITAR_EXISTE,
  CARGAR_COMA,
  CARGAR_SALTO_LINEA,
  CARGAR_CABECERA,
  CARGAR_VACIO,
  CARGAR_EOF,
  CARGAR_LARGO,
  CARGAR_FORMATO,
} Excepcion;


static void manejar_excepcion(Excepcion excepcion) {
  switch(excepcion) {
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
  char* nombre = malloc(sizeof(STRLEN)); assert(nombre);
  printf("Ingrese nombre:\n>");
  fgets(nombre, STRLEN, stdin);
  char* apellido = malloc(sizeof(STRLEN)); assert(apellido);
  printf("Ingrese apellido:\n>");
  fgets(apellido, STRLEN, stdin);
  char* nombre_apellido[2] = {nombre, apellido};
  if (tablahash_buscar(tabla, nombre_apellido)){
    manejar_excepcion(AGREGAR_EXISTE); 
    return;
  }
  unsigned int edad;
  printf("Ingrese edad:\n>");
  scanf("%u", &edad);
  char* telefono = malloc(sizeof(STRLEN)); assert(telefono);
  printf("Ingrese telefono:\n>");
  fgets(telefono, STRLEN, stdin);
  
  Contacto contacto = contacto_crear(nombre, apellido, edad, telefono);
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
  char nombre[STRLEN]; fgets(nombre, STRLEN, stdin);
  printf("Ingrese apellido:\n>");
  char apellido[STRLEN]; fgets(apellido, STRLEN, stdin);
  char* nombre_apellido[2] = {nombre, apellido};
  Contacto contacto = tablahash_buscar(tabla, nombre_apellido);
  if (contacto) {
    printf("Ingrese edad:\n>");
    scanf("%u", &contacto->edad);
    printf("Ingrese nombre:\n>");
    fgets(contacto->telefono, STRLEN, stdin);
  }
  else manejar_excepcion(EDITAR_EXISTE);
}

static int cabecera_valida(FILE* fp) {
  char keywords[] = {"nombre", "apellido", "edad", "telefono"};
  char input[STRLEN];
  for (int i = 0; i < 4; i++) {
    switch (leer_palabra(fp, STRLEN, input)) {
    case COMA:
      if (i == 3) {
        manejar_excepcion(CARGAR_COMA); return 0;
      }
      if ((strcmp(input, keywords[i]) != 0)) {
        manejar_excepcion(CARGAR_CABECERA); return 0;
      }
      break;
    case SALTO_LINEA:
      if (i != 3) {
        manejar_excepcion(CARGAR_SALTO_LINEA); return 0;
      }
      if ((strcmp(input, keywords[i]) != 0)) {
        manejar_excepcion(CARGAR_CABECERA); return 0;
      }
      break;
    case FINAL:
      if (i == 0) {
        manejar_excepcion(CARGAR_VACIO); return 0;
      }
      //fallthrough
    case ERROR_EOF:
      manejar_excepcion(CARGAR_EOF); return 0;
      break;
    case ERROR_LARGO:
      manejar_excepcion(CARGAR_LARGO); return 0;
      break;
    case ERROR_FORMATO:
      manejar_excepcion(ERROR_FORMATO); return 0;
      break;
    }
  }
  return 1;
}

static void cargar(TablaHash tabla) {
  char ruta[STRLEN];
  printf("Ingrese ruta de entrada:\n>");
  scanf("%s", ruta);
  FILE* fp = fopen(ruta, "r"); assert(fp);
  if (!cabecera_valida(fp)) {
    fclose(fp); return;
  }
  char* input[4] = {NULL, NULL, NULL, NULL}; 
  unsigned int edad;
  Resultado res; int seguir = 1;
  while(seguir) {
    for (int i = 0; i < 4; i++) {
      if (i == 2) res = leer_numero(fp, &edad);
      else {
        input[i] = malloc(STRLEN); assert(input);
        res = leer_palabra(fp, STRLEN, leer_palabra);
      }
      switch(res) {
      case COMA:
        if (i == 3) {
          manejar_excepcion(CARGAR_COMA); seguir = 0;
        }
      case SALTO_LINEA:
        if (i != 3) {
          manejar_excepcion(CARGAR_SALTO_LINEA); seguir = 0;
        }
      case FINAL:
        if (i == 0) {
          seguir = 0; break;
        }
      // falthrough
      case ERROR_EOF:
        manejar_excepcion(CARGAR_EOF); seguir = 0;
        break;
      case ERROR_LARGO:
        manejar_excepcion(CARGAR_LARGO); seguir = 0;
        break;
      case ERROR_FORMATO:
        manejar_excepcion(ERROR_LARGO); seguir = 0;
        break;
      }
    }
    if (seguir) {
      tablahash_insertar(tabla, 
        contacto_crear(input[0], input[1], edad, input[3]));
        input[0] = NULL; input[1] = NULL; input[3] = NULL;
    }
  }
  free(input[0]); free(input[1]); free(input[3]);

  fclose(fp);
}

static void guardar(TablaHash tabla) {
  char ruta[STRLEN];
  printf("Ingrese ruta de entrada:\n>");
  scanf("%s", ruta);
  FILE* fp = fopen(ruta, "r"); assert(fp);
  escribir_cabecera(fp);
  tablahash_recorrer(tabla, escribir_contacto, fp);
}

void iniciar_interfaz(TablaHash tabla) {
  print_menu();
  int salir = 0;
  int accion;
  while(!salir) {
    printf("\nSeleccione una accion:\n>");
    scanf("%d", accion);
    switch (accion){
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
      cargar(tabla);
      break;
    case 6:
      guardar(tabla);
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