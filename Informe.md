# EDyA I -- TP Final -- Santiago Coronel

## Modulos del proyecto.

### [Contacto.](src/contacto.h)
Modulo propio de la estructura de un contacto. Ademas de las funcionalidades basicas, se implementan algunas funciones que seran utiles para llevar a cabo las operaciones sobre la agenda.

### [Interfaz.](src/interfaz.h)
El proposito de este modulo es interpretar las instrucciones del usuario y operar sobre la agenda de contactos. Decidi tener un paso extra entre las operaciones de la tabla y el [main](src/main.c) ya que no me parecio sensato que la agenda de contacto este atada a la sesion con el usuario, y que esta se destruya cuando la sesion se termine.

### [Operaciones.](src/operaciones.h)
Este modulo incluye todas las operaciones a realizar sobre la agenda dadas por consigna. Cada operacion devuelve un valor de tipo _Resultado_, que luego es interpretado por el modulo **Interfaz**.

### [Historial.](src/historial.h)
Este modulo tiene como proposito manejar el registro de las operaciones _realizadas, deshechas y rehechas_. Para esto utiliza dos pilas, y un tipo de dato **Operacion**. La unica complejidad que aporta este modulo es abstraer la estrucutura de datos utilizada al funcionamiento del propio historial, y manejar la destruccion de los datos almacenados en la pila. Esto ultimo es necesario ya que decidi no copiar los datos para agregarlos al Historial, para ahorrar espacio de memoria y costo computacional.

### [Archivos.](src/archivos.h)
Modulo que manipula archivos externos. Implementamos operaciones de escritura y de lectura, estas ultimas devolviendo un valor acorde al estado del interprete luego de procesar un valor (numero o cadena de caracteres).

### [Utils.](src/utils.h)
Este modulo aporta funcionalidades auxiliares.


## Estructuras de datos.
Se implementan 3 estructuras de datos.

### [Tabla Hash.](src/estructuras/tablahash.h)
En primer lugar, utilizamos una **Tabla Hash** general para almacenar los contactos de la agenda. Se utiliza como clave nombre y apellido del contacto.
Resolvemos las colisiones con _linear probing_ y usamos un valor centinela para los datos eliminados. Cuando insertamos, chequeamos que el elemento no exista aun y, en caso de no encontrarlo, lo insertamos en el primer espacio vacio (o eliminado) que encontremos (si es que se da una colision).

### [Pila.](src/estructuras/pila.h)
Implementamos una **Pila** general para llevar un registro de las operaciones realizadas y deshechas. Ya que las cantidad de operaciones a recordar es fija, utilizamos un arreglo de tamaño fijo, y borramos los elementos mas viejos a medida que vamos quedando sin lugar.

### [Heap.](src/estructuras/heap.h)
Finalmente, utilizamos una **Cola de prioridad** general como estructura auxiliar, implementada con un **Heap**. La usamos para ordenar los contactos en caso de que sea necesario. Ya que siempre sabemos la cantidad de contactos de antemano, su tamaño puede ser fijo.


## Algoritmos y estrategias.

### [Deshacer/Rehacer.](src/operaciones.c)
Para resolver estas operaciones, implementamos una pila. En esta guardamos el tipo de operacion junto al contacto involucrado (contacto agregado, contacto eliminado, o contacto anterior a ser editado). Luego de deshacer una operacion, esta se coloca en el tope de la pila de operaciones deshechas; de esta manera podremos rehacerla si se requiere, en ese caso devolviendola a la pila de operaciones realizadas. El registro de operaciones deshechas se elimina cuando se vuelve a realizar una operacion, y el de operaciones realizadas unicamente cuando se realice la operacion **Cargar**, ya que esta interrumpiria con el determinismo a la hora de deshacer/rehacer. Hay cierta ingenieria a la hora de borrar operaciones de las pilas, ya sea cuando queremos vaciarla o destruirla al terminar la sesion.
Por ejemplo, sobre la pila de operaciones realizadas los contactos agregados no deben ser eliminados, ya que permanecen en la tabla; caso contrario para los contactos agregados en la pila de operaciones deshechas.

### [Guardar por suma de edades.](src/operaciones.c)
Decidi utilizar un heap para resolver esta operacion. Tal vez lo mas logico habria sido utilizar un algoritmo de ordenamiento, como Mergesort, por ejemplo. Pero vi la oportunidad de utilizar una de las estructuras de datos dadas en la materia y me parecio una buenda opcion, ya que de todas formas se necesita espacio auxiliar para ordenar los contactos (no se puede hacer sobre la tabla hash). Teoricamente podria haber utilizado cualquier arbol binario, pero tengo la ventaja de saber cuantos contactos tengo que ordenar, por lo que puedo usar una estructura de tamaño fijo. Ademas, sobre el heap las operaciones de insercion y extraccion se realizan siempre en tiempo logaritmico.

### [Buscar por suma de edades.](src/operaciones.c)
Para resolver este problema utilice **Programacion Dinamica**. La idea era llevar un registro del conjunto de contactos tal que sus edades suman cualquier subsuma, desde 1 hasta la suma objetivo. Es decir, teoricamente, una tabla de dimensiones ```cantidad de contactos * suma```. De todas formas, en la practica solo se necesitan dos filas de esta tabla, disminuyendo el espacio de memoria utilizado drasticamente: considerando el espacio de memoria utilizado para llevar registro de los contactos para cada subsuma, el espacio total utilizado esta acotado por ```cantidad de contactos * suma * 2```. Ademas, se plantean algunas optimizaciones ajenas al funcionamiento del algoritmo en si: ordenar los contactos de mayor a menor, y chequear si la suma total de las edades es inferior a la suma objetivo. Dependiendo de la cualidad de los datos y el contexto dado, se puede determinar si vale la pena realizar estos calculos previos.

## Dificultades.
No encontre mayores dificultades mas alla de las presentadas por la consigna. Es decir, decidir que estructuras usar, su implementacion, y decidir sobre la forma y modularizacion del programa. Dentro de las estructuras de datos, la Tabla Hash fue la que mas me costo, pero creo que finalmente pude resolver bien el tema de las colisiones. Creo que la parte algoritmica fue en la que menos dificultad encontre.

## Compilar e invocar el programa.
Para compilar basta correr [```make```](Makefile) sobre el directorio del proyecto. A partir de esto se genera un subdirectorio ```build/``` con archivos auxiliares para el compilado, y un ejecutable ```agenda_interfaz```. Basta correr ```make clean``` para remover los resultados del compilado.

### Tests.
Se pueden correr los archivos [```correr_mem_tests.sh```](correr_mem_tests.sh) y [```correr_tests.sh```](correr_tests.sh) para correr los tests de uso de memoria dinamica y tests sobre el funcionamiento del programa, respectivamente. Notese que debe compilarse el proyecto previamente.

## Bibliografia consultada.
Material brindado por la catedra.

---

### Observaciones sobre el trabajo.
- **Funcionamiento de operaciones AND/OR**: los valores para cada atributo se piden secuencialmente; si se quiere dejar un campo vacio, se ingresa literalmente _vacio_.
- **Estructuras generales**: tal como se comenta en relacion a otras decisiones, la idea fue utilizar los conceptos trabajados en su mayor extension posible. Por esto es que las estructuras de datos se definen generales, por mas que en alguno o todos los casos podria hacerse una implementacion especifica.  
