#include "utils.h"

#include <stdlib.h>
#include <stdio.h>

unsigned int KRHash(char* s, char* r) {
  unsigned int hashval = 0;
  while (*s != '\0') hashval = *s++ + 31 * hashval;
  while (*r != '\0') hashval = *r++ + 37 * hashval;
  return hashval;
}

unsigned int string_to_uint(char* s) {
  return strtoul(s, NULL, 0);
}

void input_string(char* s, int strlen) {
  int c, i;
  for (i = 0; i < strlen - 1 && ((c = getchar()) != '\n'); i++) s[i] = c; 
  while (c != '\n') c = getchar();
  s[i] = '\0';
}