#include "utils.h"

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