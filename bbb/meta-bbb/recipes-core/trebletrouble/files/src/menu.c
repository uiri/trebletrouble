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

#define TWINKLE "Twinkle-display.bin"

void play_song_menu(char* fbp, ScreenInput *si)
{
	FILE* songfile;
	int i, value;
	float pitch;
	Wave* wave;
	uint32_t duration = 1; /* 1 sec*/
	snd_pcm_t *pcmh;
	Song* song;
	Note* notes;
	
	colour_screen(fbp, WHITE);
	
	/*draw staff on screen*/
	draw_staff(fbp);
	
	/*Code for new song format*/
	song = malloc(sizeof(song));

	/* Need to change this to proper file I/O */
	/* songfile = fopen(SONG, "rb");*/
	makeBin(TWINKLE);
	notes = readTwinkle(song, TWINKLE);
	/*fclose(songfile);*/
	load_song(fbp, notes, song);

	pcmh = init_pcm(SAMPLE_RATE);

	for (i = 0; i < song->sfh->numNotes; i++) {
		wave = makeWave(duration);
		/* Change duration based on expected length of note */
		/* Section: Audio Recording */
		if (recordWAV(wave,duration)) {
			printf("Oh no! An error with the mic!\n");
			continue;
		}

		play_wave(pcmh, wave);
		pitch = get_pitch(wave);
		printf("Recognized pitch %f\n", pitch);
		/* need to store the found freqs in actuals or something */
		value = getNoteValue(notes+i);
		compare_notes(song->expected[i], find_freq(pitch), i, fbp,value);
		waveDestroy(wave);
	}

	cleanup_pcm(pcmh);

	/* Reset notes to black */
	/* Need to fix this. Length needs to come from somewhere... */
	/*clear_notes(0, song->expected, actual, fbp, len);*/
	freeSong(song);
}

void metronome_menu(char* fbp, ScreenInput *si)
{
	colour_screen(fbp, GREEN);
	metronome(100);
}
