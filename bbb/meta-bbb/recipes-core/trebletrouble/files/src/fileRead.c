#include <stdio.h>
#include<stdlib.h>
#include "fileFormat.h"


	



Note * readTwinkle(Song* ttls, char * filename)
{
	Note *notes;
	FILE * twinkle;
	twinkle = fopen(filename,"rb");
	if (!twinkle) {
		printf("Unable to open file!");
		return 0;
	}
	ttls->sfh = (SongFormatHeader *)malloc(sizeof(SongFormatHeader));
	printf("Read %ld bytes for header\n", fread(ttls->sfh, sizeof(SongFormatHeader), 1, twinkle));
	ttls->fbar = (Bar *)malloc(sizeof(Bar)*ttls->sfh->numBars+sizeof(Note)*ttls->sfh->numNotes);
	fread(ttls->fbar, sizeof(Bar)*ttls->sfh->numBars+sizeof(Note)*ttls->sfh->numNotes,1,twinkle);
	notes = (Note *)(ttls->fbar + ttls->sfh->numBars);
	fclose(twinkle);
	return notes;
}

void parseBar(Song* ttls,Note * notes)
{
	int i;
	int j;
	//read the Bars and the notes in the bar
	for (i = 0 ; i< ttls->sfh->numBars;i++)
		{
			printf("Bar %d NumNotes: %d\n",i,*(ttls->fbar));
			
			for (j = 0; j < *(ttls->fbar);j++)
				{
					
					printf("Note %d of Bar %d:\nduration:%f\nfrequency:%f\nquantumvalue:%d \n ",j,i,notes->duration,notes->frequency,notes->quantumvalue);
					notes++;
	
				
				}
			
			ttls->fbar ++;
			
		}
	printf("That is the end of the program");
 
}

//Free Song
void freeSong(Song* ttls)
{
	
	free(ttls->fbar);
	free(ttls->sfh);
	free(ttls);
}


//Increment Bar 
void incBar(Song* ttls)
{
	ttls->fbar ++;
}

//Increment Note
void incNote(Note * notes)
{
	notes++;
}



//Getters for Header Info


char* getName(Song* ttls)
{
	return ttls->sfh->name;
}

int getBpm(Song* ttls)
{
	return ttls->sfh->bpm;
}
int getNumBars(Song*ttls)
{
	return ttls->sfh->numBars;
}
int getNumNotes(Song*ttls)
{
	return ttls->sfh->numNotes;
}
char getKey(Song*ttls)
{
	return ttls->sfh->key;
}

unsigned char getTimeSig(Song*ttls)
{
	return ttls->sfh->timeSignature;
}

void printHeader( Song* ttls)
{
	printf("Name:%s\n,Bpm:%d\n,Number of Bars:%d\n,Number of Notes:%d\n,Key:%c\n,TimeSignature:%c\n",getName(ttls),getBpm(ttls),getNumBars(ttls),getNumNotes(ttls),getKey(ttls),getTimeSig(ttls));

}


//Getters for notes

float getDuration(Note *notes)
{
	return notes->duration;
}

float getFrequency(Note *notes)
{
	return notes->frequency;
}
int getNoteValue(Note *notes)
{
	return notes->quantumvalue;
}

void printNote(Note * notes)
{
	printf("Duration%f\nFrequency%f\nNoteValue%d\n",getDuration(notes),getFrequency(notes),getNoteValue(notes));
}


