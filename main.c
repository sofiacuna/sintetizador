#include <stdio.h>
#include <stdlib.h>

#include "bibliotecas.h"
#include "errors.h"
#include "lectura_midi.h"
#include "tramo.h"
#include "tda_sintetizador.h"
#include "escritura_wave.h"

int main(int argc, char *argv[]){
	
	if(argc < 7 ){
	    fprintf(stderr,"%s\n",MSJ_USR_ERR_PARAM);
        return EXIT_FAILURE;
    }
    char canal=0, *arch_sint, *arch_mid, *arch_wav;
    int f_m=44100;
    int pps=500;

    //LECTURA DE PARAMETROS 


    for(size_t i = 1; i < argc-1; i++){
        if(!strcmp(argv[i],"-s")){
            arch_sint=argv[i+1];
        }
        if(!strcmp(argv[i],"-i")){
            arch_mid=argv[i+1];
        }
        if(!strcmp(argv[i],"-o")){
            arch_wav=argv[i+1];
        }
        if(!strcmp(argv[i],"-c")){
            canal=atoi(argv[i+1]);
        }
        if(!strcmp(argv[i],"-f")){
            f_m=atoi(argv[i+1]);
        }
        if(!strcmp(argv[i],"-r")){
            pps=atoi(argv[i+1]);
        }
    }


    FILE *f_sint,*f_midi,*f_wave; 

    //APERTURA DE ARCHIVO MIDI
    if((f_midi=fopen(arch_mid,"rb")) == NULL) {
        fprintf(stderr, "%s \"%s\"\n",MSJ_USR_ERR_APERTURA, arch_mid);
        return EXIT_FAILURE;
    }

    contenedor_notas_t *notas_leidas;

    //LECTURA ARCHIVO MIDI
    if((notas_leidas=leer_archivo_midi(canal,f_midi))==NULL){
        fclose(f_midi);
        fprintf(stderr, "%s\n", MSJ_USR_ERR_LECTURA_MIDI);
        return EXIT_FAILURE;
    }
    fclose(f_midi);


    //APERTURA DE ARCHIVO SINTETIZADOR
    if((f_sint=fopen(arch_sint,"rt")) == NULL) {
	    fprintf(stderr, "%s \"%s\"\n",MSJ_USR_ERR_APERTURA, arch_sint);
        return EXIT_FAILURE;
    }

    

    //LECTURA DE ARCHIVO SINTETIZADOR
    sint_t *sint;

    if((sint=leer_archivo_sintetizador(f_sint))==NULL){
        fclose(f_sint);
        fprintf(stderr,"%s\n",MSJ_USR_ERR_LECTURA_SINT);
        return EXIT_FAILURE;
    }
    fclose(f_sint);

    
    tramo_t *tramo_final;


    if((tramo_final=sintetizar_tramo(notas_leidas, sint, f_m, pps))==NULL){
        cont_notas_destruir(notas_leidas);
        sint_destruir(sint);
        fprintf(stderr, "%s\n", MSJ_USR_ERR_SINTETIZADOR);
        return EXIT_FAILURE;
    }

    //APERTURA DE ARCHIVO WAVE
    if((f_wave=fopen(arch_wav,"wb")) == NULL) {
        cont_notas_destruir(notas_leidas);
        sint_destruir(sint);
        tramo_destruir(tramo_final);
	    fprintf(stderr, "%s \"%s\"\n",MSJ_USR_ERR_APERTURA, arch_wav);
	    return EXIT_FAILURE;
    }

    //ESCRITURA DE ARCHIVO WAVE
    if(escribir_archivo_wave(tramo_final, f_m,f_wave)==false){
        cont_notas_destruir(notas_leidas);
        sint_destruir(sint);
        tramo_destruir(tramo_final);
        fprintf(stderr, "%s\n", MSJ_USR_ERR_ESCRITURA);
        fclose(f_wave);
        return EXIT_FAILURE;
    }
    
    fclose(f_wave);

    cont_notas_destruir(notas_leidas);
    
    sint_destruir(sint);
    tramo_destruir(tramo_final);
    

	return EXIT_SUCCESS;
}