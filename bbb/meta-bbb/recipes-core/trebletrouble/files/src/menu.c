#include <stdio.h>
#include <unistd.h>

#include "colours.h"
#include "display.h"
#include "input.h"
#include "metronome.h"

void play_song_menu(char* fbp, ScreenInput *si) {
	FILE *song;
	int actual[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 62, 63, 65};
	int expected[NUM_NOTES];
	int i;

	colour_screen(fbp, WHITE);

	/*draw staff on screen*/
	draw_staff(fbp);

	/*variables for reading song*/
	song = fopen(SONG, "rb");

	load_song(song, fbp, expected);
	fclose(song);
	get_lcd_input(si);

	/* This run has an intentional mistake in the song*/
	for (i = 0; i < NUM_NOTES; i++) {
		compare_notes(expected[i], actual[i], i, fbp);
		sleep(1);
	}

	get_lcd_input(si);
	/* Reset notes to black */
	clear_notes(0, expected, actual, fbp);
	get_lcd_input(si);
	/* This run is perfect ;) */
	for (i = 0; i < NUM_NOTES; i++) {
		compare_notes(expected[i], expected[i], i, fbp);
		sleep(1);
	}
}

void metronome_menu(char* fbp, ScreenInput *si) {
	colour_screen(fbp, GREEN);
	metronome(100);
}
