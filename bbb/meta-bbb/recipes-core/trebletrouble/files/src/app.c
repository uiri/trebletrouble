#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "display.h"
#include "metronome.h"
#include "tone.h"
#include "audio_recorder.h"
#include "libfft.h"

int main(int argc, char** argv) {

  /* char* fbp; */
  /* int fbfd, err; */
  float *wave, duration = 5.0;
  float *imaginary_wave;
/*
	fbp = init_display(&fbfd);

	err = bitblit("/srv/trebletrouble/timbit.pnm", fbp, 400, 240);

	if (err) {
		if (err == -1) {
			printf("File not found :(");
		} else if (err == -2) {
			printf("Not P6 file");
		}
		return err;
	}

	sleep(5);
	display_frequency(440.0, fbp);
	sleep(10);
	display_frequency(855.304, fbp);
	sleep(10);
	display_frequency(4100.0, fbp);
	sleep(10);
	display_frequency(28.0, fbp);
	sleep(10);

	cleanup_display(fbp, &fbfd);

*/
        wave = malloc(sizeof(float) * SAMPLE_RATE * duration);
	imaginary_wave = calloc(SAMPLE_RATE * duration, sizeof(float));
	tone(wave, duration); /* wave contains an array of tone 880Hz and 44.1 khz */
	initfft(15);
	fft(wave,imaginary_wave,0);
	free(imaginary_wave);
	free(wave);
	return 0;
}
