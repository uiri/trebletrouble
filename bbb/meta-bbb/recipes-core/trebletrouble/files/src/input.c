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

ScreenInput get_lcd_input(int *fd, ScreenBounds *sb) {
	struct input_event ev[64];
	size_t rb;
	int i;
	ScreenInput st;
	unsigned char touch;

	rb = read(*fd, ev, sizeof(ev));
	rb /= sizeof(struct input_event);
	touch = '\0';
	st.x = 0;
	st.y = 0;
	st.pressure = 0;
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
				st.x = ev[i].value * 800 / TS_MAX;
			break;
		case 1:
			if (ev[i].value)
				st.y = ev[i].value * 480 / TS_YMAX;
			break;
		case 24:
			if (ev[i].value)
				st.pressure = ev[i].value;
			break;
		default:
			break;
		}
	}

	return st;
}

int init_touchscreen(int *fd, ScreenBounds *sb) {
	unsigned long bit[EV_MAX][KEY_MAX / (8 * sizeof(unsigned long))];
	int ev_abs[EV_ABS_SIZE] = {0};
	int i, j, k;

	if ((*fd = open("/dev/input/touchscreen0", O_RDONLY)) < 0) {
		return -1;
	}

	memset(bit, 0, sizeof(bit));
	ioctl(*fd, EVIOCGBIT(0, EV_MAX), bit[0]);

	sb->xMin = 0;
	sb->xMax = TS_MAX;
	sb->yMin = 0;
	sb->yMax = TS_MAX;
	for (i = 0; i < EV_MAX; i++) {
		if (!i)
			continue;
		if ((0x1 << (i%8)) & bit[0][i/8])
			continue;

		ioctl(*fd, EVIOCGBIT(i, EV_MAX), bit[i]);
	}

	return 0;
}
