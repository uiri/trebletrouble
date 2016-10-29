#include "colours.h"
#include "display.h"
#include "input.h"
<<<<<<< HEAD
#include "menu.h"
=======
#include "metronome.h"
#include "tone.h"
#include "audio_recorder.h"
>>>>>>> Added tone.c which will generate a wav file with 44.1 Khz sampling rate and a 880 Hz tone.

int main(int argc, char** argv) {
	char* fbp;
	int err;
	int fbfd;
	ScreenInput si;
	void (*menu_item)(char* fbp, ScreenInput* si);

	fbp = init_display(&fbfd);

	err = init_touchscreen(&si.fd);
	if (err) {
		printf("Error reading event input file\n");
		return err;
	}

	colour_screen(fbp, ORANGE);
	get_lcd_input(&si);

<<<<<<< HEAD
	if (si.y > (TS_MAX_Y / 2)) {
		menu_item = metronome_menu;
	} else {
		menu_item = play_song_menu;
=======

	get_lcd_input(&fd.ts, &sb);
	/* Reset notes to black */
	clear_notes(0, expected, actual, fbp);
	get_lcd_input(&fd.ts, &sb);
	/* This run is perfect ;) */
	for (i = 0; i < NUM_NOTES; i++) {
		compare_notes(expected[i], expected[i], i, fbp);
		sleep(1);
>>>>>>> Added tone.c which will generate a wav file with 44.1 Khz sampling rate and a 880 Hz tone.
	}
	menu_item(fbp, &si);

<<<<<<< HEAD
	get_lcd_input(&si);
	cleanup_display(fbp, &fbfd);
	return 0;
=======
	get_lcd_input(&fd.ts, &sb);
       	cleanup_display(fbp, &fd.fb);
	metronome(100);
	return 0; 
	
	cleanup_display(fbp, &fbfd);
	while(1); /* fuck it */
	cleanup_display(fbp, &fd.fb);
	metronome();

	return 0;

>>>>>>> Added tone.c which will generate a wav file with 44.1 Khz sampling rate and a 880 Hz tone.
}
