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
#include "libfft.h"

int main(int argc, char** argv) {
	char* fbp;
	struct {
		int fb, ts;
	} fd;
	int err, i;
	float *wave, duration = 5.0;
	float *imaginary_wave;
	int actual[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 62, 63, 65};
	ScreenBounds sb;

	fbp = init_display(&fd.fb);

	err = init_touchscreen(&fd.ts, &sb);
	if (err) {
		printf("Error reading event input file\n");
		return err;
	}

	int expected[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65};

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
	/*tone();
	return 0; 
	*/
	/*cleanup_display(fbp, &fbfd);*/
	while(1); /* fuck it */
	cleanup_display(fbp, &fd.fb);

	wave = malloc(sizeof(float) * SAMPLE_RATE * duration);
	imaginary_wave = calloc(SAMPLE_RATE * duration, sizeof(float));
	tone(wave, duration); /* wave contains an array of tone 880Hz and 44.1 khz */
	initfft(15);
	fft(wave,imaginary_wave,0);
	free(imaginary_wave);
	free(wave);
	return 0;

}
