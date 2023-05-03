#ifndef ONDA_H
#define ONDA_H

#define PI 3.141592653589793
#define PHI 0
#define POS_COL_F 0
#define POS_COL_A 1

double onda(double t, float a, float f, float phi);
void inicializar_muestras(float v[], size_t n);
void muestrear_senoidal(float v[], size_t n, double t0, int f_m, float f, float a);
void muestrear_armonicos(float v[], size_t n, double t0, int f_m, float f, float a, const float fa[][2], size_t n_fa);
void imprimir_muestras(const float v[], size_t n, double t0, int f_m);

#endif