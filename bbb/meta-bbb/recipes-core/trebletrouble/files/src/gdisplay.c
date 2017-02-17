/*
 *  Copyright (C) 2017  William Pearson
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permission under GNU GPL version 3 section 7
 *
 *  If you modify this Program, or any covered work, by linking or
 *  combining it with the uGFX Library, containing parts covered by the
 *  terms of the GFX License, Version 1.2, the licensors of this Program
 *  grant you additional permission to convey the resulting work.
 *  Corresponding Source for a non-source form of such a combination
 *  shall include the source code for the parts of the uGFX Library used
 *  as well as that of the covered work.
 *
 */

/* This file implements the framebuffer driver for our board */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include <unistd.h>

#include <linux/fb.h>
#include <sys/mman.h>

static int fbfd;

int DISP_WIDTH = 1;
long SCREENSIZE;

char *init_display() {
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	char* fbp;

	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1) {
		printf("Error: cannot open framebuffer.\n");
		return NULL;
	}

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable screen info\n");
	}

	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed screen info\n");
	}

	DISP_WIDTH = vinfo.xres;
	SCREENSIZE = finfo.smem_len;

	fbp = (char*)mmap(0, SCREENSIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
			  fbfd, 0);
	if ((long)fbp == -1) {
		printf("Mmap of screen failed.\n");
		return NULL;
	}

	/* memset(fbp, 0x00, SCREENSIZE); */
	return fbp;
}

void cleanup_display(char* fbp) {
	if (fbp)
		munmap(fbp, SCREENSIZE);
	close(fbfd);
}
