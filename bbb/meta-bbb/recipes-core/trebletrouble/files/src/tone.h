#ifndef TONE_H_
#define TONE_H_

typedef struct WaveHeader {
  
  // Riff Wave Header
  char chunkId[4];
  int chunkSize;
  char format[4];

  // Format Subchunk
  char subChunk1Id[4];
  int subChunk1Size;

  short int audioFormat;
  short int numChannels;
  int sampleRate;
  int byteRate;
  short int blockAlign;
  short int bitsPerSample;
  // short in extraParamSize;

  // Data subchunk
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

Wave tone(void);

#endif
