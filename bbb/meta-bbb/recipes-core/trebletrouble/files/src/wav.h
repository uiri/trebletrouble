#include <stdint.h>

WaveHeader *genericWAVHeader(uint16_t bit_depth, uint16_t channels);
WaveHeader *retrieveWAVHeader(const void *ptr);
int writeWaveHeader(int fd, WaveHeader *hdr);
int recordWAV(const char *fileName, WaveHeader *hdr, uint32_t duration);
