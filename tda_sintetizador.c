#include "bibliotecas.h"
#include "funciones_modulacion.h"
#include "tramo.h"
#include "lectura_midi.h"
#include "tda_nota.h"
#include "tda_sintetizador.h"

struct sintetizador{
	size_t n;//cantidad de armonicos 
	float (*armonicos)[2];//vector de armonicos con sus respectivas amplitudes y frecuencias
	f_mod_t fmod_ataque;// funcion de modulacion de ataque
	float params_ataque[MAX_PARAMETROS];//parametros de la funcion ataque
	f_mod_t fmod_sostenido;//funcion de modulacion de sostenido
	float params_sostenido[MAX_PARAMETROS];//parametros de la funcion sostenido
	f_mod_t fmod_decaimiento;//funcion de modulacion de decaimiento
	float params_decaimiento[MAX_PARAMETROS];//parametros de la funcion decaimiento	
};


bool leer_cantidad_armonicos(size_t *n_armonicos,FILE *f){
	char s[MAX_CADENA];

	if(fgets(s,MAX_CADENA,f)==NULL)
		return false;
	*n_armonicos=atoi(s);
	return true;
}


bool extraer_armonico(float armonico[2], char s[MAX_CADENA]) {

    size_t pos_espacio;
    char aux[MAX_CADENA];
    for(pos_espacio = 0; s[pos_espacio]!=' '; pos_espacio++) {
    	aux[pos_espacio]=s[pos_espacio];
        if(s[pos_espacio] == '\0')
            return false;
    }
    aux[pos_espacio]='\0';

    char aux2[MAX_CADENA];

    pos_espacio++;
    size_t i;
    for(i = 0; s[i + pos_espacio]; i++)
        aux2[i] = s[pos_espacio + i];
    aux2[i] = '\0';

    armonico[0] = atof(aux);
    armonico[1] = atof(aux2);

    return true;
}

bool leer_armonicos(float armonicos[][2], size_t max, FILE *f) {
    
    for(size_t i = 0; i < max; i++) {
        char str[MAX_CADENA];
        if(fgets(str, MAX_CADENA,f) == NULL)
            return false;

        if(!extraer_armonico(armonicos[i], str))
            return false;
    }
    return true;
}

bool leer_fmod(f_mod_t *fmod, float params[3], FILE *f){


    char *ptr,*fmod_nombre,s[MAX_CADENA];

    if(fgets(s,MAX_CADENA,f)==NULL)
		return false;

    for(size_t i=0; s[i]; i++){
		if(s[i]=='\n' || s[i]=='\r'){
			s[i]='\0';
		}
	}

    ptr = strtok(s, " ");

    if((fmod_nombre = calloc(1, (1+strlen(s))*sizeof(char)))==NULL)
        return false;
    strcpy(fmod_nombre, s);

    if (ptr != NULL){

        for(size_t i = 0; i<3 || (ptr!=NULL); i++){
            ptr = strtok(NULL, " "); 
            if (ptr != NULL){       
                params[i] = atof(ptr); 
            }
            else
            	params[i]=0;
        }
    }
    

    *fmod=encontrar_funcion(fmod_nombre);
    free(fmod_nombre);
    return true;
}


sint_t *sint_crear(size_t n_armonicos ){

	sint_t *sint=NULL;

	if((sint=malloc(sizeof(sint_t)))==NULL)
		return NULL;
	sint->n=n_armonicos;
	if((sint->armonicos = malloc(sizeof(float[2])*n_armonicos))==NULL){
		sint_destruir(sint);
		return NULL;
	}
	return sint;
}

void sint_destruir(sint_t *sint){

	free(sint->armonicos);
	free(sint);
	sint=NULL;
}

sint_t *leer_archivo_sintetizador(FILE *f){


	f_mod_t fmod_ataque=NULL, fmod_sostenido=NULL, fmod_decaimiento=NULL;
	float params_ataque[3], params_sostenido[3], params_decaimiento[3];
	size_t n_armonicos;	
	

	if(leer_cantidad_armonicos(&n_armonicos,f)==false)
		return NULL;

	float fa[n_armonicos][2];

	if(leer_armonicos(fa,n_armonicos,f)==false)
		return NULL;

	if(leer_fmod(&fmod_ataque,params_ataque,f)==false){
		return NULL;
	}

	if(leer_fmod(&fmod_sostenido,params_sostenido,f)==false){
		return NULL;
	}

	if(leer_fmod(&fmod_decaimiento,params_decaimiento,f)==false){
		return NULL;
	}

	sint_t *sint;

	if((sint=sint_crear(n_armonicos))==NULL)
		return NULL;

	for(size_t i=0;i<n_armonicos;i++){
		for(size_t j=0;j<2;j++)
			sint->armonicos[i][j]=fa[i][j];
	}
	
	sint->fmod_ataque=fmod_ataque;
	for(size_t i=0;i<3;i++)	
		sint->params_ataque[i]=params_ataque[i];
	
	sint->fmod_sostenido=fmod_sostenido;
	for(size_t i=0;i<3;i++)
		sint->params_sostenido[i]=params_sostenido[i];

	sint->fmod_decaimiento=fmod_decaimiento;
	for(size_t i=0;i<3;i++)
		sint->params_decaimiento[i]=params_decaimiento[i];
	

	return sint;
}

tramo_t *sintetizar_tramo(contenedor_notas_t *notas_leidas, sint_t *sint, float f_m, int pps){

	tramo_t *tramo_final, *tramo_aux;
	size_t cantidad_notas=cont_notas_extraer_cantidad_notas(notas_leidas);
	double t0=convertir_t0_midi(pps,cont_notas_extraer_nota(notas_leidas,0)),
			tf=convertir_tf_midi(pps,cont_notas_extraer_nota(notas_leidas,0))+sint->params_decaimiento[0];


	if((tramo_final=tramo_crear_muestreo(t0,tf, f_m, nota_leida_extraer_frecuencia(cont_notas_extraer_nota(notas_leidas,0)), nota_leida_extraer_amplitud(cont_notas_extraer_nota(notas_leidas,0)) , sint->armonicos, sint->n))==NULL){
		return NULL;
	}

	tramo_modular(tramo_final, sint->fmod_ataque, sint->params_ataque, sint->fmod_sostenido, sint->params_sostenido, sint->fmod_decaimiento, sint->params_decaimiento);


	for(size_t i=1; i< cantidad_notas; i++){

		if((tramo_aux=tramo_crear_muestreo(convertir_t0_midi(pps,cont_notas_extraer_nota(notas_leidas,i)), convertir_tf_midi(pps,cont_notas_extraer_nota(notas_leidas,i))+sint->params_decaimiento[0], f_m, nota_leida_extraer_frecuencia(cont_notas_extraer_nota(notas_leidas,i)), nota_leida_extraer_amplitud(cont_notas_extraer_nota(notas_leidas,i)) , sint->armonicos, sint->n))==NULL){
			tramo_destruir(tramo_final);
			return NULL;
		}

		tramo_modular(tramo_aux, sint->fmod_ataque, sint->params_ataque, sint->fmod_sostenido, sint->params_sostenido, sint->fmod_decaimiento, sint->params_decaimiento);
		
		if(tramo_extender(tramo_final,tramo_aux)==false){
			tramo_destruir(tramo_aux);
			tramo_destruir(tramo_final);
			return NULL;
		}
		tramo_destruir(tramo_aux);
	}

	

	return tramo_final;
}







