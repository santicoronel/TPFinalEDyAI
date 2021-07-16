#ifndef __ARBOL_H__
#define __ARBOL_H__

//NICETOHAVE: hacer autobalanceante

#include "tipos.h"

typedef struct _Arbol* Arbol;

Arbol arbol_crear(FuncionComparadora comparar);
void arbol_insertar(void* dato, Arbol arbol);
void arbol_inorder(Arbol arbol, FuncionVisitante visit, void* extra);
void arbol_destruir(Arbol arbol);



#endif /* __ARBOL_H__ */