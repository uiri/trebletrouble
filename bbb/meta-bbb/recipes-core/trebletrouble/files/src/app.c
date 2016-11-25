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
#include "libfft.h"

int main(int argc, char** argv) {
	char* fbp;
	int fbfd, err, i;
	float *wave, duration = 5.0;
	float *imaginary_wave;
	int actual[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 62, 63, 65};
	int expected[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65};

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

        wave = malloc(sizeof(float) * SAMPLE_RATE * duration);
	imaginary_wave = calloc(SAMPLE_RATE * duration, sizeof(float));
	tone(wave, duration); /* wave contains an array of tone 880Hz and 44.1 khz */
	initfft(15);
	fft(wave,imaginary_wave,0);
	free(imaginary_wave);
	free(wave);
	return 0;
}
