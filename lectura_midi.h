#ifndef LECTURA_MIDI_H
#define LECTURA_MIDI_H

#include "deco_cod.h"
#include "tda_nota.h"


#define HEADER_ID_VALUE 0x4D546864
#define HEADER_SIZE 6
#define TRACK_ID_VALUE 0x4D54726B
#define MASK_TIEMPO 0x7F
#define MASK_TIEMPO_MSB 0x80
#define MAX_BYTES_TIEMPO 4

#define METAEVENTO_FIN_DE_PISTA 0x2F
#define EVENTO_MAX_LONG 10
#define MAX_NOTAS 5000


typedef struct contenedor contenedor_notas_t;


enum {EVNOTA_NOTA, EVNOTA_VELOCIDAD};
enum {METAEVENTO_TIPO, METAEVENTO_LONGITUD};

bool leer_encabezado(FILE *f, formato_t *formato, uint16_t *numero_pistas, uint16_t *pulsos_negra);
bool leer_pista(FILE *f, uint32_t *tamagno);
bool leer_tiempo(FILE *f, uint32_t *tiempo);
bool leer_evento(FILE *f, evento_t *evento, char *canal, int *longitud, uint8_t mensaje[]);
void descartar_metaevento(FILE *f, uint8_t tamagno);

contenedor_notas_t *leer_archivo_midi(char canal_in,FILE *f);
contenedor_notas_t *cont_notas_crear(void);
void cont_notas_destruir(contenedor_notas_t *cont_notas);
size_t cont_notas_extraer_cantidad_notas(contenedor_notas_t *cont_notas);
nota_leida_t *cont_notas_extraer_nota(contenedor_notas_t *cont_notas, size_t i);

#endif