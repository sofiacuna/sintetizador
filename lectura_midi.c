#include "bibliotecas.h"
#include "endianness.h"
#include "deco_cod.h"
#include "tda_nota.h"
#include "lectura_midi.h"




struct contenedor {
    size_t n;//cantidad de notas almacenadas
    nota_leida_t *v[MAX_NOTAS];//notas leidas de un canal del midi y almacenadas
};


contenedor_notas_t *leer_archivo_midi(char canal_in,FILE *f) {


    // LECTURA DEL ENCABEZADO:
    formato_t formato;
    uint16_t numero_pistas;
    uint16_t pulsos_negra;
    contenedor_notas_t *cont_notas;
    size_t i=0,j=0;

    if((cont_notas=cont_notas_crear())==NULL){
    	return NULL;
    }

    if(! leer_encabezado(f, &formato, &numero_pistas, &pulsos_negra)){
    	cont_notas_destruir(cont_notas);
        return NULL;
    }

    // ITERAMOS LAS PISTAS:
    for(uint16_t pista = 0; pista < numero_pistas; pista++) {
        // LECTURA ENCABEZADO DE LA PISTA:
        uint32_t tamagno_pista;
        if(! leer_pista(f, &tamagno_pista)) {
        	cont_notas_destruir(cont_notas);
            return NULL;
        }

        evento_t evento;
        char canal;
        int longitud;
        uint32_t tiempo = 0;

        // ITERAMOS LOS EVENTOS:
        while(1) {
            uint32_t delta_tiempo;
            leer_tiempo(f, &delta_tiempo);
            tiempo += delta_tiempo;

            // LECTURA DEL EVENTO:
            uint8_t buffer[EVENTO_MAX_LONG];
            if(! leer_evento(f, &evento, &canal, &longitud, buffer)) {
            	cont_notas_destruir(cont_notas);
                return NULL;
            }

            // PROCESAMOS EL EVENTO:
            if(evento == EVT_METAEVENTO && canal == 0xF) {
                // METAEVENTO:
                if(buffer[METAEVENTO_TIPO] == METAEVENTO_FIN_DE_PISTA)
                    break; 
                descartar_metaevento(f, buffer[METAEVENTO_LONGITUD]);
            }
                 
            else if (evento == EVT_NOTA_ENCENDIDA || evento == EVT_NOTA_APAGADA) {
                // NOTA:
                nota_t nota;
                signed char octava;
                 
                if(! decodificar_nota(buffer[EVNOTA_NOTA], &nota, &octava)){
                    return NULL;
                }

                if(canal==canal_in){

                
                    if( evento==EVT_NOTA_ENCENDIDA && buffer[EVNOTA_VELOCIDAD]!=0){
                        
                        if(((cont_notas->v[i] = nota_leida_crear(buffer[EVNOTA_VELOCIDAD],tiempo,nota,octava)))==NULL){
                            cont_notas_destruir(cont_notas);
                            return NULL;
                        }
                        i++;    
                    }

                    else if((evento==EVT_NOTA_APAGADA) || (evento==EVT_NOTA_ENCENDIDA && buffer[EVNOTA_VELOCIDAD]==0)){
                        for(j =1; j <=i ; j++ ){
                            if(nota_leida_extraer_nota(cont_notas->v[i-j])==nota && nota_leida_extraer_octava(cont_notas->v[i-j])==octava && nota_leida_extraer_tfmidi(cont_notas->v[i-j])==0){  
                                nota_leida_cerrar(cont_notas->v[i-j],tiempo);
                                break;    
                            }      
                        }
                    }
            }   }
        }
    }
    cont_notas->n=i;

    return cont_notas;
}

contenedor_notas_t *cont_notas_crear(void){

	contenedor_notas_t *cont_notas;

	if((cont_notas=malloc(sizeof(contenedor_notas_t)))==NULL)
		return NULL;
	
	return cont_notas;
}

void cont_notas_destruir(contenedor_notas_t *cont_notas){
	for(size_t i=0;i<cont_notas->n;i++){
        nota_leida_destruir(cont_notas->v[i]);
    }
    free(cont_notas);
}

nota_leida_t *cont_notas_extraer_nota(contenedor_notas_t *cont_notas, size_t i){
    nota_leida_t *nota;

    nota=cont_notas->v[i];

    return nota;
 }

size_t cont_notas_extraer_cantidad_notas(contenedor_notas_t *cont_notas){
    return cont_notas->n;
}

bool leer_encabezado(FILE *f, formato_t *formato, uint16_t *numero_pistas, uint16_t *pulsos_negra){

 	
 	if(leer_uint32_big_endian(f)!=HEADER_ID_VALUE)
 		return false;
 	if(leer_uint32_big_endian(f)!=HEADER_SIZE)
 		return false;
 	if(decodificar_formato(leer_uint16_big_endian(f),formato)==false)
 		return false;
 	*numero_pistas=leer_uint16_big_endian(f);
 	*pulsos_negra=leer_uint16_big_endian(f);

 	return true;
}

bool leer_pista(FILE *f, uint32_t *tamagno){
 	
    if(leer_uint32_big_endian(f)!=TRACK_ID_VALUE)
 		return false;
 	*tamagno=leer_uint32_big_endian(f);
 	return true;
}

bool leer_tiempo(FILE *f, uint32_t *tiempo){

 	uint8_t aux;
 	*tiempo=0;

 	for(size_t i=0; i < MAX_BYTES_TIEMPO; i++){
 		aux=leer_uint8_t(f);
 		*tiempo=((*tiempo<<(7))|(aux & MASK_TIEMPO));
 		if(!(aux & MASK_TIEMPO_MSB))
 			break;
 	}
	
 	return true;
}

bool leer_evento(FILE *f, evento_t *evento, char *canal, int *longitud, uint8_t mensaje[]){

 	uint8_t aux;

 	aux=leer_uint8_t(f);
 	if(decodificar_evento(aux, evento, canal, longitud)==true){
 		for(size_t i=0;i <*longitud;i++){
 			mensaje[i]=leer_uint8_t(f);
 		}
 	}
 	else{
 		mensaje[0]=aux;
 		for(size_t i=1;i <*longitud;i++){
 			mensaje[i]=leer_uint8_t(f);
 		}
 	}
 	return true;
}

void descartar_metaevento(FILE *f, uint8_t tamagno){

 	for(size_t i=0;i<tamagno;i++){
 		leer_uint8_t(f);
 	} 	
}

