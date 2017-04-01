#include <math.h>
#include <stdio.h>

void filter_wavdata(float* output, int out_size, short* input, int in_size, int max)
{
	short* input_start, *izero;
	float f_sample, *ozero, *output_end;

	ozero = output;
	output_end = output + out_size;
	input_start = input;
	izero = input_start + ((out_size - in_size) / 2);

	for (input = izero; output < output_end; input++, output++) {
		f_sample = (float)(*input);
		*output = f_sample / max;
	}
	printf("Wrote %ld of %d. Read %ld of %d.\n", output - ozero, out_size, input - izero, in_size);
}
