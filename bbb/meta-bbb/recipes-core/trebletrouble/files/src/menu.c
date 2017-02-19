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

#include "colours.h"
#include "display.h"
#include "input.h"
#include "metronome.h"
#include "audio_recorder.h"

void play_song_menu(char* fbp, ScreenInput *si) {
	FILE *song;
	int actual[NUM_NOTES] = {39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 62, 63, 65};
	int expected[NUM_NOTES];
	int i;
	Wave* wave;
	uint32_t duration = 1; /* 1 sec*/

	colour_screen(fbp, WHITE);

	/*draw staff on screen*/
	draw_staff(fbp);

	/*variables for reading song*/
	song = fopen(SONG, "rb");

	load_song(song, fbp, expected);
	fclose(song);
	/* get_lcd_input(si); */

	wave = malloc(sizeof(Wave));
	/* This run has an intentional mistake in the song*/
	for (i = 0; i < NUM_NOTES; i++) {
		/* Section: Audio Recording */
	        audio_recorder(wave,duration);
	        compare_notes(expected[i], get_pitch(wave,duration), i, fbp);
	}

	waveDestroy(wave);
	/* get_lcd_input(si); */
	/* Reset notes to black */
	clear_notes(0, expected, actual, fbp);
	/* get_lcd_input(si); */
	/* This run is perfect ;) */
	for (i = 0; i < NUM_NOTES; i++) {
		compare_notes(expected[i], expected[i], i, fbp);
		sleep(1);
	}
}

void metronome_menu(char* fbp, ScreenInput *si) {
	colour_screen(fbp, GREEN);
	metronome(100);
}
