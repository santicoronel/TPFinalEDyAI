#include "arbol.h"

#include <stdlib.h>
#include <assert.h>

typedef struct _Nodo {
  void* dato;
  Nodo izq, der;
}* Nodo;

struct  _Arbol {
  Nodo raiz;
  FuncionComparadora comp;
};

Arbol arbol_crear(FuncionComparadora comparar) {
  Arbol arbol = malloc(sizeof(*arbol)); assert(arbol);
  *arbol = (struct _Arbol) {NULL, comparar};
  return arbol;
}

static Nodo nodo_insertar(Nodo nodo, void* dato, FuncionComparadora comp) {
  if (nodo == NULL) {
    Nodo nodo_nuevo = malloc(sizeof(*nodo_nuevo)); assert(nodo_nuevo);
    *nodo_nuevo = (struct _Nodo) {dato, NULL, NULL};
    return nodo_nuevo;
  }
  if (comp(nodo->dato, dato) < 0) return nodo_insertar(nodo->izq, dato, comp);
  return nodo_insertar(nodo->der, dato, comp);
}

void arbol_insertar(Arbol arbol, void* dato) {
  arbol->raiz = nodo_insertar(arbol->raiz, dato, arbol->comp);
}

static void nodo_inorder(Nodo nodo, FuncionVisitante visit, void* extra) {
  if (nodo == NULL) return;
  nodo_inorder(nodo->izq, visit, extra);
  visit(nodo->dato, extra); 
  nodo_inorder(nodo->der, visit, extra);
}

void arbol_inorder(Arbol arbol, FuncionVisitante visit, void* extra) {
  nodo_inorder(arbol->raiz, visit, extra);
}

static void nodo_destruir(Nodo nodo) {
  if (nodo == NULL) return;
  nodo_destruir(nodo->izq);
  nodo_destruir(nodo->der);
  free(nodo);
}

void destruir(Arbol arbol) {
  nodo_destruir(arbol->raiz);
  free(arbol);
}