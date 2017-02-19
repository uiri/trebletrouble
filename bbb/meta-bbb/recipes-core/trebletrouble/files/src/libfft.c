/* libfft.c - fast Fourier transform library
**
** Copyright (C) 1989 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include "libfft.h"

#define MAXFFTSIZE 32768
#define LOG2_MAXFFTSIZE 15

static int bitreverse[MAXFFTSIZE], bits;

/* initfft - initialize for fast Fourier transform
**
** b    power of two such that 2**nu = number of samples
*/
int initfft( int b )
    {
    int i, j, k;

    bits = b;
    if ( bits > LOG2_MAXFFTSIZE )
	{
	fprintf(
	    stderr, "%d is too many bits, max is %d\n", bits, LOG2_MAXFFTSIZE );
	return -1;
	}

    for ( i = ( 1 << bits ) - 1; i >= 0; --i )
	{
	k = 0;
	for ( j = 0; j < bits; ++j )
	    {
	    k *= 2;
	    if ( i & ( 1 << j ) )
		k += 1;
	    }
	bitreverse[i] = k;
	}
    return 0;
    }

/* fft - a fast Fourier transform routine
**
** xr   real part of data to be transformed
** xi   imaginary part (normally zero, unless inverse transform in effect)
** inv  flag for inverse
*/

void fft(float xr[], float xi[], int inv)
    {
    int n, n2, i, k, kn2, l, p;
    double ang;
    float s, c, tr, ti;
    double ds, dc;

    n = 1 << bits;
    n2 = n / 2;

    for ( l = 0; l < bits; ++l )
	{
	for ( k = 0; k < n; k += n2 )
	    {
	    for( i = 0; i < n2; ++i, ++k )
		{
		p = bitreverse[k / n2];
		ang = 6.283185 * p / n;
#ifdef notdef
		c = cos( ang );
		s = sin( ang );
#else /* notdef */
		sincos( ang, &ds, &dc );
		s = ds;
		c = dc;
#endif /* notdef */
		kn2 = k + n2;
		if ( inv )
		    s = -s;
		tr = xr[kn2] * c + xi[kn2] * s;
		ti = xi[kn2] * c - xr[kn2] * s;
		xr[kn2] = xr[k] - tr;
		xi[kn2] = xi[k] - ti;
		xr[k] += tr;
		xi[k] += ti;
		}
	    }
	n2 /= 2;
	}

    for ( k = 0; k < n; ++k )
	{
	i = bitreverse[k];
	if ( i <= k )
	    continue;
	tr = xr[k];
	ti = xi[k];
	xr[k] = xr[i];
	xi[k] = xi[i];
	xr[i] = tr;
	xi[i] = ti;
	}

    /* Finally, multiply each value by 1/n, if this is the forward transform. */
    if ( ! inv )
	{
	float f;

	f = 1.0 / n;
	for( i = 0; i < n ; ++i )
	    {
	    xr[i] *= f;
	    xi[i] *= f;
	    }
	}
    }
