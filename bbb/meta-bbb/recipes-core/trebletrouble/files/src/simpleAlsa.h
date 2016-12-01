void sound(snd_pcm_t *pcm_handle,short *buf);
snd_pcm_t* init_pcm(char *infilename,short **buf);
void cleanup_pcm(snd_pcm_t **pcm_handle,short **buf);
