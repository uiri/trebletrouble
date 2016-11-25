#include "colours.h"
#include "display.h"
#include "input.h"
#include "menu.h"

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

	int expected[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65};
	int freq;
	float pitch;

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
	return 0;
}


