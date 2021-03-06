/*
 *  Copyright (C) 2017       William Pearson
 *  Copyright (C) 2016-2017  Tejan Chinnakonda
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permission under GNU GPL version 3 section 7
 *
 *  If you modify this Program, or any covered work, by linking or
 *  combining it with the uGFX Library, containing parts covered by the
 *  terms of the GFX License, Version 1.2, the licensors of this Program
 *  grant you additional permission to convey the resulting work.
 *  Corresponding Source for a non-source form of such a combination
 *  shall include the source code for the parts of the uGFX Library used
 *  as well as that of the covered work.
 *
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#include "libfft.h"
#include "filter.h"
#include "tone.h"

#define FFT_SIZE 15

#define BITS_PER_SAMPLE 16

/* This macro calculates the maximum value for a N-bit signed integer */
#define MAX_INT(N) ((1<<(N-1))-1)

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

float get_pitch(Wave* wave)
{
	float *imaginary_wave, max, pitch, *waveData;
	int max_index, i, fft_size, fft_samples;

	fft_size = FFT_SIZE;
	while ((1 << fft_size) > wave->nSamples)
		fft_size--;
	fft_samples = (1 << fft_size);

	imaginary_wave = calloc(fft_samples, sizeof(float));
	waveData = malloc(fft_samples * sizeof(float));
	filter_wavdata(waveData, fft_samples, ((short*)wave->data),
		       wave->nSamples, MAX_INT(wave->header.bitsPerSample));

	initfft(fft_size);
	fft(waveData,imaginary_wave,0);
	free(imaginary_wave);

	max = 0;
	for (i = 0; i < FILTER_FREQ; i++) {
		if (waveData[i] > max && i != 2*max_index) {
			max = waveData[i];
			max_index = i;
		}
	}

	free(waveData);
	pitch = ((float)max_index) * SAMPLE_RATE / fft_samples;

	return pitch;
}

Wave* makeWave(float duration)
{
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

	wave->header.chunkSize = 4 + 8 + 16 + 8 + wave->size;
	wave->header.subChunk1Size = 16;
	wave->header.subChunk2Size = wave->size;

	wave->size = (long long int)( (wave->header.byteRate * duration) / 1000);
	wave->nSamples = wave->size / (wave->header.bitsPerSample / 8);
	wave->data = malloc(wave->size);
	wave->index = 0;

	return wave;
}

void waveAddSample(Wave* wave, float sample) {
	long long int sample32bit;
	char *sample_bytes;

	if (wave->index >= wave->size)
		return;

	sample32bit = (long long int) (MAX_INT(wave->header.bitsPerSample)*sample);
	toLittleEndian(4, (void*) &sample32bit);
	sample_bytes = (char*)&sample32bit;
	wave->data[ wave->index ++ ] = sample_bytes[0];
	wave->data[ wave->index ++ ] = sample_bytes[1];
}

void waveDestroy( Wave* wave) {
	if (!wave)
		return;
	if (wave->data)
		free(wave-> data);
	free(wave);
}
