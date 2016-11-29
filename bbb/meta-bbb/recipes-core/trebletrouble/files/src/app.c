#include <stdio.h>
#include <unistd.h>

#include "display.h"
#include "colours.h"

int main(int argc, char** argv) {
	char* fbp;
	int fbfd, err;

	fbp = init_display(&fbfd);
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
	sleep(5);
	colour_screen(fbp, WHITE);

	/*draw staff on screen*/
	draw_staff(fbp);	
	
	/*variables for reading song*/
	FILE *song = fopen(SONG, "rb");
	load_song(song, fbp);	
	cleanup_display(fbp, &fbfd);
	while(1); /* fuck it */
	return 0;
}
