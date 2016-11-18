#include <stdio.h>
#include <unistd.h>

#include "colours.h"
#include "display.h"
#include "input.h"

int main(int argc, char** argv) {
	char* fbp;
	int fbfd, tsfd, err, i;
	int actual[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 62, 63, 65};
	int expected[NUM_NOTES];
	ScreenBounds sb;
	ScreenInput si;

	fbp = init_display(&fbfd);

	err = init_touchscreen(&tsfd, &sb);
	if (err) {
		printf("Error reading event input file\n");
		return err;
	}

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
	load_song(song, fbp, expected);
	fclose(song);
	sleep(2);

	/* This run has an intentional mistake in the song*/
	for (i = 0; i < NUM_NOTES; i++) {
		compare_notes(expected[i], actual[i], i, fbp);
		sleep(1);
	}

	/* Reset notes to black */
	clear_notes(0, expected, actual, fbp);
	/* This run is perfect ;) */
	for (i = 0; i < NUM_NOTES; i++) {
		compare_notes(expected[i], expected[i], i, fbp);
		sleep(1);
	}

	while (1) {
		si = get_lcd_input(&tsfd, &sb);
		/* Need to figure out what to do with the input once we 
		   have it */
		/* if (si.y < 512) */
		/* 	colour_screen(fbp, RED); */
		/* else if (si.y < 3930) */
		/* 	colour_screen(fbp, GREEN); */
		/* else */
		/* 	colour_screen(fbp, ORANGE); */
		sleep(1);
	}

	cleanup_display(fbp, &fbfd);
	while(1); /* fuck it */
	return 0;
}
