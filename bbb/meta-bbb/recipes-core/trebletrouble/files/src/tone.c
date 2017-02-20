#define _GNU_SOURCE
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#include "libfft.h"
#include "tone.h"

#define FFT_SIZE 15

#define BITS_PER_SAMPLE 32

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

void toLittleEndian(const long long int size, void* value) {
  char needsFix = !( (0 && isBigEndian()) || (1 && !isBigEndian()) );
  if (needsFix) {
    reverseEndianness(size,value);
  }
}

float get_pitch(Wave* wave, uint32_t duration) {
  float *imaginary_wave, max, pitch;
  int max_index, i;

  if (duration * SAMPLE_RATE < 1.0)
    return NAN;

  imaginary_wave = calloc(SAMPLE_RATE * duration, sizeof(float));

  initfft(FFT_SIZE);
  fft((float*)wave->data,imaginary_wave,0);
  free(imaginary_wave);

  max = 0;
  for (i = 0; i < (SAMPLE_RATE / 2); i++) {
    /* check if there's any imaginary values */
    /* waveval = sqrt(imaginary_wave[i] * imaginary_wave[i] + wave[i] * wave[i]); */
    if (((float*)wave->data)[i] > max) {
      max = ((float*)wave->data)[i];
      max_index = i;
    }
    /* } */
  }
  pitch = ((float)max_index) * SAMPLE_RATE / (1 << (FFT_SIZE-1));

  return pitch;
}

Wave* makeWave(int duration) {
	/* Define some variables for the sound */
	Wave* wave;

	/* Create a mono(1), 32-bit sound and set the duration */
	wave = malloc(sizeof(Wave));

	/* RIFF WAVE HEADER */
	wave->header.chunkId[0] = 'R';
	wave->header.chunkId[1] = 'I';
	wave->header.chunkId[2] = 'F';
	wave->header.chunkId[3] = 'F';
	wave->header.format[0]  = 'W';
	wave->header.format[1]  = 'A';
	wave->header.format[2]  = 'V';
	wave->header.format[3]  = 'E';

	/* Format subchunk */
	wave->header.subChunk1Id[0] = 'f';
	wave->header.subChunk1Id[1] = 'm';
	wave->header.subChunk1Id[2] = 't';
	wave->header.subChunk1Id[3] = ' ';
	wave->header.audioFormat = 1;
	wave->header.numChannels = NUM_CHANS;
	wave->header.sampleRate = SAMPLE_RATE;
	wave->header.bitsPerSample = BITS_PER_SAMPLE; 
	wave->header.byteRate = SAMPLE_RATE * NUM_CHANS * (BITS_PER_SAMPLE / 8);
	wave->header.blockAlign = NUM_CHANS * (BITS_PER_SAMPLE / 8);

	wave->header.subChunk2Id[0] = 'd';
	wave->header.subChunk2Id[1] = 'a';
	wave->header.subChunk2Id[2] = 't';
	wave->header.subChunk2Id[3] = 'a';

	wave->size = (long long int)( wave->header.byteRate * duration );
	wave->header.chunkSize = 4 + 8 + 16 + 8 + wave->size;
	wave->header.subChunk1Size = 16;
	wave->header.subChunk2Size = wave->size;

	wave->data = malloc(wave->size);
	wave->index = 0;
	wave->nSamples = (long long int) wave-> header.numChannels * wave->header.sampleRate * duration;

	return wave;
}

void waveAddSample( Wave* wave, const float* samples, int channel ) {
  int i;
  long int sample32bit;
  char* sample;

  if (channel != wave-> header.numChannels) {
    return;
  }

  if ( wave->header.bitsPerSample == 32) {
    for(i = 0; i < wave->header.numChannels; i++) {
      sample32bit = ( long int) ((1<<(wave->header.bitsPerSample-1))*samples[i]);
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

void waveDestroy( Wave* wave) {
	if (!wave)
		return;
	if (wave->data)
		free(wave-> data);
	free(wave);
}
