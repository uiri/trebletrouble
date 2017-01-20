#define TS_MAX_X 800
#define TS_MAX_Y 480

typedef struct screen_input ScreenInput;
struct screen_input {
	int fd;
	int type;
	int x, y, pressure;
};

int cleanup_touchscreen(int *fd);
void get_lcd_input(ScreenInput *si);
int init_touchscreen(int *fd);
