#include "bibliotecas.h"
#include "tramo.h"
#include "endianness.h"
#include "escritura_wave.h"

bool escribir_encabezado_wave(size_t n_muestras, FILE *f){
	
	const char chunk_id[4]="RIFF", format[4]="WAVE";
	uint32_t chunk_size=36+2*n_muestras;

	if((fwrite(chunk_id,sizeof(char),4,f))!=4)
		return false;
	if(escribir_uint32_little_endian(&chunk_size,f)==false)
		return false;
	if((fwrite(format,sizeof(char),4,f))!=4)
		return false;
	return true;
}

bool escribir_fmt_subchunk_wave(uint32_t sample_rate, FILE *f){

	char subchunk1id[4]="fmt ";
	uint32_t subchunk1_size=16, byte_rate=2*sample_rate;
	uint16_t audio_format=1, num_channels=1, block_align=2, bits_per_sample=16;

	if((fwrite(subchunk1id,sizeof(char),4,f))!=4)
		return false;
	if(escribir_uint32_little_endian(&subchunk1_size,f)==false)
		return false;
	if(escribir_uint16_little_endian(&audio_format,f)==false)
		return false;
	if(escribir_uint16_little_endian(&num_channels,f)==false)
		return false;
	if(escribir_uint32_little_endian(&sample_rate,f)==false)
		return false;	
	if(escribir_uint32_little_endian(&byte_rate,f)==false)
		return false;
	if(escribir_uint16_little_endian(&block_align,f)==false)
		return false;
	if(escribir_uint16_little_endian(&bits_per_sample,f)==false)
		return false;
	return true;
}

bool escribir_data_subchunk_wave(int16_t *muestras, size_t n_muestras,FILE *f){
	
	const char subchunk2id[4]="data";
	uint32_t subchunk2_size=2*n_muestras;

	if((fwrite(subchunk2id,sizeof(char),4,f))!=4)
		return false;	
	if(escribir_uint32_little_endian(&subchunk2_size,f)==false)
		return false;	
	for(size_t i=0;i<n_muestras;i++){
		if(escribir_int16_little_endian(muestras+i,f)==false)
			return false;
	}
	return true;
}



bool escribir_archivo_wave(tramo_t *tramo_final, int f_m, FILE *f){

	int16_t *muestras_norm;
	size_t n;

	if((muestras_norm=tramo_normalizar_muestras(tramo_final,&n))==NULL)
		return false;

	if(escribir_encabezado_wave(n,f)==false)
		return false;
	if(escribir_fmt_subchunk_wave(f_m,f)==false)
		return false;
	if(escribir_data_subchunk_wave(muestras_norm, n, f)==false){
		destruir_muestras_norm(muestras_norm);
		return false;
	}
	destruir_muestras_norm(muestras_norm);

	return true;
}


