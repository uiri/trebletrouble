#include <alsa/asoundlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "audio_recorder.h"
#include "tone.h"

/* added to fix implicit delcaration warnings for alloca in target files */
#ifndef alloca
#define alloca(x) __builtin_alloca(x)
#endif

void genericWAVHeader(WaveHeader *hdr, uint16_t bit_depth, uint16_t channels) {
  if (!hdr) {
    return;
  }

  memcpy(&hdr->chunkId, "WAVE", 4);
  hdr -> chunkSize = 16;
  memcpy(&hdr->format, "fmt ", 4);

  hdr -> audioFormat = 1;
  hdr -> numChannels = 1;
  hdr -> sampleRate = SAMPLE_RATE;
  hdr -> byteRate = SAMPLE_RATE * channels * bit_depth / 8;
  hdr -> blockAlign = channels * bit_depth / 8;
  hdr -> bitsPerSample = bit_depth;

  memcpy(&hdr->subChunk1Id, "DATA", 4);
  hdr -> subChunk1Size = 32;
  memcpy(&hdr->subChunk2Id, "DATA", 4);
  hdr -> subChunk2Size = 32;

}

int writeWAVHeader(FILE* file, WaveHeader *hdr) {
  if (!hdr) {
    return -1;
  }

  fwrite(&hdr, sizeof(WaveHeader), 1, file);
  fwrite("data", 4, 1, file);

  size_t data_size = hdr -> chunkSize - 36;
  fwrite(&hdr, data_size, 1, file);

  return 0;
}

int recordWAV(Wave* wave, WaveHeader *hdr, uint32_t duration)
{
  int err;
  int size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int sampleRate = hdr->sampleRate;
  int dir;
  snd_pcm_uframes_t frames = 32;
  const char *device = "default"; /* Integrated system microphone */
  float *buffer;
  int channel = 1;
  
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
  hdr -> sampleRate = SAMPLE_RATE;

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

  size = frames * hdr-> bitsPerSample / 8 * hdr -> numChannels; /* 2 bytes/sample, 2 channels */
  buffer = (float *) malloc(size);
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

  uint32_t pcm_data_size = hdr-> sampleRate* hdr-> blockAlign * (duration/1000);
  hdr -> chunkSize = pcm_data_size + 36;

  int totalFrames = 0;
  int j;
  int i = ((duration * 1000) / (hdr->sampleRate/ frames));
  for(; i > 0; i--) {
    err = snd_pcm_readi(handle, buffer, frames);
    totalFrames += err;
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
    /* So the buffer size is 128 Bytes. This places 4 bytes of the buffer  */
    for (j = 0; j < size/sizeof(float); i++) {
      waveAddSample(wave,buffer,channel);
    }
    /* This should be appending data from the buffer into wave->data */
    /* fwrite(buffer, size, 1, filedesc); */
    
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

void audio_recorder(Wave* wave, WaveHeader* hdr, uint32_t duration) {
  int err;
  uint16_t bit_depth = 16;
  uint16_t channels = 1;
  
  /* WRITE DATA TO WAVE */  
  tone(wave, duration); /* Creates space for wave and sets the duration */
  genericWAVHeader(hdr, bit_depth, channels);
  err = recordWAV(wave, hdr, duration);
  if (err) {
    printf("Error in audio recording.");
  }

}
