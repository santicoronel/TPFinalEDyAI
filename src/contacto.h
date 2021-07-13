#ifndef __CONTACTO_H__
#define __CONTACTO_H__

struct _Contacto {
  char* nombre;
  char* apellido;
  unsigned int edad;
  char* telefono;
};

typedef struct _Contacto* Contacto;

Contacto contacto_crear(char* nombre, char* apellido, unsigned int edad, 
  char* telefono);

Contacto contacto_destruir(Contacto contacto);

int contacto_comparar_nombre(Contacto contacto1, Contacto contacto2);

int contacto_comparar_apellido(Contacto contacto1, Contacto contacto2);

int contacto_comparar_edad(Contacto contacto1, Contacto contacto2);

int contacto_comparar_telefono(Contacto contacto1, Contacto contacto2);

unsigned int contacto_hashear(Contacto contacto);

void contacto_imprimir(Contacto contacto);

#endif /* __CONTACTO_H__ */