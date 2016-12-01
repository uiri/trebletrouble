#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>
#include <alsa/asoundlib.h>
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
	short *buf = NULL ;
	struct timeval tv;
	int dir;
	int64_t d, corr, slp, cur, next;
	tv = start;
	d = corr = 0;
	dir = 0;
	next = tv_to_u(start) + delay;
	pcm_handle=init_pcm(infilename,&buf);
	while (1) {
		gettimeofday(&tv, 0);
		slp = next - tv_to_u(tv) - corr;
		usleep(slp);
		gettimeofday(&tv, 0);
 		sound(pcm_handle,buf);
		fflush(stdout);
 		dir = !dir;
 		cur = tv_to_u(tv);
		d = cur - next;
		corr = (corr + d) / 2;
		next += delay;
 	}
	cleanup_pcm(&pcm_handle,&buf);
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
