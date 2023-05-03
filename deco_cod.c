#include "bibliotecas.h"
#include "deco_cod.h"

static const int longitud_eventos[]={
	[EVT_NOTA_APAGADA] = 2,
    [EVT_NOTA_ENCENDIDA] = 2,
    [EVT_NOTA_DURANTE] = 2,
    [EVT_CAMBIO_CONTROL] = 2,
    [EVT_CAMBIO_PROGRAMA]= 1,
    [EVT_VARIAR_CANAL] = 1,
    [EVT_CAMBIO_PITCH] = 2,
    [EVT_METAEVENTO] = 2
};

static const char *formato_cadena[] = {
    [FMT_PISTA_UNICA] = "Pista unica",
    [FMT_MULTIPISTAS_SINC] = "Multipistas sincronicas",
    [FMT_MULTIPISTAS_ASINC] = "Multipistas asincronicas"
};

static const char *eventos_cadena[] = {
    [EVT_NOTA_APAGADA] = "nota apagada",
    [EVT_NOTA_ENCENDIDA] = "nota encendida",
    [EVT_NOTA_DURANTE] = "nota durante",
    [EVT_CAMBIO_CONTROL] = "cambio de control",
    [EVT_CAMBIO_PROGRAMA]="cambio de programa",
    [EVT_VARIAR_CANAL] = "variar canal",
    [EVT_CAMBIO_PITCH] = "cambio de pitch",
    [EVT_METAEVENTO] = "metaevento"
};

static const char *notas_cadena[] = {
    [NOTA_DO] = "C",
    [NOTA_DO_SOSTENIDO] = "C#",
    [NOTA_RE] = "D",
    [NOTA_RE_SOSTENIDO] = "D#",
    [NOTA_MI] = "E",
    [NOTA_FA] = "F",
    [NOTA_FA_SOSTENIDO] = "F#",
    [NOTA_SOL]="G",
    [NOTA_SOL_SOSTENIDO]="G#",
    [NOTA_LA]="A",
    [NOTA_LA_SOSTENIDO]="A#",
    [NOTA_SI]="B"
};


bool decodificar_formato(uint16_t valor, formato_t *formato){
    if(valor > MAX_VALOR_FORMATO)
        return false;
    *formato=valor;

    return true;
}

bool decodificar_evento(uint8_t valor, evento_t *evento, char *canal, int *longitud){
    
    if(!(valor & MASK_VALIDACION))
        return false;
    *evento=(valor & MASK_EVENTO) >> SHIFT_EVENTO;
    *canal=valor & MASK_CANAL;
    *longitud=longitud_eventos[*evento];

    return true;
}

bool decodificar_nota(uint8_t valor, nota_t *nota, signed char *octava){

    if(valor & MASK_VALIDACION){
        return false;
    }
    *nota = valor%CANT_NOTAS;
    *octava = valor/CANT_NOTAS-1;
    
    return true;
}



const char *codificar_formato(formato_t formato){
    return formato_cadena[formato];
}
const char *codificar_evento(evento_t evento){
    return eventos_cadena[evento];
}
const char *codificar_nota(nota_t nota){
    return notas_cadena[nota];
}
