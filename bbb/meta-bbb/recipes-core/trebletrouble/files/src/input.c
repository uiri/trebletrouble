/*
 *  Copyright (C) 2016-2017  William Pearson
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

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <linux/input.h>

#include "input.h"
#include "display.h"
#include "colours.h"

#define EV_ABS_SIZE 5

/* Where does this value come from? Manual testing. Lots and lots of
 * manual testing. */
#define TS_MAX 3930

int cleanup_touchscreen(int *fd) {
	return close(*fd);
}

void get_lcd_input(ScreenInput *st) {
	struct input_event ev[64];
	size_t rb;
	int i;
	unsigned char touch;

	rb = read(st->fd, ev, sizeof(ev));
	rb /= sizeof(struct input_event);
	touch = '\0';
	st->x = 0;
	st->y = 0;
	st->pressure = 0;
	for (i = 0; i < rb; i++) {

		if (ev[i].type == EV_KEY) {
			/* Touch started. */
			if (ev[i].value)
				touch++;
			/* else touch ended */
			else if (touch)
				/* previously recorded touch started */
				break;
		}

		if (ev[i].type != EV_ABS)
			continue;

		switch (ev[i].code) {
		case 0:
			if (ev[i].value)
				st->x = ev[i].value * 800 / TS_MAX;
			break;
		case 1:
			if (ev[i].value)
				st->y = ev[i].value * 480 / TS_MAX;
			break;
		case 24:
			if (ev[i].value)
				st->pressure = ev[i].value;
			break;
		default:
			break;
		}
	}
}

int init_touchscreen(int *fd) {
	unsigned long bit[EV_MAX][KEY_MAX / (8 * sizeof(unsigned long))];

	if ((*fd = open("/dev/input/touchscreen0", O_RDONLY)) < 0) {
		return -1;
	}

	memset(bit, 0, sizeof(bit));
	ioctl(*fd, EVIOCGBIT(0, EV_MAX), bit[0]);

	return 0;
}
