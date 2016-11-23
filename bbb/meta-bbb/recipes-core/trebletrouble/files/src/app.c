#include <stdio.h>
#include <unistd.h>
#include "display.h"
#include "metronome.h"
#include "tone.h"
#include "audio_recorder.h"

int main(int argc, char** argv) {

  //char* fbp;
  //int fbfd, err;
  Wave wave;
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
	wave = tone(); // temp contains a tone 880Hz and 44.1 khz
	
	/*
	  INSERT CODE TO TAKE THE WAVE FILE AND OUTPUT A FREQUENCY
	  initfft(44100*5);
	  fft(temp,0,0);
	 */


	return 0;
}
