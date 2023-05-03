#ifndef ESCRITURA_WAVE_H
#define ESCRITURA_WAVE_H

#include "tramo.h"

bool escribir_encabezado_wave(size_t n_muestras, FILE *f);
bool escribir_fmt_subchunk_wave(uint32_t sample_rate,FILE *f);
bool escribir_data_subchunk_wave(int16_t *muestras, size_t n_muestras,FILE *f);
bool escribir_archivo_wave(tramo_t *tramo_final, int f_m, FILE *f);

#endif