#include <stdio.h>
#include<stdlib.h>
#include "fileFormat.h"


void createTwinkle()
{
	makeBin();

}

note * readTwinkle(song * ttls)
{
	note *notes;
	FILE * twinkle;
	twinkle = fopen(TWINKLE_FILENAME,"rb");
	if (!twinkle) {
		printf("Unable to open file!");
		return 0;
	}

	printf("Read %ld bytes for header\n", fread(ttls->sfh, sizeof(songFormatHeader), 1, twinkle));
	ttls->fbar = (bar *)malloc(sizeof(bar)*ttls->sfh->numBars+sizeof(note)*ttls->sfh->numNotes);
	fread(ttls->fbar, sizeof(bar)*ttls->sfh->numBars+sizeof(note)*ttls->sfh->numNotes,1,twinkle);
	notes = (note *)(ttls->fbar + ttls->sfh->numBars);
	fclose(twinkle);
	return notes;
}

void parseBar(song * ttls,note * notes)
{
	int i;
	int j;
	//read the Bars and the notes in the bar
	for (i = 0 ; i< ttls->sfh->numBars;i++)
		{
			printf("Bar %d NumNotes: %d\n",i,ttls->fbar->numNotes);
			
			for (j = 0; j < ttls->fbar->numNotes;j++)
				{
					
					printf("Note %d of Bar %d:\nduration:%d\nfrequency:%f\nquantumvalue:%d \n ",j,i,notes->duration,notes->frequency,notes->quantumvalue);
					notes++;
	
				
				}
			
			ttls->fbar ++;
			
		}
	printf("That is the end of the program");
 
}

//Free Song 
void freeSong(song * ttls)
{

free(ttls->fbar);

}


//Increment Bar 
void incBar(song * ttls)
{
	ttls->fbar ++;
}

//Increment Note
void incNote(note * notes)
{
	notes++;
}



//Getters for Header Info


char* getName(song * ttls)
{
	return ttls->sfh->name;
}

int getBpm(song * ttls)
{
	return ttls->sfh->bpm;
}
int getNumBars(song *ttls)
{
	return ttls->sfh->numBars;
}
int getNumNotes(song *ttls)
{
	return ttls->sfh->numNotes;
}
char getKey(song *ttls)
{
	return ttls->sfh->key;
}

unsigned char getTS(song *ttls)
{
	return ttls->sfh->timeSignature;
}

void printHeader( song * ttls)
{
	printf("Name:%s\n,Bpm:%d\n,Number of Bars:%d\n,Number of Notes:%d\n,Key:%c\n,TimeSignature:%c\n",getName(ttls),getBpm(ttls),getNumBars(ttls),getNumNotes(ttls),getKey(ttls),getTS(ttls));

}


//Getters for notes

int getDuration(note *notes)
{
	return notes->duration;
}

float getFrequency(note *notes)
{
	return notes->frequency;
}
int getNoteValue(note *notes)
{
	return notes->quantumvalue;
}

void printNote(note * notes)
{
	printf("Duration%d\nFrequency%f\nNoteValue%d\n",getDuration(notes),getFrequency(notes),getNoteValue(notes));
}


