ESTRDIR = src/estructuras
ESTRHEAD = $(ESTRDIR)/heap.h $(ESTRDIR)/pila.h $(ESTRDIR)/tablahash.h 
BUILD_ESTRUCTURAS = build/heap.o build/pila.o build/tablahash.o

CFLAGS = -Wall -Wextra -Werror -std=c99 -g -fno-omit-frame-pointer

agenda_interfaz: build/main.o build/contacto.o build/interfaz.o build/operaciones.o build/historial.o build/utils.o build/archivos.o $(BUILD_ESTRUCTURAS)
	gcc -o $@ $^

clean:
	-@rm -rf build/
	-@rm -f agenda_interfaz
	-@rm -rf tmp/

.PHONY: clean

build/main.o: src/main.c src/contacto.h src/interfaz.h $(ESTRDIR)/tablahash.h $(ESTRDIR)/tipos.h
build/contacto.o: src/contacto.c src/contacto.h src/utils.h
build/interfaz.o: src/interfaz.c src/interfaz.h src/operaciones.h src/historial.h $(ESTRDIR)/tablahash.h $(ESTRDIR)/tipos.h
build/operaciones.o: src/operaciones.c src/operaciones.h src/historial.h src/contacto.h src/archivos.h src/utils.h $(ESTRHEAD)
build/historial.o: src/historial.c src/historial.c src/contacto.h $(ESTRDIR)/pila.h $(ESTRDIR)/tipos.h
build/utils.o: src/utils.c src/utils.h
build/heap.o: $(ESTRDIR)/heap.c $(ESTRDIR)/heap.c $(ESTRDIR)/tipos.h 
build/pila.o: $(ESTRDIR)/pila.c $(ESTRDIR)/pila.c $(ESTRDIR)/tipos.h 
build/tablahash.o: $(ESTRDIR)/tablahash.c $(ESTRDIR)/tablahash.c $(ESTRDIR)/tipos.h

build/%.o: src/%.c
	@mkdir -p build
	gcc $(CFLAGS) -c -o $@ $<

build/%.o: $(ESTRDIR)/%.c
	mkdir -p build
	gcc $(CFLAGS) -c -o $@ $<