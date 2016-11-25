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

void tone(float* data, float duration);

#endif
