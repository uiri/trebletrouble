typedef struct screen_bounds ScreenBounds;
struct screen_bounds {
	int xMin, xMax;
	int yMin, yMax;
};

typedef struct screen_input ScreenInput;
struct screen_input {
	int type;
	int x, y, pressure;
};

int cleanup_touchscreen(int *fd);
ScreenInput get_lcd_input(int *fd, ScreenBounds *sb);
int init_touchscreen(int *fd, ScreenBounds *sb);
