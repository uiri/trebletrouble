#include <stdint.h>

#ifndef TONE_H_
#define TONE_H_

#define SAMPLE_RATE 44100.0

typedef struct WaveHeader{

  /* Riff Wave Header */
  char chunkId[4];
  int chunkSize;
  char format[4];

  short int audioFormat;
  short int numChannels;
  int sampleRate;
  int byteRate;
  short int blockAlign;
  short int bitsPerSample;
  /* short in extraParamSize; */

  /* Data subchunk */
  char subChunk1Id[4];
  int subChunk1Size;

  char subChunk2Id[4];
  int subChunk2Size;

} WaveHeader;

typedef struct Wave { 
   WaveHeader header; 
   char* data; 
   long long int index; 
   long long int size; 
   long long int nSamples; 

} Wave;

float get_pitch(Wave* wave, uint32_t duration);
void waveAddSample(Wave* wave, const float* samples, int channel);
void tone(Wave* wave, int duration);
void waveDestroy(Wave* wave);

#endif
