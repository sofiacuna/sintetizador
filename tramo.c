#include "bibliotecas.h"
#include "tda_sintetizador.h"
#include "tda_nota.h"
#include "onda.h"
#include "tramo.h"


struct tramo{
    float *v;//muestras generadas por tramo
    size_t n;//cantidad de muestras
    double t0;// tiempo inicial del tramo
    int f_m;// frecuencia de muestreo
};

tramo_t *_tramo_crear(double t0, double tf, int f_m){

	tramo_t *t=NULL;

	if((t=malloc(sizeof(tramo_t)))==NULL)
		return NULL;
	t->n=(tf-t0)*f_m+1;
	t->t0=t0;
	t->f_m=f_m;
	if((t->v=malloc(t->n*sizeof(float)))==NULL){
		tramo_destruir(t);
		return NULL;
	}
	
	return t;
} 

void tramo_destruir(tramo_t *t){

	if(t->v!=NULL)
		free(t->v);
	free(t);
	t=NULL;
}

tramo_t *tramo_clonar(const tramo_t *t){
	
	tramo_t *copia=NULL;
	double tf=(double)(t->n-1)/t->f_m+t->t0;

	
	if((copia=_tramo_crear(t->t0, tf, t->f_m))==NULL)
		return NULL;
	memcpy(copia->v, t->v, copia->n*sizeof(float));
	copia->n=t->n;
	copia->t0=t->t0;
	copia->f_m=t->f_m;

	return copia;
}

tramo_t *tramo_crear_muestreo(double t0, double tf, int f_m, float f, float a, const float fa[][2], size_t n_fa){

	tramo_t *tramo_muestreo=NULL;

	if((tramo_muestreo=_tramo_crear(t0,tf,f_m))==NULL)
		return NULL;
	muestrear_armonicos(tramo_muestreo->v, tramo_muestreo->n, t0, f_m, f, a, fa, n_fa);

	return tramo_muestreo;
}

bool tramo_redimensionar(tramo_t *t, double tf){

	float *aux=NULL;
	size_t n_redim=(tf-t->t0)*t->f_m+1;


	if(n_redim==t->n)
		return true;

	if((aux=realloc(t->v,n_redim*sizeof(float)))==NULL)
		return false;
	
	for(size_t i=t->n; i<n_redim; i++){
		aux[i]=0;
	}
	
	t->v=aux;
	t->n=n_redim;
	
	return true;
}

bool tramo_extender(tramo_t *destino, const tramo_t *extension){

	double  tf_destino=(double)(destino->n-1)/destino->f_m+destino->t0,
			tf_extension=(double)(extension->n-1)/extension->f_m+extension->t0;
	size_t n_desfasaje=(extension->t0 - destino->t0)*destino->f_m-1;

	if(destino->t0 > extension->t0)
		return false;
	else if(tf_extension <= tf_destino){
		
		for(size_t i=0; i < extension->n; i++){
			destino->v[i + n_desfasaje]+=extension->v[i];
		}
	}
	else if(tf_extension > tf_destino){

		if(tramo_redimensionar(destino,tf_extension)==false)
			return false;
		for(size_t i=0; i < extension->n; i++){
			destino->v[i+n_desfasaje]+=extension->v[i];
		}
	}
	return true;
}

void tramo_modular(tramo_t *tramo, float (*fmod_ataque) (double, float *), float params_ataque[3], float (*fmod_sostenido) (double, float *),float params_sostenido[3], float (*fmod_decaimiento) (double, float *), float params_decaimiento[3] ){
	
	size_t i; 

	for(i=0; i<tramo->n;i++){

		double t_i=tramo->t0+(double)i/tramo->f_m;

		if(t_i< tramo->t0+params_ataque[0])
			tramo->v[i]=fmod_ataque(t_i-tramo->t0, params_ataque)*tramo->v[i];
		
		else if(t_i>tramo->t0+params_ataque[0] && t_i<tramo->t0+(double)tramo->n/tramo->f_m)
			tramo->v[i]=fmod_sostenido(t_i-(tramo->t0+params_ataque[0]), params_sostenido)*tramo->v[i];
		
		else if(t_i>tramo->t0+(double)tramo->n/tramo->f_m )
			tramo->v[i]=fmod_sostenido(tramo->t0+((double)tramo->n-1)/tramo->f_m, params_sostenido)*fmod_decaimiento(t_i-(tramo->t0+((double)tramo->n-1)/tramo->f_m), params_decaimiento)*tramo->v[i];
		else
			tramo->v[i]=0;
	}	
}

double convertir_t0_midi(int pps, nota_leida_t *nota_leida){

	double t0=(double)nota_leida_extraer_t0midi(nota_leida)/pps;

	return t0;
}

double convertir_tf_midi(int pps, nota_leida_t *nota_leida){

	double tf=(double)nota_leida_extraer_tfmidi(nota_leida)/pps;

	return tf;
}

int16_t *tramo_normalizar_muestras(tramo_t *tramo, size_t *n){

	float valor_max=tramo->v[0];
	int16_t *muestras_norm;


	for(size_t i=1; i<tramo->n; i++){
		if(tramo->v[i] > valor_max)
			valor_max=tramo->v[i];
	}

	if((muestras_norm=malloc(tramo->n*sizeof(int16_t)))==NULL)
		return NULL;

	for(size_t i=0; i < tramo->n; i++){
		*(muestras_norm+i)=(double)tramo->v[i]*pow(2,15)/valor_max;
	}
	*n=tramo->n;
	return muestras_norm;
}

void destruir_muestras_norm(int16_t *muestras_norm){
	free(muestras_norm);
	muestras_norm=NULL;
}









