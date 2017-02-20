/*
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
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#include "simpleAlsa.h"

struct timeval start,last;
 
int64_t tv_to_u(struct timeval s)
{
	return s.tv_sec * 1000000 + s.tv_usec;
}
 
void beat(int delay)
{
	snd_pcm_t *pcm_handle;
	char *infilename = "/srv/trebletrouble/metro_1.wav" ;
	struct timeval tv;
	int dir;
	int64_t d, corr, slp, cur, next;
	unsigned int samplerate;
	tv = start;
	d = corr = 0;
	dir = 0;
	next = tv_to_u(start) + delay;
	samplerate = init_sndfile(infilename);
	pcm_handle=init_pcm(samplerate);
	while (1) {
		gettimeofday(&tv, 0);
		slp = next - tv_to_u(tv) - corr;
		usleep(slp);
		gettimeofday(&tv, 0);
		play_sndfile(pcm_handle);
		fflush(stdout);
 		dir = !dir;
 		cur = tv_to_u(tv);
		d = cur - next;
		corr = (corr + d) / 2;
		next += delay;
 	}
	cleanup_pcm(&pcm_handle);
}
 
void metronome(int bpm)
{
	if (bpm > 600) {
		fprintf(stderr, "frequency %d too high\n", bpm);
		exit(1);
	}
	gettimeofday(&start, 0);
	last = start;
	beat(60 * 1000000 / bpm);

}
