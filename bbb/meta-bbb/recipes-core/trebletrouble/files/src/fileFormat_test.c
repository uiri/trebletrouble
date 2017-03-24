#include <stdio.h>
#include <stdlib.h>
#include "fileFormat.h"
#include "fileRead.h"
#include <string.h>



int main(int argc, char** argv)
{
	Song * ttls;
	int i;
	ttls =(Song *)malloc (sizeof(ttls));
	Note * notes;
	char binname[80];
	strcpy(tname,argv[1]);
	makeBin(argv[1],binname);
	notes=readTwinkle(ttls,binname);
	printHeader(ttls);
	for(i= 0;i<ttls->sfh->numNotes;i++){
		printNote(notes);
		notes++;
	}
	freeSong(ttls);
	return 1;
}
