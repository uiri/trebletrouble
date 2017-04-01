/*
 *  Copyright (C) 2017  Daniel Rosales
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
		for (j = 0; j < ttls->fbar->notes;j++) {
			printNote(notes);
			notes++;
			
		}
		ttls->fbar ++;
	}
}
