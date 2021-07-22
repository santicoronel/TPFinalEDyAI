/** Tipos utilizados en las estructuras de datos. */

#ifndef __TIPOS_H__
#define __TIPOS_H__

/** Retorna un entero negativo si dato1 < dato2, 0 si son iguales y un entero
 * positivo si dato1 > dato2  */
typedef int (*FuncionComparadora)(void* dato1, void* dato2);
/** Libera la memoria alocada para el dato */
typedef void (*FuncionDestructora)(void* dato);
/** Retorna un entero sin signo para el dato */
typedef unsigned (*FuncionHash)(void* dato);
/** Visita el elemento, utilizando un dato extra en la llamada */
typedef void (*FuncionVisitante)(void* dato, void* extra);

#endif /** __TIPOS_H__ */