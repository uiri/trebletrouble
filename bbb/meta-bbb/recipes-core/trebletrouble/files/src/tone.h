#include <stdint.h>

#ifndef TONE_H_
#define TONE_H_

/* 44100 hertz, cd quality audio */
#define SAMPLE_RATE 44100.0

/* 1 for MONO, 2 for stereo */
#define NUM_CHANS 1

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

float get_pitch(Wave* wave);
void waveAddSample(Wave* wave, float sample);
Wave* makeWave(float duration);
void waveDestroy(Wave* wave);

#endif
