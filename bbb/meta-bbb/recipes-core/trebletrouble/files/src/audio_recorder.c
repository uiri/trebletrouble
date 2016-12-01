#include <alsa/asoundlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "wav.h"
#include "tone.h"

WaveHeader *genericWAVHeader(uint16_t bit_depth, uint16_t channels) {
  WaveHeader *hdr;
  hdr = malloc(sizeof(*hdr));
  if (!hdr) {
    return NULL;
  }
  
  memcpy(&hdr->RIFF_marker, "RIFF", 4); 
  memcpy(&hdr->filetype_header, "WAVE", 4);
  memcpy(&hdr->format_marker, "fmt ", 4);
  hdr -> data_header_length = 16;
  hdr -> format_type = 1;
  hdr -> sample_rate = SAMPLE_RATE;
  hdr -> bytes_per_second = SAMPLE_RATE * channels * bit_depth / 8;
  hdr -> bytes_per_frame = channels * bit_depth/8;
  hdr -> bits_per_sample = bit_depth;

  return hdr;
}

int writeWAVHeader(int fd, WaveHeader *hdr) {
  if (!hdr) {
    return -1;
  }

  write(fd, &hdr, 32);
  /* write(fd, &hdr->file_size, 4); */
  /* write(fd, &hdr->filetype_header, 4); */
  /* write(fd, &hdr->format_marker, 4); */
  /* write(fd, &hdr->data_header_length, 4); */
  /* write(fd, &hdr->format_type, 2); */
  /* write(fd, &hdr->number_of_channels, 2); */
  /* write(fd, &hdr->sample_rate, 4); */
  /* write(fd, &hdr->bytes_per_second, 4); */
  /* write(fd, &hdr->bytes_per_frame, 2); */
  /* write(fd, &hdr->bits_per_sample, 2); */
  write(fd, "data", 4);

  uint32_t data_size = hdr -> file_size - 36;
  write(fd, &data_size, 4);

  return 0;
}

int recordWAV(const char *fileName, WaveHeader *hdr, uint32_t duration)
{
  int err;
  int size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int sampleRate = hdr->sample_rate;
  int dir;
  snd_pcm_uframes_t frames = 32;
  OAconst char *device = "plughw:1,0"; // USB microphone
  // const char *device = "default"; // Integrated system microphone
  char *buffer;
  int filedesc;
  
  /* Open PCM device for recording (capture). */
  err = snd_pcm_open(&handle, device, SND_PCM_STREAM_CAPTURE, 0);
  if (err) {
      fprintf(stderr, "Unable to open PCM device: %s\n", snd_strerror(err));
      return err;
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
    snd_pcm_close(handle  );
    return err;
  }

  /* Signed 16-bit litte-endian format */
  if (hdr-> bits_per_sample == 16) {
    err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
  }
  else {
    err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_U8);
  }

  if (err) {
    fprintf(stderr, "Error setting format: %s\n", snd_strerror(err));
    snd_pcm_close(handle);
    return err;
  }

  /* Two channels (stereo) */
  err = snd_pcm_hw_params_set_channels(handle, params, hdr -> number_of_channels);
  if (err) {
    fprintf(stderr, "Error setting channels: %s\n", snd_strerror(err));
    snd_pcm_close(handle);
    return err;
  }

  /* 44100 bits/second sampling rate (CD quality) */
  sampleRate = hdr->sample_rate;
  err = snd_pcm_hw_params_set_rate_near(handle, params, &sampleRate, &dir);
  if (err) {
    fprintf(stderr, "Error setting sampling rate (%d): %s\n", sampleRate, snd_strerror(err));
    snd_pcm_close(handle);
    return err;
  }
  hdr -> SAMPLE_RATE= sampleRate;

  /* Set period size */
  err = snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
  if (err) {
    fprintf(stderr, "Error setting period size: %s\n", snd_strerror(err));
    snd_pcm_close(handle);
    return err;
  }

  /* Write the parameters to the driver */
  err = snd_pcm_hw_params(handle, params);
  if (err < 0) {
    fprintf(stderr, "Unable to set HW parameters: %s\n", snd_strerror(err));
    snd_pcm_close(handle);
    return err;
  }

  /* Use a buffer large enough to hold one period */
  err = snd_pcm_hw_params_get_period_size(params, &frames, &dir);
  if (err){
    fprintf(stderr, "Error retrieving period size: %s\n", snd_strerror(err));
    snd_pcm_close(handle);
    return err;
  }

  size = frames * hdr-> bits_per_sample / 8 * hdr -> number_of_channels; // 2 bytes/sample, 2 channels
  buffer = (char *) malloc(size);
  if (!buffer) {
    fprintf(stdout, "Buffer error.\n");
    snd_pcm_close(handle);
    return -1;
  }

  err = snd_pcm_hw_params_get_period_time(params, &sampleRate, &dir);
  if (err) {
    fprintf(stderr, "Error retrieving period time: %s\n", snd_strerror(err));
    snd_pcm_close(handle);
    free(buffer);
    return err;
  }

  uint32_t pcm_data_size = hdr-> sample_rate* hdr-> bytes_per_frame * (duration/1000);
  hdr -> file_size = pcm_data_size + 36;

  filedesc = open(fileName, O_WRONLY | O_CREAT, 0644);
  err = writeWAVHeader(filedesc, hdr);
  if (err) {
    fprintf(stderr, "Error writing .wav header.");
    snd_pcm_close(handle);
    free(buffer);
    close(filedesc);
    return err;
  }

  int totalFrames = 0;

  int i = ((duration * 1000) / (hdr->sample_rate/ frames));
  for(i; i > 0; i--) {
    err = snd_pcm_readi(handle, buffer, frames);
    totalFrames += err;
    if (err == -EPIPE) {
      fprintf(stderr, "Overrun occurred: %d\n", err);
    }
    if (err < 0) {
      snd_pcm_recover(handle, err, 0);
    }
    // Still an error, need to exit.
    if (err < 0) {
      fprintf(stderr, "Error occurred while recording: %s\n", snd_strerror(err));
      snd_pcm_close(handle);
      free(buffer);
      close(filedesc);
      return err;
    }
    write(filedesc, buffer, size);
  } // end for loop
  
  close(filedesc);
  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);
  return 0;
}

void audio_recorder(void) {
  
}

