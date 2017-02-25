/*
 *  Copyright (C) 2016-2017  Emilie Cobbold
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permission under GNU GPL version 3 section 7
 *
 *  If you modify this Program, or any covered work, by linking or
 *  combining it with the uGFX Library, containing parts covered by the
 *  terms of the GFX License, Version 1.2, the licensors of this Program
 *  grant you additional permission to convey the resulting work.
 *  Corresponding Source for a non-source form of such a combination
 *  shall include the source code for the parts of the uGFX Library used
 *  as well as that of the covered work.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "colours.h"
#include "fileFormat.h"
#include "fileRead.h"
#include "display.h"

#define TWINKLE "Twinkle-display.bin"
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
		
	/*Testing code for Winter
	Testing the new Song format*/
	Song * ttls;
	ttls = (Song *)malloc(sizeof(ttls));
	Note * notes;
	makeBin(TWINKLE);
	notes = readTwinkle(ttls,TWINKLE);

	printHeader(ttls);
	printNote(notes);	
	

	load_song(fbp,getBpm(ttls),getNumBars(ttls),getNumNotes(ttls),getKey(ttls),getTimeSig(ttls), notes);

	/*Testing code for Fall
	variables for reading song
	FILE *song = fopen(SONG, "rb");
	load_song(song, fbp);
	fclose(song);*/

	/*writes the thing to a pnm hopefully --update: it does*/
	screen_capture(fbp, "screenshot.pnm");
	
	/*~goodbye friends~*/
	free(fbp);
	freeSong(ttls);
	return 0;

}
