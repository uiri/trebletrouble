/*
 *  Copyright (C) 2016-2017  William Pearson
 *  Copyright (C) 2016-2017  Emilie Cobbold
 *  Copyright (C) 2016-2017  Daniel Rosales
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
#include <unistd.h>
#include <stdlib.h>

#include "colours.h"
#include "fileFormat.h"
#include "fileRead.h"
#include "display.h"
#include "input.h"
#include "metronome.h"
#include "alsa.h"
#include "find_freq.h"
#define SONG "/srv/trebletrouble/TwinkleTwinkleLittleStar.bin"

void play_song_menu(char* fbp, ScreenInput *si)
{
	/*FILE* songfile;*/
	int i, j, k, m, *actuals, x_s;
	float pitch;
	Wave* wave;
	snd_pcm_t *pcmh;
	Song* song;
	Note* notes, *notes_p;
	Bar* fbar, *firstNote, *worstBar, *fbar_n, *fbar_p;
	
	colour_screen(fbp, WHITE);
	
	/*draw staff on screen*/
	draw_staff(fbp);
	
	/*Code for new song format*/
	song = malloc(sizeof(Song));
	
	notes = readTwinkle(song, SONG);

	fbar = song->fbar;
	pcmh = init_pcm(SAMPLE_RATE);
	x_s = load_start_song(fbp, song->sfh->key, song->sfh->timeSignature);
	fbar_p = fbar;
	notes_p = notes;
	fbar_n = load_song(fbp, notes, song, x_s, fbar);

	actuals = malloc(sizeof(int) * song->sfh->numNotes);
	firstNote = song->fbar + song->sfh->numBars;

	for (i = 0, j=0, k=0; k+i < song->sfh->numNotes; i++, j++, notes++) {
		if ( j == fbar->notes){
			fbar++;
			j = 0;
			printf("Next bar\n");
			if (fbar == firstNote)
				break;
			if( fbar == fbar_n){
				printf("Clearing notes to draw new bars\n");
				clear_all_notes(notes_p, fbar_p, actuals+k, fbp, song->sfh->key, song->sfh->timeSignature);
				k += i;
				i = 0;
				fbar_p = fbar;
				notes_p = notes;
				fbar_n = load_song(fbp, notes, song, x_s, fbar);
			}
		}
		printf("Start compare notes\n");
		wave = makeWave(notes->duration);
		/* Change duration based on expected length of note */
		/* Section: Audio Recording */
		if (recordWAV(wave, notes->duration)) {
			printf("Oh no! An error with the mic!\n");
			continue;
		}

		play_wave(pcmh, wave);
		pitch = get_pitch(wave);
		printf("Recognized pitch %f\n", pitch);
		/* need to store the found freqs in actuals or something */
		actuals[k+i] = find_freq(pitch);
		m = compare_notes(song, notes, actuals, k+i, j,i, fbp, fbar->barspace);
		printf("End compare notes\n");
		j -= m;
		i -= m;
		waveDestroy(wave);
	}
	worstBar = find_worst_bar(song, actuals);
	cleanup_pcm(pcmh);
	sleep(10);

	/* Reset notes to black */
	/* Need to fix this. Length needs to come from somewhere... */
	/*clear_notes(0, song->expected, actual, fbp, len);*/
	freeSong(song);
	free(actuals);

	colour_screen(fbp, WHITE);
}

void metronome_menu(char* fbp, ScreenInput *si)
{
	colour_screen(fbp, GREEN);
	metronome(100);
}
