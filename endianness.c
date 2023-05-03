#include "bibliotecas.h"
#include "endianness.h"


 
uint8_t leer_uint8_t(FILE *f){

 	uint8_t valor;
 	
 	fread(&valor,sizeof(uint8_t),1,f);
 	return valor;	
}

uint16_t leer_uint16_big_endian(FILE *f){

	uint16_t valor;
	uint8_t bytes[2];

	fread(bytes,sizeof(uint8_t),2,f);
	valor=(bytes[0]<<8)|(bytes[1]);

	return valor;
}

uint32_t leer_uint32_big_endian(FILE *f){

	uint32_t valor;
	uint8_t bytes[4];

	fread(bytes,sizeof(uint8_t),4,f);
	valor=(bytes[0]<<24)|(bytes[1]<<16)|(bytes[2]<<8)|(bytes[3]);
	return valor;
}

bool escribir_uint8_t(uint8_t *valor,FILE *f){
 	
 	if(fwrite(&valor,sizeof(uint8_t),1,f)!=1)
 		return false;
 	return true;	
}

bool escribir_uint16_little_endian(uint16_t *valor,FILE *f){

	uint8_t bytes[2];

	bytes[0]=(*valor)&MASK_BYTE;
	bytes[1]=((*valor)>>8)&MASK_BYTE;

	if(fwrite(bytes,sizeof(uint16_t),1,f)!=1)
		return false;
	return true;
}

bool escribir_int16_little_endian(int16_t *valor,FILE *f){

	uint8_t bytes[2];

	bytes[0]=(*valor)&MASK_BYTE;
	bytes[1]=((*valor)>>8)&MASK_BYTE;
	
	if(fwrite(bytes,sizeof(int16_t),1,f)!=1)
		return false;
	return true;
}

bool escribir_uint32_little_endian(uint32_t *valor,FILE *f){

	uint8_t bytes[4];

	bytes[0]=(*valor)&MASK_BYTE;
	bytes[1]=((*valor)>>8)&MASK_BYTE;
	bytes[2]=((*valor)>>16)&MASK_BYTE;
	bytes[3]=((*valor)>>24)&MASK_BYTE;

	
	if(fwrite(bytes,sizeof(uint32_t),1,f)!=1)
		return false;
	
	return true;
}