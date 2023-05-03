#ifndef FUNCIONES_MODULACION_H
#define FUNCIONES_MODULACION_H


#define PI 3.141592653589793

typedef float (*f_mod_t)(double, float[3]);

typedef struct fmodulacion fmod_t;

float f_mod_constant(double t, float params[3]);
float f_mod_linear(double t, float params[3]);
float f_mod_invlinear(double t, float params[3]);
float f_mod_sin(double t, float params[3]);
float f_mod_exp(double t, float params[3]);
float f_mod_invexp(double t, float params[3]);
float f_mod_quartcos(double t, float params[3]);
float f_mod_quartsin(double t, float params[3]);
float f_mod_halfcos(double t, float params[3]);
float f_mod_halfsin(double t, float params[3]);
float f_mod_log(double t, float params[3]);
float f_mod_invlog(double t, float params[3]);
float f_mod_tri(double t, float params[3]);
float f_mod_pulses(double t, float params[3]);
f_mod_t encontrar_funcion(char *fmod_nombre);


#endif