#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "colours.h"
#include "display.h"

extern long SCREENSIZE;
extern int DISP_WIDTH;

//char *N[12] = {"a", "as", "b", "c", "cs", "d", "ds", "e", "f", "fs", "g", "gs"};
//int O[9] = {0,1,2,3,4,5,6,7,8};

void screen_capture(char* fbp, char* filename){

	FILE* file;
	
	char* e = fbp+800*480*2;
	
	/* creating buf for r -> buf[0], g -> buf[1], b -> buf[2] */
	unsigned char buf[3], px[2];

	file = fopen(filename, "wb");
	
	if (file == NULL){
		printf("Error: file is Null.\n");
		return;
	}
	
	fwrite("P6\n800 480\n255\n",1, 15, file); 
	
	/* wirte fbp to pnm stuff */
	for (;fbp<e;fbp++) {
		
		px[0] = *fbp;
		fbp++;
		px[1] = *fbp;
		

		buf[0] = (px[0] & 0xF8);
		buf[1] = (((px[0] & 0x07)<<5)| ((px[1] & 0xE0)>>3));
		buf[2] = ((px[1] & 0x1F)<<3);
		
			
		fwrite(buf, 1, 3, file);

	}

	fclose(file);
}

int main(int argc, char** argv){

	/*set screensize and display_width for testing*/
	SCREENSIZE = 800*480*2;
	DISP_WIDTH = 800;
	/* Char for testing fbp */
	char* fbp = malloc(SCREENSIZE);

	/*draw colour to screen*/
	colour_screen(fbp, WHITE);	
	
	/*draw staff on screen*/
	draw_staff(fbp);	
	
	/*variables for reading song*/
	FILE *song = fopen(SONG, "rb");
	load_song(song, fbp);
	fclose(song);

	/*writes the thing to a pnm hopefully --update: it does*/
	screen_capture(fbp, "screenshot.pnm");
	
	/*~goodbye friends~*/
	free(fbp);
	return 0;

}
