 #ifndef TDA_NOTA_H
#define TDA_NOTA_H

#include "deco_cod.h"


typedef struct nota_leida nota_leida_t;

nota_leida_t *nota_leida_crear(float velocidad,double t0, nota_t nota, signed char octava);
void nota_leida_destruir(nota_leida_t *nota_leida);
void nota_leida_cerrar(nota_leida_t *nota_leida, double tf);
double nota_leida_calcular_f(nota_t nota,signed char octava);
nota_t nota_leida_extraer_nota(nota_leida_t *nota_leida);
signed char nota_leida_extraer_octava(nota_leida_t *nota_leida);
double nota_leida_extraer_t0midi(nota_leida_t *nota_leida);
double nota_leida_extraer_tfmidi(nota_leida_t *nota_leida);
float nota_leida_extraer_frecuencia(nota_leida_t *nota_leida);
float nota_leida_extraer_amplitud(nota_leida_t *nota_leida);

#endif