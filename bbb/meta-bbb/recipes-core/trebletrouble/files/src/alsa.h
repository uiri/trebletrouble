#include <alsa/asoundlib.h>
#include "tone.h"

#ifndef SIMPLE_ALSA_H_
#define SIMPLE_ALSA_H_

void cleanup_pcm(snd_pcm_t *pcm_handle);
unsigned int init_sndfile(char *infilename);
snd_pcm_t * init_pcm(unsigned int samplerate);
void init_pcm_params(snd_pcm_t *pcm_handle, snd_pcm_hw_params_t *params,
		     unsigned int samplerate, int *dir, int channels);
void play_sndfile(snd_pcm_t *pcm_handle);
void play_wave(snd_pcm_t *pcm_handle, Wave* wav);
void sound(snd_pcm_t *pcm_handle, short *buf, int readcount);
int recordWAV(Wave* wave, float duration);

#endif
