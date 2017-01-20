#include <stdio.h>
#include <unistd.h>

#include "colours.h"
#include "display.h"
#include "input.h"
#include "metronome.h"

int main(int argc, char** argv) {

	char* fbp;
	int err, i;
	int actual[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 62, 63, 65};
	int expected[NUM_NOTES];
	int fbfd;
	ScreenInput si;

	fbp = init_display(&fbfd);

	err = init_touchscreen(&si.fd);
	if (err) {
		printf("Error reading event input file\n");
		return err;
	}

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


	get_lcd_input(&si);
	colour_screen(fbp, WHITE);

	/*draw staff on screen*/
	draw_staff(fbp);

	/*variables for reading song*/
	FILE *song = fopen(SONG, "rb");

	load_song(song, fbp, expected);
	fclose(song);
	get_lcd_input(&si);

	/* This run has an intentional mistake in the song*/
	for (i = 0; i < NUM_NOTES; i++) {
		compare_notes(expected[i], actual[i], i, fbp);
		sleep(1);
	}

	get_lcd_input(&si);
	/* Reset notes to black */
	clear_notes(0, expected, actual, fbp);
	get_lcd_input(&si);
	/* This run is perfect ;) */
	for (i = 0; i < NUM_NOTES; i++) {
		compare_notes(expected[i], expected[i], i, fbp);
		sleep(1);
	}

	get_lcd_input(&si);
	cleanup_display(fbp, &fbfd);
	metronome(100);
	return 0;
}
