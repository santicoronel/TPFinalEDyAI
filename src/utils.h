#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * Funcion de hash para strings propuesta por Kernighan & Ritchie en "The C
 * Programming Language (Second Ed.)".
 */
unsigned int KRHash(char* s, char* r);

unsigned int string_to_uint(char* s);

void input_string(char* s, int strlen);

#endif /* __UTILS_H__ */