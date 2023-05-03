CFLAGS=-Wall -std=c99 -pedantic -g -c 

sintetizador: main.o lectura_midi.o tramo.o tda_sintetizador.o tda_nota.o escritura_wave.o funciones_modulacion.o deco_cod.o endianness.o onda.o
	gcc main.o lectura_midi.o tramo.o tda_sintetizador.o tda_nota.o escritura_wave.o funciones_modulacion.o deco_cod.o endianness.o onda.o -o sintetizador -lm

main.o: main.c bibliotecas.h errors.h lectura_midi.h tramo.h tda_sintetizador.h escritura_wave.h 
	gcc $(CFLAGS) main.c 

lectura_midi.o: lectura_midi.c bibliotecas.h endianness.h tda_nota.h deco_cod.h lectura_midi.h
	gcc $(CFLAGS) lectura_midi.c

tramo.o: tramo.c bibliotecas.h tda_sintetizador.h tda_nota.h onda.h tramo.h
	gcc $(CFLAGS) tramo.c

tda_sintetizador.o: tda_sintetizador.c bibliotecas.h funciones_modulacion.h tramo.h lectura_midi.h tda_nota.h tda_sintetizador.h 
	gcc $(CFLAGS) tda_sintetizador.c

tda_nota.o: tda_nota.c bibliotecas.h deco_cod.h tda_nota.h   
	gcc $(CFLAGS) tda_nota.c

escritura_wave.o: escritura_wave.c bibliotecas.h tramo.h endianness.h escritura_wave.h
	gcc $(CFLAGS) escritura_wave.c

funciones_modulacion.o: funciones_modulacion.c bibliotecas.h funciones_modulacion.h
	gcc $(CFLAGS) funciones_modulacion.c 

deco_cod.o: deco_cod.c bibliotecas.h deco_cod.h
	gcc $(CFLAGS) deco_cod.c

endianness.o: endianness.c bibliotecas.h endianness.h 
	gcc $(CFLAGS) endianness.c

onda.o: onda.c bibliotecas.h onda.h 
	gcc $(CFLAGS) onda.c 

clean:
	rm *.o 













