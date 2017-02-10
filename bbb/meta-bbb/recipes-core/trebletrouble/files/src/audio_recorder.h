#include <stdint.h>
#include <stdlib.h>
#include "tone.h"

void audio_recorder(Wave* wave, WaveHeader* hdr, uint32_t duration);
void genericWAVHeader(WaveHeader *hdr, uint16_t bit_depth, uint16_t channels);
/*WaveHeader *retrieveWAVHeader(const void *ptr);*/
int writeWaveHeader(int fd, WaveHeader *hdr);
int recordWAV(Wave* wave, WaveHeader *hdr, uint32_t duration);

