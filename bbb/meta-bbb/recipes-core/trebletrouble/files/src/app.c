#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "display.h"
#include "metronome.h"
#include "tone.h"
#include "audio_recorder.h"

int main(int argc, char** argv) {

  /* char* fbp; */
  /* int fbfd, err; */
  int freq;
  float pitch;
  float duration;
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
  freq = 880;
  duration = 1.0;
  pitch = get_pitch(freq,duration);

  return 0;
}


