#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "display.h"
#include "colours.h"

#define NUM_NOTES 16

#include <stdio.h>
#include "metronome.h"
#include "tone.h"
#include "libfft.h"
#include "audio_recorder.h"

int main(int argc, char** argv) {
	char* fbp;
	int fbfd, err, i;
	int actual[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 62, 63, 65};
	int expected[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65};
	int freq;
	int numFrequencies = 11;
	int freqList[numFrequencies]; 
	float durationList[numFrequencies];
	float pitch[numFrequencies];
	float duration;

	fbp = init_display(&fbfd);
	colour_screen(fbp, ORANGE);
	err = bitblit("/srv/trebletrouble/timbit.pnm", fbp, 400, 240);

	if (err) {
		if (err == -1) {
			printf("File not found :(");
		} else if (err == -2) {
			printf("Not P6 file");
		}
		return err;
	}
	sleep(2);
	colour_screen(fbp, WHITE);

	/*draw staff on screen*/
	draw_staff(fbp);

	/*variables for reading song*/
	FILE *song = fopen(SONG, "rb");
	load_song(song, fbp);
	fclose(song);
	sleep(2);
	for (i = 0; i < NUM_NOTES; i++) {
		compare_notes(expected[i], actual[i], i, fbp);
		sleep(1);
	}

	cleanup_display(fbp, &fbfd);

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

	/* generate wave data */ 
	for (i = 0; i < numFrequencies; i++) {
	  freq = freqList[i];
	  duration = durationList[i];
	  pitch[i] = get_pitch(freq,duration);
	  printf("%f\n",pitch[i]);
	  sleep(duration);
	}
  
	return 0;
}


