#include "colours.h"
#include "display.h"
#include "input.h"
#include "menu.h"
#include "audio_recorder.h"

int main(int argc, char** argv) {
	char* fbp;
	int err;
	int fbfd;
	ScreenInput si;
	void (*menu_item)(char* fbp, ScreenInput* si);
	WaveHeader *hdr;
	Wave* wave;
	int duration;

	fbp = init_display(&fbfd);

	err = init_touchscreen(&si.fd);
	if (err) {
		printf("Error reading event input file\n");
		return err;
	}

	colour_screen(fbp, ORANGE);
	get_lcd_input(&si);

	if (si.y > (TS_MAX_Y / 2)) {
		menu_item = metronome_menu;
	} else {
		menu_item = play_song_menu;
	}
	menu_item(fbp, &si);

	get_lcd_input(&si);
	cleanup_display(fbp, &fbfd);

	/* Section: Audio Recording */
	
	duration = 10;
        
	hdr = malloc(sizeof(*hdr));
	wave = malloc(SAMPLE_RATE * duration);
	
	audio_recorder(wave,hdr,duration); /* record for 10 seconds */

	return 0;
}


