/*
 *  Copyright (C) 2016-2017  William Pearson
 *  Copyright (C) 2016-2017  Emilie Cobbold
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
#include <string.h>
#include <stdlib.h>

#include "colours.h"
#include "fileFormat.h"
#include "fileRead.h"
#include "display.h"
#include "find_freq.h"


extern int DISP_WIDTH;
extern long SCREENSIZE;

static int x_start;
static int BARSP;
static int NOTESP;

#define END (DISP_WIDTH-16)
#define KEYS 88
#define NSP 30

#define CALC_OCTAVE_BOUND(N) (12*N + 2)

char *OCTAVE[9] = {"/srv/trebletrouble/0.pnm", "/srv/trebletrouble/1.pnm", "/srv/trebletrouble/2.pnm", "/srv/trebletrouble/3.pnm", "/srv/trebletrouble/4.pnm", "/srv/trebletrouble/5.pnm", "/srv/trebletrouble/6.pnm", "/srv/trebletrouble/7.pnm", "/srv/trebletrouble/8.pnm"};

char *TIME_STAMP[10] = {"","","/srv/trebletrouble/2_ts.pnm","/srv/trebletrouble/3_ts.pnm","/srv/trebletrouble/4_ts.pnm","/srv/trebletrouble/5_ts.pnm","","/srv/trebletrouble/7_ts.pnm","/srv/trebletrouble/8_ts.pnm", ""};

/* Internal functions */

int get_xnote(int i) {
	/* First note is at 145,
	   each subsequent note is 34 px further */
	return x_start + i*XS + BARSP + NOTESP;
}

void add_notespace(int value)
{
	if (value == 16)
		NOTESP += NSP;
	else if (value == 32)
		NOTESP += NSP*4;
}

void remove_notespace(int value)
{
	if (value == 16)
		NOTESP -= NSP;
	else if (value == 32)
		NOTESP -= NSP*4;
}

void draw_note(int i, int ynote, char* fbp, short colour, int value) {
	if(ynote < 61){
		bitblit_colour(L_PIC, fbp, get_xnote(i)-3, 45, colour);
	}
	if(ynote < 31){
		bitblit_colour(L_PIC, fbp, get_xnote(i)-3, 15, colour);
	}
	if(ynote > 239){
		bitblit_colour(L_PIC, fbp, get_xnote(i)-3, 225, colour);
	}

	/*if ynote on treble clef is above B - flip the stem
	 150 = 90+30+30*/
	if(ynote < 150) {
		if (value == 2) {
			bitblit_colour(S_N_F, fbp, get_xnote(i), ynote, colour);
		} else if (value == 4) {
			bitblit_colour(E_N_F, fbp, get_xnote(i), ynote, colour);
		} else if (value == 8) {
			bitblit_colour(Q_N_F, fbp, get_xnote(i), ynote, colour);
		} else if (value == 16) {
			bitblit_colour(H_N_F, fbp, get_xnote(i), ynote, colour);
		} else {
			bitblit_colour(W_N, fbp, get_xnote(i), ynote-15, colour);
		}
	} else {
		if (value == 2) {
			bitblit_colour(S_N, fbp, get_xnote(i), ynote-105, colour);
		} else if (value == 4) {
			bitblit_colour(E_N, fbp, get_xnote(i), ynote-105, colour);
		} else if (value == 8) {
			bitblit_colour(Q_N, fbp, get_xnote(i), ynote-105, colour);
		} else if (value == 16) {
			bitblit_colour(H_N, fbp, get_xnote(i), ynote-105, colour);
		} else {
			bitblit_colour(W_N, fbp, get_xnote(i), ynote-15, colour);
		}
	}
	add_notespace(value);
}

void set_time_signature(int t1,int t2, char *fbp){
	int y = 90;
	bitblit(TIME_STAMP[t1], fbp, x_start, y);
	bitblit(TIME_STAMP[t2], fbp, x_start, y+60);
	x_start = x_start+25;
}

void draw_bar(char *fbp, int x_pos){
	BARSP+= 10;
	bitblit(BAR, fbp,get_xnote(x_pos)-10,90);
}

int draw_key(char *fbp, char key){
	/* p is x variable for the pitch signatures*/
	int p = 64;
	/* m is the x interval for each pitch */
	int m = 15;
	int y = 90;
	/*Drawing key Signature*/
	if (0x80 & key){
		if (0x40 & key){
			bitblit(SHARP, fbp, p, y-15);
			p = p+m;
		}
		if(0x20 & key){
			bitblit(SHARP, fbp, p, y+30);
			p = p+m;
		}
		if(0x10 & key){
			bitblit(SHARP, fbp, p, y-30);
			p = p+m;
		}	
		if(0x08 & key){
			bitblit(SHARP, fbp, p, y+15);
			p = p+m;
		}
		if(0x04 & key){
			bitblit(SHARP, fbp, p, y+60);
			p = p+m;
		}
		if(0x02 & key){
			bitblit(SHARP, fbp, p, y);
			p = p+m;
		}
		if(0x01 & key){
			bitblit(SHARP, fbp, p, y+45);
			p = p+m;
		}
	}else{
		if (0x40 & key){
			bitblit(FLAT, fbp, p, y+45);
			p = p+m;
		}
		if(0x20 & key){
			bitblit(FLAT, fbp, p, y);
			p = p+m;
		}
		if(0x10 & key){
			bitblit(FLAT, fbp, p, y+60);
			p = p+m;
		}
		if(0x08 & key){
			bitblit(FLAT, fbp, p, y+15);
			p = p+m;
		}
		if(0x04 & key){
			bitblit(FLAT, fbp, p, y+75);
			p = p+m;
		}	
		if(0x02 & key){
			bitblit(FLAT, fbp, p, y+30);
			p = p+m;
		}
		if(0x01 & key){
			bitblit(FLAT, fbp, p, y+90);
			p = p+m;
		}
	}
	return p;
}

void draw_staff_lines(char *fbp){
	/* place staff (five lines) on screen */
	int i;
	int x = 0;
	int y = 90;
	int m = 30;
	/* draw the lines of the staff */
	for(i=0; i < 5; i++, y+=m)
		bitblit("/srv/trebletrouble/line.pnm", fbp, x, y);
}
/* External API */


int find_ind(char note, int oct){
	int ind, v;

	ind = (oct*12);

	v = ((note - 'C'+7)%7)*2;

	if (v > 4)
		v--;

	return ind+v-9;
}

int normalize_freq(int index)
{
	while (CALC_OCTAVE_BOUND(4) < index)
		index -= 12;
	while (index < (CALC_OCTAVE_BOUND(3)+1))
		index += 12;
	return index;
}

int bitblit_colour(char* filename, char* fbp, int x, int y, short colour) {
	FILE* fd;
	int i, j, w, h, vw;
	unsigned char buf[3], px[2];

	if (y < 0 || y > 480)
		return -4;

	if (x < 0 || x > 800)
		return -5;

	if (!fbp)
		return -3;

	fd = fopen(filename, "rb");
	if (fd == NULL)
		return -1;

	fread(buf, 1, 3, fd);
	if (buf[0] != 'P' || buf[1] != '6') {
		fclose(fd);
		return -2;
	}

	fread(buf, 1, 3, fd);
	w = 0;
	for (i = 0; i < 3; i++) {
		if (buf[i] == ' ' || (9 < buf[i] && buf[i] < 13))
			break;
		w *= 10;
		w += buf[i] - '0';
	}
	w--;
	j = 0;
	if (i == 3) {
		/* throw away whitespace */
		fread(buf, 1, 1, fd);
	} else if (i < 2) {
		for (; j+i+1 < 3; j++) {
			buf[j] = buf[j+i+1];
		}
	}
	fread(buf+j, 1, 3-j, fd);
	h = 0;
	for (i = 0; i < 3; i++) {
		if (buf[i] == ' ' || (9 < buf[i] && buf[i] < 13))
			break;
		h *= 10;
		h += buf[i] - '0';
	}
	h--;
	if (i == 3) {
		buf[0] = fgetc(fd);
	}
	do {
		buf[0] = fgetc(fd);
	} while (buf[0] != ' ' && (buf[0] < 9 || 13 < buf[0]));

	vw = (x+w) > 800 ? x+w-799 : 0;

	for (i = 0; i < SCREENSIZE/2; i++) {
		if ((i / DISP_WIDTH) < y)
			continue;
		if (vw && (i % DISP_WIDTH) == DISP_WIDTH-1)
			fseek(fd, 3*vw, SEEK_CUR);
		if ((i % DISP_WIDTH) < x || (x+w) < (i % DISP_WIDTH))
			continue;
		if ((y+h) < (i / DISP_WIDTH))
			break;
		if (fread(buf, 1, 3, fd) != 3) {
			break;
		}
		if (buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0xFF)
			continue;
		if (buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0x00) {
			memset(fbp + 2*i, colour, 2);
			continue;
		}
		px[0] = (buf[0] & 0xF8) | ((buf[1] & 0xE0) >> 5);
		px[1] = ((buf[1] & 0x1C) << 3) | ((buf[2] & 0xF8) >> 3);
		memset(fbp + 2*i, *(short*)px, 2);
	}

	fclose(fd);
	return 0;
}

void colour_screen(char* fbp, short colour) {
	int i;

	if (!fbp)
		return;
	for (i = 0; i < SCREENSIZE/2; i++) {
		/*
		 * So, we have 16-bits per pixel. It is split up into
		 * 5 bits for Red, 6 bits for Green and 5 bits for Blue
		 * Why 6 bits for Green? Human biology, that's why.
		 * The least significant _byte_ is first and the most
		 * significant byte is second. Byte order is always the same.
		 * 0x25E2 is actually the colour E225 which is orange.
		 * Red - 1110 0; Green - 010 001; Blue - 0 0101)
		 */
		memset(fbp + 2*i, colour, 2);
	}
}

int bitblit(char* filename, char* fbp, int x, int y) {
	return bitblit_colour(filename, fbp, x, y, BLACK);
}

void draw_staff(char* fbp){
	int y = 90;
	/* m is the space between the lines */
	int start =0;
	int end = 800;
	/* draw clef and end lines */
	bitblit("/srv/trebletrouble/clef.pnm", fbp, start, y-35);
	bitblit("/srv/trebletrouble/bar.pnm", fbp, end-16, y);
	bitblit("/srv/trebletrouble/end.pnm", fbp, end-7, y);

	draw_staff_lines(fbp);
}

int get_ynote(int i) {
	int j;

	/* How does this work?
	 * A0 is at index 0, so the value of actual will be 0 in that case.
	 * We want to re-index it so that C0 is 0 which means adding 9 for the
	 * calculations hence the subtraction after we have the value for ynote.
	 * Modulo 12 accounts for where the index is within the octave. Now we
	 * need to map these to letter notes:
	 *	0, 1  -> C ( 0 )
	 *	2, 3  -> D ( 1 )
	 *	4     -> E ( 2 )
	 *	5, 6  -> F ( 3 )
	 *	7, 8  -> G ( 4 )
	 *	9, 10 -> A ( 5 )
	 *	11    -> B ( 6 )
	 * If we divide by 2, the odd numbers will go have their result rounded
	 * down. This is good for 1 and 3 but for values over 5, we want them
	 * rounded up. This is easily accomplished by incrementing those values
	 * by 1 before performing the division.
	 *
	 * The spacing between successive notes is 15 so we multiple our j value
	 * by 15.
	 *
	 * The spacing between successive octaves is 7 notes - 15 * 7 = 105
	 * We have set C4 to 240. C is mapped to 0 so no need to account for any
	 * adjustment there. But, the octave number will be 4, so we need to
	 * subtract 4 before multiplying by 105. Octaves which are lower will
	 * have a negative portion here and octaves which are higher will have a
	 * positive portion.
	 *
	 * B3 will be mapped to 6 and 3 -> 6*15 = 90, 105*(3-4) = -105
	 * 90-105 = -15 - exactly where we want to place B3 relative to C4
	 */
	i += 9;
	j = i%12;
	if (j > 4)
		j++;
	j /= 2;
	return (240 - (15*j + 105*(i/12 - 4)));
}


int compare_notes(Song* song, Note* note, int* actuals, int i, int j, char* fbp, int barspace)
{
	int k, notes_back;
	int* expected = song->expected + i;
	short colour = (*expected == actuals[i]) ? GREEN : RED;

	for (notes_back = 0; colour == RED && (j - notes_back); notes_back++) {
		if (actuals[i] == *(expected - notes_back)) {
			actuals[i - notes_back] = actuals[i];
			i -= notes_back;
			colour = GREEN;
			break;
		}
	}

	if (colour == RED)
		notes_back = 0;

	for (k = 1; k < notes_back+1; k++) {
		remove_notespace(getNoteValue(note-k));
	}

	BARSP = barspace;
	draw_note(i, get_ynote(actuals[i]), fbp, colour, getNoteValue(note-notes_back));
	BARSP = 0;

	return notes_back;
}

Bar* find_worst_bar(Song* song, int* actuals) {

  int i, *numWrongNotes, j;
  float *errorPercentage;
  Bar *fbar, *worstBar;

  fbar = song->fbar;

  numWrongNotes = malloc(sizeof(int) * song->sfh->numBars);
  errorPercentage = malloc(sizeof(float) * song->sfh->numBars);

  for (j = 0, i = 0; j < song->sfh->numNotes; i++, j++) {

    // increment the number of wrong notes in this bar if it does not match expected
    if (song->expected[j] != actuals[j]) {
      numWrongNotes[fbar - song->fbar]++;
    }
	       
    if (i == fbar->notes) {
		  	
      // calculate the percentage error per bar
      errorPercentage[fbar - song->fbar] = ( numWrongNotes[fbar - song->fbar] ) / fbar->notes;

      // set the the worst bar by comparing the maximum %error of the current bar and the last bar.
      if ( (fbar - song->fbar) > 0) { 
	if ( errorPercentage[fbar - song->fbar] > errorPercentage[ (fbar - song->fbar) - 1] )
	  worstBar = fbar;
      } else {
	worstBar = song->fbar;
      }

      fbar++;
      i = 0;

    }
		
  }

  free(numWrongNotes);
  free(errorPercentage);

  return worstBar;

}
	
void clear_notes(int i, int *expected, int *actual, char* fbp, int len, int value) {
	int actual_ynote;
	for (; i < len; i++) {
		actual_ynote = get_ynote(actual[i]);
		draw_note(i, actual_ynote, fbp, WHITE, 1);
		if (!(actual_ynote%30))
			bitblit(L_PIC, fbp, get_xnote(i)-3, get_ynote(actual[i])-14);
		draw_note(i, get_ynote(expected[i]), fbp, BLACK, value);
	}

void clear_all_notes(Song* song, int* actuals, char* fbp, int ind, int length){
	int actual_ynote, i, j, time;

	Note * notes = (Note *)(song->fbar+song->sfh->numBars);
	BARSP = 0;
	NOTESP = 0;
	printf("Clearing the notes\n");

	Bar* fbar;
	fbar = song -> fbar;
	for(i=0, j=0; i < ind; i++, j++, notes++){
		if (j == fbar->notes){
			fbar++;
			j = 0;
		}
	}
	for(i=0, j=0; i < length; i++, j++, notes++){
		if (j == fbar -> notes){
			BARSP+=10;
			fbar++;
			fbar->barspace = BARSP;
			j = 0;
		}
		time = getNoteValue(notes);
		actual_ynote = get_ynote(actuals[i]);

		draw_note(i, actual_ynote, fbp, WHITE, time);
	}
	draw_staff_lines(fbp);
	NOTESP = 0;
	BARSP = 0;

}

void load_start_song(char *fbp, Song * song){
	/*Time signature:
	Timesig = Ts1-1 * 16 + Ts2-1
	so for 4/4= 51
	so for 3/4= 35*/

	/*Draw the key, return the current x value*/
	x_start = draw_key(fbp, song->sfh->key);
	printf("Drawing the notes\n");

	/*
	 * Take ints from char away
	 * Don't forget to change millisec calculations if the timeSignature
	 * format changes!
	 */
	int ts1 = (song->sfh->timeSignature/16)+1;
	int ts2 = (song->sfh->timeSignature%16)+1;
	set_time_signature(ts1, ts2, fbp);
}
/*loads a partial part of the song, given the 4 bars at a time.*/
Bar * load_song(char *fbp, Note * notes, Song * song, int x_s, Bar * fbar){
	int i, j, ynote, time, note;
	double freq;

	BARSP = 0;
	NOTESP = 0;


	for(i = 0, j=0; get_xnote(i) < END; j++, i++, notes++){
		if ( j == fbar->notes){
			draw_bar(fbp, i);
			fbar++;
			fbar->barspace = BARSP;
			printf("**:%d\n",fbar-song->fbar);
			j = 0;
			if (get_xnote(i+fbar->notes-1)>= END-30)
				break;
		}

		time = getNoteValue(notes);
		freq = getFrequency(notes);
		
		note = find_freq(freq);
		ynote  = get_ynote(note);

		draw_note(i, ynote, fbp, BLACK, time);

	}
	NOTESP = 0;
	BARSP = 0;
	return fbar;
}
