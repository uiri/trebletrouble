#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tone.h"

#define NUM_FREQS 11

int main(void)
{
        
	int freqList[NUM_FREQS];
	int freq, i;
	float durationList[NUM_FREQS];
	float duration;
        
        /*freq = 880;
        duration = 1.0;
	pitch = get_pitch(freq,duration);
	*/
  
	/*JINGLE BELLS*/
	freqList[0]  = 330;
	freqList[1]  = 330;
	freqList[2]  = 330;
	freqList[3]  = 330;
	freqList[4]  = 330;
	freqList[5]  = 330;
	freqList[6]  = 330;
	freqList[7]  = 391;
	freqList[8]  = 261;
 	freqList[9]  = 293;
	freqList[10] = 330;

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

	/* generate wave data and fft*/ 
	for (i = 0; i < numFrequencies; i++) {
	  freq = freqList[i];
	  duration = durationList[i];
	  pitch[i] = get_pitch(freq,duration);
	  printf("%f\n",pitch[i]);
	  sleep(duration);
	}
	return 0;
}
