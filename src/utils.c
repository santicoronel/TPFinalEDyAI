#include "utils.h"

#include <stdlib.h>

unsigned int KRHash(char* s, char* r) {
  unsigned int hashval = 0;
  while (*s != '\0') {
    hashval = *s + 31 * hashval;
  }
  while (*r != '\0') {
    hashval = *r + 37 * hashval;
  }
  return hashval;
}

unsigned int string_to_uint(char* s) {
  return strtoul(s, NULL, 0);
}