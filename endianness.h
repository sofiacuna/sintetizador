#ifndef ENDIANNESS_H
#define ENDIANNESS_H

#define MASK_BYTE 0xFF



uint8_t leer_uint8_t(FILE *f);
uint16_t leer_uint16_big_endian(FILE *f);
uint32_t leer_uint32_big_endian(FILE *f);

bool escribir_uint8_t(uint8_t *valor,FILE *f);
bool escribir_uint16_little_endian(uint16_t *valor,FILE *f);
bool escribir_int16_little_endian(int16_t *valor,FILE *f);
bool escribir_uint32_little_endian(uint32_t *valor,FILE *f);

#endif