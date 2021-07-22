/** Biblioteca de funciones auxiliares. */ 

#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * Funcion de hash para strings propuesta por Kernighan & Ritchie en "The C
 * Programming Language (Second Ed.)".
 */
unsigned int KRHash(char* s, char* r);
/**
 * Funcion que toma un string y lo transforma a un entero no negativo.
 */
unsigned int string_to_uint(char* s);
/**
 * Pide por entrada estandar el valor dado y retorna el string ingresado.
 */
char* ingresar_string(const char* valor, int strlen);

#endif /* __UTILS_H__ */