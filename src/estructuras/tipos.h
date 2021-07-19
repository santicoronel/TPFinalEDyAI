#ifndef __TIPOS_H__
#define __TIPOS_H__

typedef int (*FuncionComparadora)(void* dato1, void* dato2);
/** Retorna un entero negativo si dato1 < dato2, 0 si son iguales y un entero
 * positivo si dato1 > dato2  */
typedef void (*FuncionDestructora)(void* dato);
/** Libera la memoria alocada para el dato */
typedef unsigned (*FuncionHash)(void* dato);
/** Retorna un entero sin signo para el dato */
typedef void (*FuncionVisitante)(void* dato, void* extra);

typedef void* (*FuncionClave)(void* dato);

#endif /* __TIPOS_H__ */