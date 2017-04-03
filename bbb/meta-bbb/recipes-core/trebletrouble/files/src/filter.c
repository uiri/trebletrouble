#include <math.h>
#include <stdio.h>

#define AVG_NUM 10

void filter_wavdata(float* output, int out_size, short* input, int in_size, int max)
{
	short* input_start, *izero, i_sample;
	int i;
	float f_sample, *ozero, *output_end;

	ozero = output;
	output_end = output + out_size;
	input_start = input;
	izero = input_start + AVG_NUM + ((out_size - (in_size - AVG_NUM)) / 2);

	for (input = izero; output < output_end; input++, output++) {
		i_sample = 0;
		for (i = 0; i < AVG_NUM; i++) {
			i_sample += input[-i];
		}
		f_sample = (float)(i_sample) / AVG_NUM;
		*output = f_sample / max;
	}
	printf("Wrote %ld of %d. Read %ld of %d.\n", output - ozero, out_size, input - izero, in_size);
}
