#include "bibliotecas.h"
#include "onda.h"


double onda(double t, float a, float f, float phi){
	
	return a*sin(2*PI*f*t+phi);
}

void inicializar_muestras(float v[], size_t n){
	for(size_t i=0;i<n;i++){
		v[i]=0;
	}
}


void muestrear_senoidal(float v[], size_t n, double t0, int f_m, float f, float a){
	
	double t_i;

	for(size_t i=0;i<n;i++){
		t_i=t0+(double)i/f_m;
		v[i]+=onda(t_i,a,f,PHI);
	}
}

void muestrear_armonicos(float v[], size_t n, double t0, int f_m, float f, float a, const float fa[][2], size_t n_fa){

	inicializar_muestras(v,n);
	for (size_t i  = 0; i < n_fa; i++){
		muestrear_senoidal(v,n,t0,f_m,f*fa[i][POS_COL_F],a*fa[i][POS_COL_A]);
	}
}

void imprimir_muestras(const float v[], size_t n,double t0,int f_m){
	
	double t_i;

	for(size_t i=0;i<n;i++){
		t_i=t0+(double)i/f_m;
		printf("%f,%f\n",t_i,v[i]);	
	}
}