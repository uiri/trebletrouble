#define _GNU_SOURCE
#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "libfft.h"
#include "tone.h"

#define FFT_SIZE 15

/*
 * The audio format will be PCM.
 *  
 * -------------Data to be determined ----------------
 * numChannels -  1 for mono, 2 for stereo, etc...
 * sampleRate - samples per second (44100 or 96000)
 * bitsPerSample - 8 or 16 or 32. The higher, the better the resolution.
 * subChunk2Size - size of the actual sound data.
 * chunkSize - the size of the sound data + size of the header.
 *  
 */


/* ------------------------------------------------------ [ Section: Endians ] */
int isBigEndian() {
  int test = 1;
  char *p = (char*)&test;

  return p[0] == 0;

}

void reverseEndianness(const long long int size, void* value) {

  int i;
  char result[32];
  for (i = 0; i < size; i++) {
    result[i] = ((char*)value)[size-i-1];
  }
  for (i = 0; i < size; i++) {
    ((char*)value)[i] = result[i];
  }
}

void toBigEndian(const long long int size, void* value) {
  
  char needsFix = !( (1 && isBigEndian()) || (0 && !isBigEndian()) );
  if (needsFix) {
    reverseEndianness(size,value);
  }
}

void toLittleEndian(const long long int size, void* value) {
  char needsFix = !( (0 && isBigEndian()) || (1 && !isBigEndian()) );
  if (needsFix) {
    reverseEndianness(size,value);
  }
}

/* ------------------------------------------------------ [ Section: Wave Header ]  */

void makeWaveHeader(WaveHeader* myHeader,const int sampleRate, const short numChannels, const short bitsPerSample ) {

  /* RIFF WAVE HEADER */
  myHeader->chunkId[0] = 'R';
  myHeader->chunkId[1] = 'I';
  myHeader->chunkId[2] = 'F';
  myHeader->chunkId[3] = 'F';
  myHeader->format[0] = 'W';
  myHeader->format[1] = 'A';
  myHeader->format[2] = 'V';
  myHeader->format[3] = 'E';

  /* Format subchunk */
  myHeader->subChunk1Id[0] = 'f';
  myHeader->subChunk1Id[1] = 'm';
  myHeader->subChunk1Id[2] = 't';
  myHeader->subChunk1Id[3] = ' ';
  myHeader->audioFormat = 1; /* For PCM */
  myHeader->numChannels = numChannels; /* 1 for MONO, 2 for stereo */
  myHeader->sampleRate = sampleRate; /* ie 44100 hertz, cd quality audio */
  myHeader->bitsPerSample = bitsPerSample; 
  myHeader->byteRate = myHeader->sampleRate * myHeader->numChannels * myHeader->bitsPerSample / 8;
  myHeader->blockAlign = myHeader->numChannels * myHeader->bitsPerSample/8;

  /* Data subchunk */
  myHeader->subChunk2Id[0] = 'd';
  myHeader->subChunk2Id[1] = 'a';
  myHeader->subChunk2Id[2] = 't';
  myHeader->subChunk2Id[3] = 'a';

  myHeader->chunkSize = 4+8+16+8+0;
  myHeader->subChunk1Size = 16;
  myHeader->subChunk2Size = 0;

}

void makeWave(Wave* myWave, const int sampleRate, const short numChannels, const short bitsPerSample) {
  makeWaveHeader(&myWave->header, sampleRate, numChannels, bitsPerSample);
}

void waveDestroy( Wave* wave) {
  free(wave-> data);
}

void waveSetDuration(Wave* wave, const float seconds) {
  long long int totalBytes = (long long int)(wave->header.byteRate*seconds);
  wave->data = (char*)malloc(totalBytes);
  wave->index = 0;
  wave->size = totalBytes;
  wave->nSamples = (long long int) wave-> header.numChannels * wave->header.sampleRate * seconds;
  wave->header.chunkSize = 4+8+16+8+totalBytes;
  wave->header.subChunk2Size = totalBytes;
}

void waveAddSample( Wave* wave, const float* samples ) {
  int i;
  
  long int sampleNbit;
  char* sample;

  if ( wave->header.bitsPerSample == 32) {
    for(i = 0; i < wave->header.numChannels; i++) {
      sampleNbit = ( long int) ((1<<(wave->header.bitsPerSample-1))*samples[i]);
      toLittleEndian(4, (void*) &sampleNbit);
      sample = (char*)&sampleNbit;
      wave->data[ wave->index + 0 ] = sample[0];
      wave->data[ wave->index + 1 ] = sample[1];
      wave->data[ wave->index + 2 ] = sample[2];
      wave->data[ wave->index + 3 ] = sample[3];
      wave->index += 4;
    }
  }
}

void waveToFile( Wave* wave, const char* filename ) {

  /* First make sure all numbers are little endian */
  toLittleEndian(sizeof(int), (void*)&(wave->header.chunkSize));
  toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk1Size));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.audioFormat));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.numChannels));
  toLittleEndian(sizeof(int), (void*)&(wave->header.sampleRate));
  toLittleEndian(sizeof(int), (void*)&(wave->header.byteRate));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.blockAlign));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.bitsPerSample));
  toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk2Size));

  /* Open the file, write header, write data */
  FILE *file;
  file = fopen(filename, "wb");
  fwrite( &(wave->header), sizeof(WaveHeader), 1, file);
  fwrite( (void*)(wave->data), sizeof(char), wave->size, file);
  fclose( file );
  
  /* Convert back to the system endian-ness */
  toLittleEndian(sizeof(int), (void*)&(wave->header.chunkSize));
  toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk1Size));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.audioFormat));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.numChannels));
  toLittleEndian(sizeof(int), (void*)&(wave->header.sampleRate));
  toLittleEndian(sizeof(int), (void*)&(wave->header.byteRate));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.blockAlign));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.bitsPerSample));
  toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk2Size));
}

/* -------------------------------------------------------------------- [ Section: Main ]  */
/* Wave tone(void){ */
void tone(float freq, float* data, float duration){ 
  /* Define some variables for the sound */
  /*float duration = 0.3;*/       /* seconds */

  int nSamples = (int)(duration*SAMPLE_RATE);
  Wave mySound;

  /* Create a mono(1), 32-bit sound and set the duration */
  makeWave(&mySound,(int)SAMPLE_RATE,1,32);
  waveSetDuration(&mySound, duration);

  /* Add all of the data */
  int i;

  for (i = 0; i < nSamples; i++) {
    data[i] = cos(freq*(float)i*3.14159/SAMPLE_RATE);
  }

}

float get_pitch(float freq, float duration)
{
  float *wave, *imaginary_wave, max, pitch;
  int max_index, i;

  if (duration * SAMPLE_RATE < 1.0)
    return NAN;
  wave = malloc(sizeof(float) * SAMPLE_RATE * duration);
  imaginary_wave = calloc(SAMPLE_RATE * duration, sizeof(float));
  tone(freq, wave, duration); /* wave contains an array of tone Hz and sample rate 44.1 khz */
  initfft(FFT_SIZE);
  fft(wave,imaginary_wave,0);
  free(imaginary_wave);
  max = 0;
  for (i = 0; i < (SAMPLE_RATE / 2); i++) {
    /* check if there's any imaginary values */
    /* waveval = sqrt(imaginary_wave[i] * imaginary_wave[i] + wave[i] * wave[i]); */
    if (wave[i] > max) {
      max = wave[i];
      max_index = i;
    }
    /* } */
  }
  pitch = ((float)max_index) * SAMPLE_RATE / (1 << (FFT_SIZE-1));
  free(wave);

  return pitch;
}
