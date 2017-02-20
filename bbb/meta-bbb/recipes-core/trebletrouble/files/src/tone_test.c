#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tone.h"

#define NUM_FREQS 11

int main(void)
{
        
	float freqList[NUM_FREQS];
	float freq;
	int i;
	long long int j;
	float durationList[NUM_FREQS];
	float duration;
	Wave* wave;
  
	/*JINGLE BELLS*/
	freqList[0]  = 330.0;
	freqList[1]  = 330.0;
	freqList[2]  = 330.0;
	freqList[3]  = 330.0;
	freqList[4]  = 330.0;
	freqList[5]  = 330.0;
	freqList[6]  = 330.0;
	freqList[7]  = 391.0;
	freqList[8]  = 261.0;
	freqList[9]  = 293.0;
	freqList[10] = 330.0;

	durationList[0]  = 1.0;  /* E  */
	durationList[1]  = 1.0;  /* E  */
	durationList[2]  = 2.0;  /* E  */
  
	durationList[3]  = 1.0;  /* E  */
	durationList[4]  = 1.0;  /* E  */
	durationList[5]  = 2.0;  /* E  */
 
	durationList[6]  = 1.0;  /* E  */
	durationList[7]  = 1.0;  /* G  */
	durationList[8]  = 1.0;  /* C  */
	durationList[9]  = 1.0;  /* D  */
	durationList[10] = 2.0;  /* E  */

	for (i = 0; i < NUM_FREQS; i++) {
	  freq = freqList[i];
	  duration = durationList[i];
	  wave = makeWave(duration);

	  for (j = 0; j < wave->nSamples; j++) {
		  waveAddSample(wave, sin(freq * j * (duration * 3.14159 / wave->nSamples)));
	  }

	  printf("%f\n", get_pitch(wave));
	  waveDestroy(wave);
	}

	return 0;
}
