#include <stdio.h>
#include<stdlib.h>
#include "fileFormat.h"
#include "fileRead.h"



int main(int argc, char** argv)
{
	Song ttls;
	Note * notes;
	makeBin(TWINKLE_FILENAME);
	notes=readTwinkle(&ttls,TWINKLE_FILENAME);
	printHeader(&ttls);
	printNote(notes);
	//parseBar(&ttls,notes);
	freeSong(&ttls);
	return 1;
}
