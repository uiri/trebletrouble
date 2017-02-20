#include <stdio.h>
#include<stdlib.h>
#include "fileFormat.h"
#include "fileRead.h"



int main(int argc, char** argv){
	
	song ttls;
	note * notes;
	createTwinkle();
	notes=readTwinkle(&ttls);
	printHeader(&ttls);
	printNote(notes);
	//parseBar(&ttls,notes);
	freeSong(&ttls);
	return 1;
}
