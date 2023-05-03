#ifndef TRAMO_H
#define TRAMO_H 

#include "funciones_modulacion.h"
#include "tda_nota.h"


typedef struct tramo tramo_t;


tramo_t *_tramo_crear(double t0, double tf, int f_m); 
void tramo_destruir(tramo_t *t);
tramo_t *tramo_clonar(const tramo_t *t);
tramo_t *tramo_crear_muestreo(double t0, double tf, int f_m, float f, float a, const float fa[][2], size_t n_fa);
bool tramo_redimensionar(tramo_t *t, double tf);
bool tramo_extender(tramo_t *destino, const tramo_t *extension);
int tramo_extraer_fm(tramo_t *tramo);


void tramo_modular(tramo_t *tramo, float (*fmod_ataque) (double, float *), float params_ataque[3], float (*fmod_sostenido) (double, float *),float params_sostenido[3], float (*fmod_decaimiento) (double, float *), float params_decaimiento[3] );

double convertir_t0_midi(int pps, nota_leida_t *nota_leida);
double convertir_tf_midi(int pps, nota_leida_t *nota_leida);

int16_t *tramo_normalizar_muestras(tramo_t *tramo, size_t *n);
void destruir_muestras_norm(int16_t *muestras_norm);


#endif
