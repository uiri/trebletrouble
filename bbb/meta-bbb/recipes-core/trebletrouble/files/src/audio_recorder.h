#include <stdint.h>
#include "tone.h"

void audio_recorder(void);
void genericWAVHeader(WaveHeader *hdr, uint16_t bit_depth, uint16_t channels);
/*WaveHeader *retrieveWAVHeader(const void *ptr);*/
int writeWaveHeader(int fd, WaveHeader *hdr);
int recordWAV(const char *fileName, WaveHeader *hdr, uint32_t duration);

