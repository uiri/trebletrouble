#define _GNU_SOURCE
#include <stdio.h>
#include <malloc.h>
#include <math.h>

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


// ------------------------------------------------------ [ Section: Endians ]
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

// ------------------------------------------------------ [ Section: Wave Header ] 


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
  // short int extraParamSize;

  // Data subchunk
  char subChunk2Id[4];
  int subChunk2Size;

} WaveHeader;

WaveHeader makeWaveHeader(int const sampleRate, short int const numChannels, short int const bitsPerSample ) {

  WaveHeader myHeader;

  // RIFF WAVE HEADER
  myHeader.chunkId[0] = 'R';
  myHeader.chunkId[1] = 'I';
  myHeader.chunkId[2] = 'F';
  myHeader.chunkId[3] = 'F';
  myHeader.format[0] = 'W';
  myHeader.format[1] = 'A';
  myHeader.format[2] = 'V';
  myHeader.format[3] = 'E';

  // Format subchunk
  myHeader.subChunk1Id[0] = 'f';
  myHeader.subChunk1Id[1] = 'm';
  myHeader.subChunk1Id[2] = 't';
  myHeader.subChunk1Id[3] = ' ';
  myHeader.audioFormat = 1; // For PCM
  myHeader.numChannels = numChannels; // 1 for MONO, 2 for stereo
  myHeader.sampleRate = sampleRate; // ie 44100 hertz, cd quality audio
  myHeader.bitsPerSample = bitsPerSample; 
  myHeader.byteRate = myHeader.sampleRate * myHeader.numChannels * myHeader.bitsPerSample / 8;
  myHeader.blockAlign = myHeader.numChannels * myHeader.bitsPerSample/8;

  // Data subchunk
  myHeader.subChunk2Id[0] = 'd';
  myHeader.subChunk2Id[1] = 'a';
  myHeader.subChunk2Id[2] = 't';
  myHeader.subChunk2Id[3] = 'a';

  myHeader.chunkSize = 4+8+16+8+0;
  myHeader.subChunk1Size = 16;
  myHeader.subChunk2Size = 0;

  return myHeader;

}

// ----------------------------------------------------- [ Section: Wave ] 
typedef struct Wave {
  WaveHeader header;
  char* data;
  long long int index;
  long long int size;
  long long int nSamples;
  
} Wave;


Wave makeWave(int const sampleRate, short int const numChannels, short int const bitsPerSample) {
  Wave myWave;
  myWave.header = makeWaveHeader(sampleRate, numChannels, bitsPerSample);
  return myWave;
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
  short int sample8bit;
  int sample16bit;
  long int sample32bit;
  char* sample;
  if ( wave->header.bitsPerSample == 8) {
    for(i = 0; i < wave->header.numChannels; i++) {
      sample8bit = (short int)(127+127.0*samples[i]);
      toLittleEndian(1, (void*) &sample8bit);
      sample = (char*)&sample8bit;
      (wave->data)[ wave->index ] = sample[0];
      wave->index += 1;
    }
  }
  if ( wave->header.bitsPerSample == 16) {
    for(i = 0; i < wave->header.numChannels; i++) {
      sample16bit = (int) (32767*samples[i]);
      toLittleEndian(2, (void*) &sample16bit);
      sample = (char*)&sample16bit;
      wave->data[ wave->index + 0 ] = sample[0];
      wave->data[ wave->index + 1 ] = sample[1];
      wave->index += 2;
    }
  }
  if ( wave->header.bitsPerSample == 32) {
    for(i = 0; i < wave->header.numChannels; i++) {
      sample32bit = ( long int) ((pow(2, 32-1)-1)*samples[i]);
      toLittleEndian(4, (void*) &sample32bit);
      sample = (char*)&sample32bit;
      wave->data[ wave->index + 0 ] = sample[0];
      wave->data[ wave->index + 1 ] = sample[1];
      wave->data[ wave->index + 2 ] = sample[2];
      wave->data[ wave->index + 3 ] = sample[3];
      wave->index += 4;
    }
  }
}

void waveToFile( Wave* wave, const char* filename ) {

  // First make sure all numbers are little endian
  toLittleEndian(sizeof(int), (void*)&(wave->header.chunkSize));
  toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk1Size));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.audioFormat));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.numChannels));
  toLittleEndian(sizeof(int), (void*)&(wave->header.sampleRate));
  toLittleEndian(sizeof(int), (void*)&(wave->header.byteRate));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.blockAlign));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.bitsPerSample));
  toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk2Size));

  // Open the file, write header, write data
  FILE *file;
  file = fopen(filename, "wb");
  fwrite( &(wave->header), sizeof(WaveHeader), 1, file);
  fwrite( (void*)(wave->data), sizeof(char), wave->size, file);
  fclose( file);
  
  // Convert back to the system endian-ness
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

// -------------------------------------------------------------------- [ Section: Main ] 
int tone(void){
  
  // Define some variables fro the sound
  float sampleRate = 44100.0; // hertz
  float freq = 880.0;         // hertz
  //float duration = 0.3;       // seconds
  float duration = 5.0; 

  int nSamples = (int)(duration*sampleRate);

  // Create a mono(1), 32-bit sound and set the duration
  Wave mySound = makeWave((int)sampleRate,1,32);
  waveSetDuration(&mySound, duration);
  
  // Add all of the data
  int i;
  float frameData[1];
  for(i = 0; i<nSamples; i++){
    frameData[0] = cos(freq*(float)i*3.14159/sampleRate);
    waveAddSample(&mySound,frameData);
  }

  // Write it to a file and clear up when done
  //waveToFile( &mySound, "mono-32bit.wav");
  
  // play the wav sound
  

  waveDestroy( &mySound );

  return 0;

}
