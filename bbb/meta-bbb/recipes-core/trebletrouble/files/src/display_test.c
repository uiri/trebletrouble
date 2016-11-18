#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "colours.h"
#include "display.h"


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
		
		if(px[0] || px[1])
			printf("NonBlack pixel\n");

		buf[0] = (px[0] & 0xF8);
		buf[1] = (((px[0] & 0x07)<<5)| ((px[1] & 0xE0)>>3));
		buf[2] = ((px[1] & 0x1F)<<3);
		
			
		fwrite(buf, 1, 3, file);

	}
	

	fclose(file);

}

int main(int argc, char** argv){

	//Char for testing fbp
	char* fbp = malloc(800*480*2);
	//int err;
	
	//write timbits to screen
	//err = bitblit("/srv/trebletrouble/timbit.pnm", fbp, 400, 240);
	

	//test colour screen
	colour_screen(fbp, ORANGE);
	//help with errors
	/*if (err){
		if (err == -1){
			printf("Error: File not found\n");
		} else if (err == -2){
			printf("Error: Not P6 file???\n");
		}	
		return err;
	}*/
	
	//writes the thing to a pnm hopefully
	screen_capture(fbp, "screenshot.pnm");
	
	//goodbye friends
	free(fbp);
	return 0;

}
