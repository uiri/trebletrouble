#include <stdio.h>
#include <stdlib.h>
#include "fileFormat.h"
#include "fileRead.h"
#include <string.h>



int main(int argc, char** argv)
{
	Song * ttls;
	char tname [50] ;
	ttls =(Song *)malloc (sizeof(ttls));
	Note * notes;
	char binname[80];
	strcpy(tname,argv[1]);
	printf("This is what you passes:%s",tname);
	makeBin(argv[1],binname);
	notes=readTwinkle(ttls,binname);
	printHeader(ttls);
	printNote(notes);
	notes++;
	printNote(notes);
	notes++;
	printNote(notes);
	notes++;
	printNote(notes);
	notes++;
	//parseBar(ttls,notes);
	freeSong(ttls);
	return 1;
}
