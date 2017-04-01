/*
 *  Copyright (C) 2017       William Pearson
 *  Copyright (C) 2016-2017  Tejan Chinnakonda
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
#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#include "alsa.h"

#define REPEAT                 10  /* Do it 10 times */
#define DURATION               1   /* Record for DURATION sec each iteration */
#define PLAYBACK_DURATION      10  /* Record for # seconds in playback mode */
#define SND_FILE "/home/vagrant/trebletrouble/wave.wav" /* The sound file that will be recorded */

int main(void) {
  
  float pitch; /* pitch of the wave */
  uint32_t duration          = DURATION;
  uint32_t playback_duration = PLAYBACK_DURATION;
  int i                      = 0;
  char *infilename           = SND_FILE; 
  short *buf                 = NULL;
  snd_pcm_t *pcm_handle;
  FILE *fp;
  Wave* wave;

  printf("Recording for: %d second(s).\n\n\n", duration * REPEAT);




  printf("------------- TEST: Frequency output mode ------------------\n\n\n");


  wave = malloc( SAMPLE_RATE * duration * sizeof(float) );
  tone( wave, duration); /* Makes wave and sets duration */

  while( i < REPEAT) {
    audio_recorder(    wave, duration ); /* records audio */
    pitch = get_pitch( wave, duration ); /* provides provides the frequency of the wave */
    printf( "Frequency = %f\n", pitch );
    i++;
  }

  waveDestroy(wave);
  printf("\n\n\n");




  printf("------------- TEST: Audio Playback mode --------------------\n\n\n");
  

  /* Initialize pcm device and make wave */
  wave = malloc( SAMPLE_RATE * duration * sizeof(float) );
  tone( wave, duration ); /* Makes the wave and sets duration */
  fp = fopen( infilename, "w+" ); /* Opens a file for both read/write */
  printf("get channels: ");
  pcm_handle = init_pcm( infilename, &buf );
  printf("Initializing pcm device and making wave.\n");
  

  /* Record audio in playback mode */
  audio_recorder( wave, playback_duration );
  printf("Recording done...\n");

  
  /* Write to a file */
  fwrite( &(wave->header), sizeof(WaveHeader), 1, fp );
  fwrite( (void*)(wave->data), sizeof(char), wave->size, fp );
  fclose(fp);
  printf("writing to file done...\n");


  /* Play back sound */
  sound( pcm_handle, buf );
  fflush( stdout );
  cleanup_pcm( &pcm_handle, &buf );
  printf("playing back sound.\n");
  waveDestroy(wave);

  
  printf("\n\n\n");


  return 0;
  
}
