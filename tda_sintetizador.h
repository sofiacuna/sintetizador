#ifndef TDA_SINTETIZADOR_H
#define TDA_SINTETIZADOR_H

#include "funciones_modulacion.h"
#include "tramo.h"
#include "lectura_midi.h"

#define MAX_CADENA 100
#define MAX_PARAMETROS 3
#define MAX_ARMONICOS 100

typedef struct sintetizador sint_t;

bool leer_cantidad_armonicos(size_t *n_armonicos,FILE *f);
bool extraer_armonico(float armonico[2], char s[MAX_CADENA]);
bool leer_armonicos(float armonicos[][2], size_t max, FILE *f);
bool leer_fmod(f_mod_t *fmod, float params[3], FILE *f);


sint_t *sint_crear(size_t n_armonicos);
void sint_destruir(sint_t *sint);
sint_t *leer_archivo_sintetizador(FILE *f);

tramo_t *sintetizar_tramo(contenedor_notas_t *notas_leidas, sint_t *sint, float f_m, int pps);



#endif

