/*
 *  Copyright (C) 2016-2017  Daniel Rosales
 *  Copyright (C) 2017       William Pearson
 *
 *  Copyright (C) 2009 Alessandro Ghedini <alessandro@ghedini.me>
 *
 *  This file is covered by the terms of BOTH licenses listed below.
 *
 *  --------------------------------------------------------------
 *  "THE BEER-WARE LICENSE" (Revision 42):
 *  Daniel Rosales wrote this file. William Pearson wrote this file.
 *  Alessandro Ghedini wrote this file. As long as you retain this
 *  notice you can do whatever you want with this stuff. If we
 *  meet some day, and you think this stuff is worth it, you can
 *  buy me a beer in return.
 *  --------------------------------------------------------------
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

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <sndfile.h>
#include <math.h>
#include "tone.h"

#define PCM_DEVICE "default"
/* WAV code */
static snd_pcm_uframes_t frames;
static SNDFILE *infile = NULL;

/* added to fix implicit declaration warnings for alloca in target files */
#ifndef alloca
#define alloca(x) __builtin_alloca(x)
#endif

void init_pcm_params(snd_pcm_t *pcm_handle, snd_pcm_hw_params_t *params, unsigned int samplerate, int *dir)
{
	unsigned int pcm;
	/* Set parameters */
	pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
					   SND_PCM_ACCESS_RW_INTERLEAVED);
	if (pcm < 0)
		printf("ERROR: Can't set noninterleaved mode. %s\n",
		       snd_strerror(pcm));
	/* Set formatting */
	pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
					   SND_PCM_FORMAT_S16_LE);
	if (pcm < 0)
		printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

	/* Setting channels */
	pcm = snd_pcm_hw_params_set_channels(pcm_handle, params,1);
	if (pcm < 0)
		printf("ERROR: Can't set channels number. %s\n",
		       snd_strerror(pcm));

	/* Set sample rate */
	pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params,
					      &samplerate,
					      dir);
	if (pcm < 0)
		printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

}

snd_pcm_t * init_pcm(unsigned int samplerate)
{
	snd_pcm_hw_params_t *params;
	snd_pcm_t *pcm_handle;
	int dir;
	unsigned int tmp,pcm;
	dir = 0;
	/* Open the PCM device in playback mode */
	if ((pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
				SND_PCM_STREAM_PLAYBACK, 0)) < 0)
		printf("ERROR: Can't open \"%s\" PCM device. %s\n",
		       PCM_DEVICE, snd_strerror(pcm));

	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(pcm_handle, params);
	init_pcm_params(pcm_handle, params, samplerate, &dir);
	/* Resume information */
	snd_pcm_hw_params_get_channels(params, &tmp);
	if (tmp == 1)
		printf("(mono)\n");
	else if (tmp == 2)
		printf("(stereo)\n");
	snd_pcm_hw_params_get_rate(params, &tmp, 0);
	/* Write parameters */
	snd_pcm_hw_params(pcm_handle, params);
	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	return pcm_handle;
}

unsigned int init_sndfile(char *infilename)
{
	SF_INFO sfinfo;
	if ( access( infilename, F_OK) ==-1) {
		printf("File doesn't exists");
	} else {
		/* printf("File does exist!"); */
	}
	infile = sf_open(infilename, SFM_READ, &sfinfo);
	return sfinfo.samplerate;
}


void cleanup_pcm(snd_pcm_t *pcm_handle)
{
	/*Drain the buffer*/
	snd_pcm_drain(pcm_handle);
	/*Close the device*/
	snd_pcm_close(pcm_handle);
}

void sound(snd_pcm_t *pcm_handle, void *buf, int readcount)
{
	int pcmrc;
	pcmrc = snd_pcm_writei(pcm_handle, buf, frames);
	if (pcmrc == -EPIPE) {
		/* fprintf(stderr, "Underrun!\n"); */
		snd_pcm_prepare(pcm_handle);
	} else if (pcmrc < 0) {
		/* fprintf(stderr, "Error writing to PCM device: %s\n",
		   snd_strerror(pcmrc)); */
	} else if (pcmrc != readcount) {
		/* fprintf(stderr,"PCM write differs from PCM read.\n");
		 */
	}
}

void play_sndfile(snd_pcm_t *pcm_handle)
{
	short readcount;
	short* buf;
	/* Allocate 2 in case of stereo */
	buf = malloc(frames * 2 * sizeof(short));
	while ((readcount = sf_readf_short(infile, buf, frames))>0) {
		sound(pcm_handle, buf, readcount);
	}
	sf_seek(infile,0,SEEK_SET);
	free(buf);
}

void play_wave(snd_pcm_t *pcm_handle, Wave* wav)
{
	int *buf, size, i;
	size = frames * wav->header.numChannels * (wav->header.bitsPerSample / 8);
	buf = malloc(size);
	for (i = 0;i < wav->size;i++) {
		memcpy(buf, wav->data + i, size);
		i += size;
		sound(pcm_handle, buf, size);
	}
	free(buf);
}
