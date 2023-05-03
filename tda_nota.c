#include "bibliotecas.h"
#include "deco_cod.h"
#include "tda_nota.h"

 struct nota_leida{
	double t0;//tiempo de inicio de la nota >0
	double tf;//tiempo final de la nota >t0
	float f;//frecuencia de la nota
	float a;//amplitud de la nota
	nota_t nota;//valor de la nota
	signed char octava;//valor de la octava

};


nota_leida_t *nota_leida_crear(float velocidad,double t0, nota_t nota, signed char octava){
	
	nota_leida_t *nota_leida;

	if((nota_leida=malloc(sizeof(nota_leida_t)))==NULL)
		return NULL;
	nota_leida->t0=t0;
	nota_leida->tf=0;
	nota_leida->nota=nota;
	nota_leida->octava=octava;
	nota_leida->a=velocidad;
	nota_leida->f=nota_leida_calcular_f(nota,octava);

	return nota_leida;
}

void nota_leida_destruir(nota_leida_t *nota_leida){
	free(nota_leida);
	nota_leida=NULL;
}

void nota_leida_cerrar(nota_leida_t *nota_leida, double tf){
	nota_leida->tf=tf;
}

double nota_leida_calcular_f(nota_t nota,signed char octava){

	int o = octava-4, s= nota-9;	
	return 440*pow(2,o+(float)s/12);
}

nota_t nota_leida_extraer_nota(nota_leida_t *nota_leida){
	return nota_leida->nota;
}

signed char nota_leida_extraer_octava(nota_leida_t *nota_leida){
	return nota_leida->octava;
}

double nota_leida_extraer_t0midi(nota_leida_t *nota_leida){
	return nota_leida->t0;
}
double nota_leida_extraer_tfmidi(nota_leida_t *nota_leida){
	return nota_leida->tf;
}

float nota_leida_extraer_frecuencia(nota_leida_t *nota_leida){
	return nota_leida->f;
}
float nota_leida_extraer_amplitud(nota_leida_t *nota_leida){
	return nota_leida->a;
}










