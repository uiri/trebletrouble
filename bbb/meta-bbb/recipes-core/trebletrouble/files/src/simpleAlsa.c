/*
 * Simple sound playback using ALSA API and libasound.
 *
 * Compile:
 * $ cc -o play sound_playback.c -lasound
 * 
 * Usage:
 * $ ./play <sample_rate> <channels> <seconds> < <file>
 * 
 * Examples:
 * $ ./play 44100 2 5 < /dev/urandom
 * $ ./play 22050 1 8 < /path/to/file.wav
 *
 * Copyright (C) 2009 Alessandro Ghedini <alessandro@ghedini.me>
 * --------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Alessandro Ghedini wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we
 * meet some day, and you think this stuff is worth it, you can
 * buy me a beer in return.
 * --------------------------------------------------------------
 */

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <sndfile.h>

#define PCM_DEVICE "default"

int sound (void){

  unsigned int pcm, tmp;
	int rate, channels, seconds;
	
	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;
	snd_async_handler_t *pcm_callback;
	int dir , pcmrc;
	
	char *infilename = "/srv/trebletrouble/metro_1.wav";
	short* buf =NULL;
	short readcount;
	
	if ( access( infilename, F_OK) !=-1){
	  printf("File exists");	
	}
	else{
	  printf("File doesn't exist");
	}
	SF_INFO sfinfo;
  	SNDFILE *infile = NULL;
	
	infile = sf_open(infilename, SFM_READ, &sfinfo);
        fprintf(stderr,"Channels: %d\n", sfinfo.channels);
        fprintf(stderr,"Sample rate: %d\n", sfinfo.samplerate);
        fprintf(stderr,"Sections: %d\n", sfinfo.sections);
	        fprintf(stderr,"Format: %d\n", sfinfo.format);

		rate = 44100;
	channels = 1;
	seconds = 0.5;

	/* Open the PCM device in playback mode */
	if (pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
					SND_PCM_STREAM_PLAYBACK, 0) < 0) 
	  	printf("ERROR: Can't open \"%s\" PCM device. %s\n",
					PCM_DEVICE, snd_strerror(pcm));
					
	/* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(pcm_handle, params);
	/* Set parameters */
	if (pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
					SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
		printf("ERROR: Can't set noninterleaved mode. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
						SND_PCM_FORMAT_S16_LE) < 0) 
		printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));
	//Setting channels
	if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, params,sfinfo.channels) < 0) 
		printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &sfinfo.samplerate, 0) < 0) 
		printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));
	
		

	/* Resume information */
	printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));
	printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));
	snd_pcm_hw_params_get_channels(params, &tmp);
	printf("channels: %i ", tmp);

	if (tmp == 1)
		printf("(mono)\n");
	else if (tmp == 2)
		printf("(stereo)\n");

	snd_pcm_hw_params_get_rate(params, &tmp, 0);
	printf("rate: %d bps\n", tmp);

	//printf("seconds: %d\n", seconds);	

	 /* Write parameters */
    snd_pcm_hw_params(pcm_handle, params);

    /* Allocate buffer to hold single period */
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    fprintf(stderr,"# frames in a period: %d\n", frames);

    fprintf(stderr,"Starting read/write loop\n");
    buf = malloc(frames * sfinfo.channels * sizeof(short));
    while ((readcount = sf_readf_short(infile, buf, frames))>0) {
      printf("My readcount is:%d",readcount);
        pcmrc = snd_pcm_writei(pcm_handle, buf, frames);
        if (pcmrc == -EPIPE) {
            fprintf(stderr, "Underrun!\n");
            snd_pcm_prepare(pcm_handle);
	    //frames = snd_pcm_writei(pcm.handle,buf, )
        }
        else if (pcmrc < 0) {
            fprintf(stderr, "Error writing to PCM device: %s\n", snd_strerror(pcmrc));
        }
        else if (pcmrc != readcount) {
            fprintf(stderr,"PCM write differs from PCM read.\n");
        }

    }
    fprintf(stderr,"End read/write loop\n");

    //Drain the buffer
    snd_pcm_drain(pcm_handle);
    //Close the device
    snd_pcm_close(pcm_handle);
    free(buf);

    return 0;

				
}
