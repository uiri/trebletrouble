#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "colours.h"
#include "display.h"

extern long SCREENSIZE;
extern int DISP_WIDTH;

char *N[12] = {"a", "as", "b", "c", "cs", "d", "ds", "e", "f", "fs", "g", "gs"};
int O[9] = {0,1,2,3,4,5,6,7,8};


char *NOTE_PIC = "/srv/trebletrouble/note.pnm";
char *L_PIC = "/srv/trebletrouble/note_l.pnm";
char *SONG = "/srv/trebletrouble/twinkle_v1"; 

void screen_capture(char* fbp, char* filename){

	FILE* file;
	
	char* e = fbp+800*480*2;
	
	//creating buf for r -> buf[0], g -> buf[1] & buf[2], b -> buf[3]
	unsigned char buf[3], px[2];
	

	file = fopen(filename, "wb");
	fwrite("P6\n800 480\n255\n",1, 15, file); 
	
	if (file == NULL){
		printf("Error: file is Null.\n");
		return;
	}
	//wirte fbp to pnm stuff
		
	for (;fbp<e;fbp++) {
		
		px[0] = *fbp;
		fbp++;
		px[1] = *fbp;
		
		//check nonblack pixels because black pixels are scary
		/*if(px[0] || px[1])
			printf("NonBlack pixel\n");*/

		buf[0] = (px[0] & 0xF8);
		buf[1] = (((px[0] & 0x07)<<5)| ((px[1] & 0xE0)>>3));
		buf[2] = ((px[1] & 0x1F)<<3);
		
			
		fwrite(buf, 1, 3, file);

	}

	fclose(file);
}

void display_note_test(double freq){
	int ind;
	char *note;
	int oct;

	ind = find_freq(freq);

	note = N[ind % 12];
	oct = O[(ind+10) /12];

	//print out data for testing
	printf("freq: %f, ind: %d, note: %s, octave:%d\n", freq, ind, note, oct);
	
	//bitblit(NOTE_PIC)

}

void draw_note(unsigned char *buf, int xnote, char* fbp){
	int ynote;
	printf("buf:%s\n", buf);
	/*Middle C = 240*/
	/* find the y position for each note*/
	ynote =(240 - (((buf[1]-'4') * 105) + (((buf[0]-'C'+7)%7)*15)));

	bitblit(NOTE_PIC, fbp, xnote, ynote-15);	
}

int main(int argc, char** argv){

//	double freq = 523.251; //Tenor C
	

	int i;
	//set screensize and display_width for testing
	SCREENSIZE = 800*480*2;
	DISP_WIDTH = 800;

	//Char for testing fbp
	char* fbp = malloc(SCREENSIZE);
	
	//draw colour to screen
	colour_screen(fbp, WHITE);	


	//place staff (five lines) on screen 
	int x = 0;  //stop changing me
	int y = 90; 
	int m = 30; //line space
	int start =0;
	int end = 800;
	int xstart = 145;
	int xnote = xstart;
	FILE *song = fopen(SONG, "rb");
	unsigned char buf[2];

	//draw clef and end lines
	bitblit("/srv/trebletrouble/clef.pnm", fbp, start, y-35);	
	bitblit("/srv/trebletrouble/end.pnm", fbp, end-15, y);	
	bitblit("/srv/trebletrouble/end.pnm", fbp, end-5, y);	

	//draw the lines of the staff	
	for(i=0; i < 5; i++){
		bitblit ("/srv/trebletrouble/line.pnm", fbp, x, y);
		y = y+m;
	}
	
	//draw the notes
	while (fread(buf, 1, 2, song) == 2){
		
		draw_note(buf, xnote, fbp);
		xnote = xnote+m+4;
		printf("it works?\n");

	}
	fclose(song);

	//place note on the board	
	/*while (xnote=0;xnote<15 ;xnote++ ){
		display_note_test(*notes*, xnote);	
	}*/

	//writes the thing to a pnm hopefully
	screen_capture(fbp, "screenshot.pnm");
	
	//goodbye friends
	free(fbp);
	return 0;

}
