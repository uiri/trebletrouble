#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include <unistd.h>

#include <linux/fb.h>
#include <sys/mman.h>

#include "colours.h"
#include "display.h"

int DISP_WIDTH = 1;
long SCREENSIZE;

#define KEYS 88

double FREQS[KEYS] = {27.5, 29.1352, 30.8677, 32.7032, 34.6478, 36.7081, 38.8909, 41.2034, 43.6535, 46.2493, 48.9994, 51.9131, 55.0, 58.2705, 61.7354, 65.4064, 69.2957, 73.4162, 77.7817, 82.4069, 87.3071, 92.4986, 97.9989, 103.826, 110.0, 116.541, 123.471, 130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 207.652, 220.0, 233.082, 246.942, 261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440.0, 466.164, 493.883, 523.251, 554.365, 587.330, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880.0, 932.328, 987.767, 1046.50, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760.0, 1864.66, 1975.53, 2093.0, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520.0, 3729.31, 3951.07, 4186.01};

char *NOTE[12] = {"/srv/trebletrouble/A.pnm", "/srv/trebletrouble/As.pnm", "/srv/trebletrouble/B.pnm", "/srv/trebletrouble/C.pnm", "/srv/trebletrouble/Cs.pnm", "/srv/trebletrouble/D.pnm", "/srv/trebletrouble/Ds.pnm", "/srv/trebletrouble/E.pnm", "/srv/trebletrouble/F.pnm", "/srv/trebletrouble/Fs.pnm", "/srv/trebletrouble/G.pnm", "/srv/trebletrouble/Gs.pnm"};

char *OCTAVE[9] = {"/srv/trebletrouble/0.pnm", "/srv/trebletrouble/1.pnm", "/srv/trebletrouble/2.pnm", "/srv/trebletrouble/3.pnm", "/srv/trebletrouble/4.pnm", "/srv/trebletrouble/5.pnm", "/srv/trebletrouble/6.pnm", "/srv/trebletrouble/7.pnm", "/srv/trebletrouble/8.pnm"};

/* Internal functions */

void colour_screen(char* fbp, short colour) {
	int i;

	for (i = 0; i < SCREENSIZE/2; i++) {
		/*
		 * So, we have 16-bits per pixel. It is split up into 
		 * 5 bits for Red, 6 bits for Green and 5 bits for Blue
		 * Why 6 bits for Green? Human biology, that's why.
		 * The least significant _byte_ is first and the most 
		 * significant byte is second. Byte order is always the same.
		 * 0x25E2 is actually the colour E225 which is orange.
		 * Red - 1110 0; Green - 010 001; Blue - 0 0101)
		 */
		memset(fbp + 2*i, colour, 2);
	}
}

int find_freq_recur(double freq, int i, int len) {
	while (1) {
		if (FREQS[i] <= freq) {
			if (freq <= FREQS[i+1]) {
				if ((freq - FREQS[i]) < (FREQS[i+1] - freq))
					return i;
				else
					return i+1;
			} else
				i += (len%2 == 0 ? len/4 : len/4 + 1);
		} else
			i -= (len%2 == 0 ? len/4 : len/4 + 1);
		len /= 2;
	}
}

int get_xnote(int i) {
	/* First note is at 145,
	   each subsequent note is 34 px further */
	return 145 + i*34;
}

void draw_note(int i, int ynote, char* fbp, short colour) {
	bitblit_colour(NOTE_PIC, fbp, get_xnote(i), ynote-15, colour);
}

/* External API */
int find_freq(double freq) {
	if (freq <= FREQS[0])
		return 0;
	if (FREQS[KEYS-1] <= freq)
		return KEYS-1;
	return find_freq_recur(freq, KEYS / 2, KEYS);
}

int find_ind(char note, int oct){
	int ind, v;

	ind = (oct*12);

	v = ((note - 'C'+7)%7)*2;

	if (v > 4)
		v--;

	return ind+v-9;
}

int bitblit_colour(char* filename, char* fbp, int x, int y, short colour) {
	FILE* fd;
	int i, j, w, h, vw;
	unsigned char buf[3], px[2];

	fd = fopen(filename, "rb");
	if (fd == NULL)
		return -1;
	fread(buf, 1, 3, fd);
	if (buf[0] != 'P' || buf[1] != '6') {
		fclose(fd);
		return -2;
	}
	fread(buf, 1, 3, fd);
	w = 0;
	for (i = 0; i < 3; i++) {
		if (buf[i] == ' ' || (9 < buf[i] && buf[i] < 13))
			break;
		w *= 10;
		w += buf[i] - '0';
	}
	w--;
	j = 0;
	if (i == 3) {
		/* throw away whitespace */
		fread(buf, 1, 1, fd);
	} else if (i < 2) {
		for (; j+i+1 < 3; j++) {
			buf[j] = buf[j+i+1];
		}
	}
	fread(buf+j, 1, 3-j, fd);
	h = 0;
	for (i = 0; i < 3; i++) {
		if (buf[i] == ' ' || (9 < buf[i] && buf[i] < 13))
			break;
		h *= 10;
		h += buf[i] - '0';
	}
	h--;
	if (i == 3) {
		buf[0] = fgetc(fd);
	}
	do {
		buf[0] = fgetc(fd);
	} while (buf[0] != ' ' && (buf[0] < 9 || 13 < buf[0]));

	vw = (x+w) > 800 ? x+w-799 : 0;

	for (i = 0; i < SCREENSIZE/2; i++) {
		if ((i / DISP_WIDTH) < y)
			continue;
		if (vw && (i % DISP_WIDTH) == DISP_WIDTH-1)
			fseek(fd, 3*vw, SEEK_CUR);
		if ((i % DISP_WIDTH) < x || (x+w) < (i % DISP_WIDTH))
			continue;
		if ((y+h) < (i / DISP_WIDTH))
			break;
		if (fread(buf, 1, 3, fd) != 3) {
			break;
		}
		if (buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0xFF)
			continue;
		if (buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0x00) {
			memset(fbp + 2*i, colour, 2);
			continue;
		}
		px[0] = (buf[0] & 0xF8) | ((buf[1] & 0xE0) >> 5);
		px[1] = ((buf[1] & 0x1C) << 3) | ((buf[2] & 0xF8) >> 3);
		memset(fbp + 2*i, *(short*)px, 2);
	}

	fclose(fd);
	return 0;
}

int bitblit(char* filename, char* fbp, int x, int y) {
	return bitblit_colour(filename, fbp, x, y, BLACK);
}

void draw_staff(char* fbp){
	/* place staff (five lines) on screen */
	int i;
	int x = 0;
	int y = 90;
	/* m is the space between the lines */
	int m = 30;
	int start =0;
	int end = 800;
	/* draw clef and end lines */
	bitblit("/srv/trebletrouble/clef.pnm", fbp, start, y-35);
	bitblit("/srv/trebletrouble/end.pnm", fbp, end-15, y);
	bitblit("/srv/trebletrouble/end.pnm", fbp, end-5, y);

	/* draw the lines of the staff */
	for(i=0; i < 5; i++, y+=m)
		bitblit("/srv/trebletrouble/line.pnm", fbp, x, y);
}

void compare_notes(int expected, int actual, int i, char* fbp) {
	int ynote, j;

	/* Does this work? idklol */
	actual += 9;
	j = actual%12;
	if (j > 4)
		j++;
	j /= 2;
	ynote = 240 - (15*j + 105*(actual/12 - 4));
	actual -= 9;

	if (actual == expected)
		draw_note(i, ynote, fbp, GREEN);
	else
		draw_note(i, ynote, fbp, RED);
}

void load_song(FILE *song, char *fbp){
	/*TODO: return expected notes*/
	unsigned char buf[2];
	int i, ynote;

	for (i = 0; i < 15 && fread(buf, 1, 2, song) == 2; i++) {
		ynote =(240 - (((buf[1]-'4') * 105) + (((buf[0]-'C'+7)%7)*15)));
		draw_note(i, ynote, fbp, BLACK);
	}
}

char *init_display(int *fbfd) {
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	char* fbp;

	*fbfd = open("/dev/fb0", O_RDWR);
	if (*fbfd == -1) {
		printf("Error: cannot open framebuffer.\n");
		return NULL;
	}

	if (ioctl(*fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable screen info\n");
	}

	if (ioctl(*fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed screen info\n");
	}

	DISP_WIDTH = vinfo.xres;
	SCREENSIZE = finfo.smem_len;

	fbp = (char*)mmap(0, SCREENSIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
			  *fbfd, 0);
	if ((long)fbp == -1) {
		printf("Mmap of screen failed.\n");
		return NULL;
	}

	memset(fbp, 0x00, SCREENSIZE);
	colour_screen(fbp, ORANGE);
	return fbp;
}

void cleanup_display(char* fbp, int *fbfd) {
	munmap(fbp, SCREENSIZE);
	close(*fbfd);
}
