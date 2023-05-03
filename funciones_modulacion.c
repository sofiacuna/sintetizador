#include "bibliotecas.h"
#include "funciones_modulacion.h"



struct fmodulacion{
    char *nombre_fmod;
    f_mod_t funcion;
};

static const fmod_t funciones[] = {
    				{"CONSTANT",f_mod_constant},
					{"LINEAR",f_mod_linear},
					{"INVLINEAR",f_mod_invlinear},
					{"SIN",f_mod_sin},
					{"EXP",f_mod_exp},
					{"INVEXP",f_mod_invexp},
					{"QUARTCOS",f_mod_quartcos},
					{"QUARTSIN",f_mod_quartsin},
					{"HALFCOS",f_mod_halfcos},
					{"HALFSIN",f_mod_halfsin},
					{"LOG",f_mod_log},
					{"INVLOG",f_mod_invlog},
					{"TRI",f_mod_tri},
					{"PULSES",f_mod_pulses}
};



float f_mod_constant(double t, float params[3]){
	return 1;
}

float f_mod_linear(double t, float params[3]){
	return t/params[0];
}

float f_mod_invlinear(double t, float params[3]){
	float t_mod= 1- t/params[0];

	if( t_mod> 0 )
		return t_mod; 
	return 0;
}

float f_mod_sin(double t, float params[3]){
	return 1+params[0]*sin(params[1]*t);
}

float f_mod_exp(double t, float params[3]){
	return exp(5*(t-params[0])/params[0]);
}

float f_mod_invexp(double t, float params[3]){
	return exp(-5*t/params[0]);
}

float f_mod_quartcos(double t, float params[3]){
	return cos((PI*t)/(2*params[0]));
}

float f_mod_quartsin(double t, float params[3]){
	return sin((PI*t)/(2*params[0]));
}

float f_mod_halfcos(double t, float params[3]){
	return ((1+cos(PI*t/params[0]))/2);
}

float f_mod_halfsin(double t, float params[3]){
	return (1+sin(PI*(t/params[0]-0.5)))/2;
}

float f_mod_log(double t, float params[3]){
	return log10(9*t/params[0]+1);
}

float f_mod_invlog(double t, float params[3]){
	if(t<params[0])
		return log10(-9*t/params[0]+10);
	return 0;
}

float f_mod_tri(double t, float params[3]){
	if(t<=params[1])
		return (t*params[2])/params[1];
	return (t-params[1])*(params[2]-1)/(params[1]-params[0])+params[2];
}

float f_mod_pulses(double t, float params[3]){
	
	float  t_prima=(t/params[0]-(int)t/params[0])*params[0],
			value=fabs((1-params[2])*(t_prima-params[0]+params[1])/params[1]+params[2]);

	if(value<1)
		return value; 
	return 1;
}


f_mod_t encontrar_funcion(char *fmod_nombre){

	f_mod_t funcion_mod=NULL;
	
	size_t i;

    for( i = 0; i < sizeof(funciones) / sizeof(funciones[0]); i++) {
        if(! strcmp(funciones[i].nombre_fmod,fmod_nombre )) {
            funcion_mod = funciones[i].funcion;
            break;
        }
    }
    return funcion_mod;
}

