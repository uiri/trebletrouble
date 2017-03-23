#include <stdio.h>
#include<stdlib.h>
#include "fileFormat.h"
#include "find_freq.h"
//Free Song
void freeSong(Song* ttls)
{
	free(ttls->expected);
	free(ttls->fbar);
	free(ttls->sfh);
	free(ttls);
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


/* Getters for notes */

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

/* Read and parsing logic */

Note * readTwinkle(Song* ttls, char * filename)
{
	Note *notes;
	int i;
	FILE * twinkle;
	twinkle = fopen(filename,"rb");
	if (!twinkle) {
		printf("Unable to open file!");
		return 0;
	}
	ttls->sfh = malloc(sizeof(SongFormatHeader));
	printf("Read %ld bytes for header\n", fread(ttls->sfh, sizeof(SongFormatHeader), 1, twinkle));
	ttls->fbar = malloc(sizeof(Bar)*ttls->sfh->numBars+sizeof(Note)*ttls->sfh->numNotes);
	fread(ttls->fbar, sizeof(Bar)*ttls->sfh->numBars+sizeof(Note)*ttls->sfh->numNotes,1,twinkle);
	notes = (Note *)(ttls->fbar + ttls->sfh->numBars);
	fclose(twinkle);
	ttls->expected = malloc(sizeof(int) * ttls->sfh->numNotes);
	for (i = 0; i < ttls->sfh->numNotes; i++) {
		ttls->expected[i] = find_freq(getFrequency(notes+i));
	}
	return notes;
}

void parseBar(Song* ttls,Note * notes)
{
	int i;
	int j;

	//read the Bars and the notes in the bar
	for (i = 0 ; i< ttls->sfh->numBars;i++) {
		for (j = 0; j < ttls->fbar->notes-1 ;j++) {
			printNote(notes);
			notes++;
			
		}
		ttls->fbar ++;
	}
}
