#ifndef DECO_COD_H
#define DECO_COD_H

#define MASK_EVENTO 0xF0
#define SHIFT_EVENTO 4
#define MASK_CANAL 0x0F
#define MASK_VALIDACION 0x80
#define CANT_NOTAS 12
#define MAX_VALOR_FORMATO 2

typedef enum{
			FMT_PISTA_UNICA,
			FMT_MULTIPISTAS_SINC,
			FMT_MULTIPISTAS_ASINC
} formato_t; 

typedef enum{
			EVT_NOTA_APAGADA=8,
			EVT_NOTA_ENCENDIDA,
			EVT_NOTA_DURANTE,
			EVT_CAMBIO_CONTROL,
			EVT_CAMBIO_PROGRAMA,
			EVT_VARIAR_CANAL,
			EVT_CAMBIO_PITCH,
			EVT_METAEVENTO
} evento_t;

typedef enum{
			NOTA_DO,
			NOTA_DO_SOSTENIDO,
			NOTA_RE,
			NOTA_RE_SOSTENIDO,
			NOTA_MI,
			NOTA_FA,
			NOTA_FA_SOSTENIDO,
			NOTA_SOL,
			NOTA_SOL_SOSTENIDO,
			NOTA_LA,
			NOTA_LA_SOSTENIDO,
			NOTA_SI
} nota_t;

bool decodificar_formato(uint16_t valor, formato_t *formato);
bool decodificar_evento(uint8_t valor, evento_t *evento, char *canal, int *longitud);
bool decodificar_nota(uint8_t valor, nota_t *nota, signed char *octava);


const char *codificar_formato(formato_t formato);
const char *codificar_evento(evento_t evento);
const char *codificar_nota(nota_t nota);

#endif