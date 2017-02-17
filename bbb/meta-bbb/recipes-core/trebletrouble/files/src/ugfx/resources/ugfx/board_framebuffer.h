/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

// Set this to your frame buffer pixel format.
#ifndef GDISP_LLD_PIXELFORMAT
	#define GDISP_LLD_PIXELFORMAT		GDISP_PIXELFORMAT_RGB565
#endif

char *init_display(void);
void cleanup_display(char* fbp);

// Uncomment this if your frame buffer device requires flushing
//#define GDISP_HARDWARE_FLUSH		TRUE

#ifdef GDISP_DRIVER_VMT

	static void board_init(GDisplay *g, fbInfo *fbi) {
		g->g.Width = 800;
		g->g.Height = 480;
		g->g.Backlight = 100;
		g->g.Contrast = 50;
		fbi->linelen = g->g.Width * sizeof(LLDCOLOR_TYPE);				// bytes per row
		fbi->pixels = init_display();
	}

	#if GDISP_HARDWARE_FLUSH
		static void board_flush(GDisplay *g) {
			// TODO: Can be an empty function if your hardware doesn't support this
			(void) g;
		}
	#endif

	#if GDISP_NEED_CONTROL
		static void board_backlight(GDisplay *g, uint8_t percent) {
			// TODO: Can be an empty function if your hardware doesn't support this
			(void) g;
			(void) percent;
		}

		static void board_contrast(GDisplay *g, uint8_t percent) {
			// TODO: Can be an empty function if your hardware doesn't support this
			(void) g;
			(void) percent;
		}

		static void board_power(GDisplay *g, powermode_t pwr) {
			// TODO: Can be an empty function if your hardware doesn't support this
			(void) g;
			(void) pwr;
		}
	#endif

#endif /* GDISP_LLD_BOARD_IMPLEMENTATION */