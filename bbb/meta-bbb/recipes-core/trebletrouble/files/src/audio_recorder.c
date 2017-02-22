#include <alsa/asoundlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "audio_recorder.h"

/* added to fix implicit delcaration warnings for alloca in target files */
#ifndef alloca
#define alloca(x) __builtin_alloca(x)
#endif

int recordWAV(Wave* wave, uint32_t duration)
{
  int err, i, size, dir;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int sampleRate;
  int dir;
  WaveHeader* hdr;
  snd_pcm_uframes_t frames;
  const char *device; /* Integrated system microphone */
  char *buffer;
  
  hdr = &(wave->header);
  frames = 1024;
  device = "default";

  /* Open PCM device for recording (capture). */
  err = snd_pcm_open(&handle, device, SND_PCM_STREAM_CAPTURE, 0);
  if (err) {
      fprintf(stderr, "Unable to open PCM device: %s\n", snd_strerror(err));
      goto END;
  }

  /* Allocate a hardware parameteres object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* ### Set the desired hardware paramteteres. ### */

  /* Interleaved mode */
  err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
  if (err) {
    fprintf(stderr, "Error setting interleaved mode: %s\n", snd_strerror(err));
    goto END_PCM;
  }

  /* Signed 16-bit litte-endian format */
  if (hdr-> bitsPerSample == 16) {
    err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
  } else {
    err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_U8);
  }

  if (err) {
    fprintf(stderr, "Error setting format: %s\n", snd_strerror(err)); 
    goto END_PCM;
  }

  /* Two channels (stereo) */
  err = snd_pcm_hw_params_set_channels(handle, params, hdr -> numChannels);
  if (err) {
    fprintf(stderr, "Error setting channels: %s\n", snd_strerror(err));
    goto END_PCM;
  }

  /* 44100 bits/second sampling rate (CD quality) */
  sampleRate = hdr->sampleRate;
  err = snd_pcm_hw_params_set_rate_near(handle, params, &sampleRate, &dir);
  if (err) {
    fprintf(stderr, "Error setting sampling rate (%d): %s\n", sampleRate, snd_strerror(err));
    goto END_PCM;
  }

  /* Set period size */
  err = snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
  if (err) {
    fprintf(stderr, "Error setting period size: %s\n", snd_strerror(err));
    goto END_PCM;
  }

  /* Write the parameters to the driver */
  err = snd_pcm_hw_params(handle, params);
  if (err < 0) {
    fprintf(stderr, "Unable to set HW parameters: %s\n", snd_strerror(err));
    goto END_PCM;
  }

  /* Use a buffer large enough to hold one period */
  err = snd_pcm_hw_params_get_period_size(params, &frames, &dir);
  if (err){
    fprintf(stderr, "Error retrieving period size: %s\n", snd_strerror(err));
    goto END_PCM;
  }

  size = frames * hdr -> numChannels * (hdr -> bitsPerSample / 8);
  buffer = malloc(size);
  if (!buffer) {
    fprintf(stdout, "Buffer error.\n");
    err = -1;
    goto END_PCM;
  }

  err = snd_pcm_hw_params_get_period_time(params, &sampleRate, &dir);
  if (err) {
    fprintf(stderr, "Error retrieving period time: %s\n", snd_strerror(err));
    goto END_BUF;
  }

  for (i = ((duration * 1000) / (hdr->sampleRate/ frames)); i > 0; i--) {
    err = snd_pcm_readi(handle, buffer, frames);
    if (err >= 0) {
      err *= hdr -> numChannels * (hdr -> bitsPerSample / 8);
      memcpy(wave->data + wave->index, buffer, err);
      wave->index += err;
      continue;
    }
    if (err == -EPIPE) {
      fprintf(stderr, "Overrun occurred: %d\n", err);
    }
    if (err < 0) {
      snd_pcm_recover(handle, err, 0);
    }
    /* Still an error, need to exit. */
    if (err < 0) {
      fprintf(stderr, "Error occurred while recording: %s\n", snd_strerror(err));
      goto END;
    }
  } /* end for loop */

  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);
  return 0;

  /* clean up */
 END_BUF:
  free(buffer);
 END_PCM:
  snd_pcm_close(handle);
 END:
  return err;
}
