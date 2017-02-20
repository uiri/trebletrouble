void cleanup_pcm(snd_pcm_t **pcm_handle);
unsigned int init_sndfile(char *infilename);
snd_pcm_t * init_pcm(unsigned int samplerate);
void play_sndfile(snd_pcm_t *pcm_handle);
void sound(snd_pcm_t *pcm_handle, short *buf, int readcount);
