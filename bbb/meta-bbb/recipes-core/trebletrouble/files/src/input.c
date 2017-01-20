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
