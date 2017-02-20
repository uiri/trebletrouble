void cleanup_pcm(snd_pcm_t **pcm_handle,short **buf);
unsigned int init_sndfile(char *infilename, short **buf);
snd_pcm_t * init_pcm(unsigned int samplerate);
void sound(snd_pcm_t *pcm_handle,short *buf);
