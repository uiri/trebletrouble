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
#include <math.h>


#define PCM_DEVICE "default"
//WAV code
static snd_pcm_uframes_t frames;
static SNDFILE *infile = NULL;	


snd_pcm_t * init_pcm(char *infilename,short **buf){
        
	SF_INFO sfinfo;
	snd_pcm_hw_params_t *params;
	snd_pcm_t *pcm_handle;
	int dir;
	unsigned int tmp,pcm;
	dir = 0;
	if ( access( infilename, F_OK) ==-1){
		printf("File doesn't exists");	
	}
	else{
		//	printf("File does exist!");
	}
	infile = sf_open(infilename, SFM_READ, &sfinfo);
        /* Open the PCM device in playback mode */
	if ((pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
				SND_PCM_STREAM_PLAYBACK, 0)) < 0) 
	  	printf("ERROR: Can't open \"%s\" PCM device. %s\n",
					PCM_DEVICE, snd_strerror(pcm));
       	/* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(pcm_handle, params);
	/* Set parameters */
	if ((pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
						SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) 
		printf("ERROR: Can't set noninterleaved mode. %s\n", snd_strerror(pcm));
	//set formatting 
	if ((pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
					       SND_PCM_FORMAT_S16_LE)) < 0) 
		printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));
	//Setting channels
	if ((pcm = snd_pcm_hw_params_set_channels(pcm_handle, params,1)) < 0) 
		printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));
	//Set sample rate
	if ((pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, (unsigned int*)&sfinfo.samplerate, &dir)) < 0) 
		printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));
	
		

	/* Resume information */
       	snd_pcm_hw_params_get_channels(params, &tmp);
	if (tmp == 1)
		printf("(mono)\n");
	else if (tmp == 2)
		printf("(stereo)\n");
	snd_pcm_hw_params_get_rate(params, &tmp, 0);
       	/* Write parameters */
	snd_pcm_hw_params(pcm_handle, params);
	/* Allocate buffer to hold single period */
	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	*buf = malloc(frames * sfinfo.channels * sizeof(short));
	return pcm_handle;
}
void cleanup_pcm(snd_pcm_t *pcm_handle,short *buf){
    //Drain the buffer
    snd_pcm_drain(pcm_handle);
    //Close the device
    snd_pcm_close(pcm_handle);
    free(buf);
    

}



void sound (snd_pcm_t *pcm_handle,short *buf){

	int pcmrc;
	short readcount;
    while ((readcount = sf_readf_short(infile, buf, frames))>0) {
	
        pcmrc = snd_pcm_writei(pcm_handle, buf, frames);
        if (pcmrc == -EPIPE) {
		//    fprintf(stderr, "Underrun!\n");
            snd_pcm_prepare(pcm_handle);
	        }
        else if (pcmrc < 0) {
		//            fprintf(stderr, "Error writing to PCM device: %s\n", snd_strerror(pcmrc));
        }
        else if (pcmrc != readcount) {
		//    fprintf(stderr,"PCM write differs from PCM read.\n");
        }

    }
    sf_seek(infile,0,SEEK_SET);
    }
