/*
 *  Copyright (C) 2016-2017  William Pearson
 *  Copyright (C) 2016-2017  Emilie Cobbold
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

/* #include "colours.h" */
/* #include "display.h" */
/* #include "input.h" */
/* #include "menu.h" */

#include "gfx.h"
#include "gui.h"

int main(int argc, char** argv) {
	/* char* fbp; */
	/* int err; */
	/* int fbfd; */
	/* ScreenInput si; */
	/* void (*menu_item)(char* fbp, ScreenInput* si); */

	gfxInit();

	gdispSetBacklight(100);
	gdispSetContrast(100);

	geventListenerInit(&glistener);
	gwinAttachListener(&glistener);

	guiCreate();

	while (1)
		guiEventLoop();

	/* fbp = init_display(&fbfd); */

	/* err = init_touchscreen(&si.fd); */
	/* if (err) { */
	/* 	printf("Error reading event input file\n"); */
	/* 	return err; */
	/* } */

	/* colour_screen(fbp, ORANGE); */
	/* get_lcd_input(&si); */

	/* if (si.y > (TS_MAX_Y / 2)) { */
	/* 	menu_item = metronome_menu; */
	/* } else { */
	/* 	menu_item = play_song_menu; */
	/* } */
	/* menu_item(fbp, &si); */

	/* get_lcd_input(&si); */
	/* cleanup_display(fbp, &fbfd); */
	return 0;
}
