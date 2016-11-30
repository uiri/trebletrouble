#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "display.h"
#include "colours.h"
#include "display.h"
#include "input.h"
#include "metronome.h"
#include "tone.h"
#include "libfft.h"
#include "audio_recorder.h"

int main(int argc, char** argv) {
	char* fbp;
	struct {
		int fb, ts;
	} fd;
	int err, i;
		
	int actual[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 62, 63, 65};
	ScreenBounds sb;

	fbp = init_display(&fd.fb);

	err = init_touchscreen(&fd.ts, &sb);
	if (err) {
		printf("Error reading event input file\n");
		return err;
	}

	int expected[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65};
	int freq;

	int numFrequencies = 11;
	int freqList[numFrequencies]; 
	float durationList[numFrequencies];
	float pitch[numFrequencies];
	float duration;

	colour_screen(fbp, ORANGE);
	err = bitblit("/srv/trebletrouble/timbit.pnm", fbp, 400, 240);
	if (err) {
		if (err == -1) {
			printf("File not found :(\n");
		} else if (err == -2) {
			printf("Not P6 file\n");
		}
		return err;
	}


	get_lcd_input(&fd.ts, &sb);
	colour_screen(fbp, WHITE);

	/*draw staff on screen*/
	draw_staff(fbp);

	/*variables for reading song*/
	FILE *song = fopen(SONG, "rb");

	load_song(song, fbp, expected);
	fclose(song);
	get_lcd_input(&fd.ts, &sb);

	/* This run has an intentional mistake in the song*/
	for (i = 0; i < NUM_NOTES; i++) {
		compare_notes(expected[i], actual[i], i, fbp);
		sleep(1);
	}

	get_lcd_input(&fd.ts, &sb);
	/* Reset notes to black */
	clear_notes(0, expected, actual, fbp);
	get_lcd_input(&fd.ts, &sb);
	/* This run is perfect ;) */
	for (i = 0; i < NUM_NOTES; i++) {
		compare_notes(expected[i], expected[i], i, fbp);
		sleep(1);
	}

	get_lcd_input(&fd.ts, &sb);

       	cleanup_display(fbp, &fd.fb);
	metronome(100);
	/*cleanup_display(fbp, &fbfd);*/
	while(1); /* fuck it */
	cleanup_display(fbp, &fd.fb);

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


