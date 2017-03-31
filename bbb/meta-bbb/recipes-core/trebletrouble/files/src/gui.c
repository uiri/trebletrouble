/*
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

/******************************************************************************/
/* This file has been generated by the uGFX-Studio                            */
/*                                                                            */
/* http://ugfx.org                                                            */
/******************************************************************************/

#include "colors.h"
#include "widgetstyles.h"

#include "gui.h"
#include "menu.h"

#include "src/gdisp/gdisp.h"
#include "src/gdisp/gdisp_driver.h"
#include "src/gdisp/gdisp_pixmap.h"

#include "src/gwin/gwin_label.h"

#include "colours.h"
#include "display.h"

extern GDisplay* GDISP;

// GListeners
GListener glistener;

// GHandles
GHandle ghContainerPage0;
GHandle ghContainer1;
GHandle ghButton1_2;
GHandle ghButton1_1;
GHandle ghButton1;
GHandle ghLabel1;

GHandle ghContainerFeedbackPage;
GHandle ghProgressbar4;
GHandle ghProgressbar1;
GHandle ghProgressbar2;
GHandle ghProgressbar3;
GHandle ghLabel6;
GHandle ghLabel5;
GHandle ghLabel4;
GHandle ghLabel3;
GHandle ghLabel2;
GHandle ghButton2;

// Fonts
font_t dejavu_sans_16;
font_t dejavu_sans_16_anti_aliased;
font_t dejavu_sans_32;

static void createPageFeedbackPage(void)
{
  GWidgetInit wi;
  gwinWidgetClearInit(&wi);


  // create container widget: ghContainerFeedbackPage
  wi.g.show = FALSE;
  wi.g.x = 0;
  wi.g.y = 0;
  wi.g.width = 800;
  wi.g.height = 480;
  wi.g.parent = 0;
  wi.text = "Container";
  wi.customDraw = 0;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghContainerFeedbackPage = gwinContainerCreate(0, &wi, 0);

  // create container widget: ghContainer1
  wi.g.show = TRUE;
  wi.g.x = 50;
  wi.g.y = 110;
  wi.g.width = 600;
  wi.g.height = 300;
  wi.g.parent = ghContainerFeedbackPage;
  wi.text = "Container1";
  wi.customDraw = gwinContainerDraw_Std;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghContainer1 = gwinContainerCreate(0, &wi, GWIN_CONTAINER_BORDER);

  // Create progressbar widget: ghProgressbar4
  wi.g.show = TRUE;
  wi.g.x = 30;
  wi.g.y = 240;
  wi.g.width = 200;
  wi.g.height = 20;
  wi.g.parent = ghContainer1;
  wi.text = "% Correct";
  wi.customDraw = gwinProgressbarDraw_Std;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghProgressbar4 = gwinProgressbarCreate(0, &wi);
  gwinProgressbarSetRange(ghProgressbar4, 0, 100);
  gwinProgressbarSetPosition(ghProgressbar4, 25);

  // Create progressbar widget: ghProgressbar1
  wi.g.show = TRUE;
  wi.g.x = 30;
  wi.g.y = 60;
  wi.g.width = 200;
  wi.g.height = 20;
  wi.g.parent = ghContainer1;
  wi.text = "% Correct";
  wi.customDraw = gwinProgressbarDraw_Std;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghProgressbar1 = gwinProgressbarCreate(0, &wi);
  gwinProgressbarSetRange(ghProgressbar1, 0, 100);
  gwinProgressbarSetPosition(ghProgressbar1, 25);

  // Create progressbar widget: ghProgressbar2
  wi.g.show = TRUE;
  wi.g.x = 30;
  wi.g.y = 120;
  wi.g.width = 200;
  wi.g.height = 20;
  wi.g.parent = ghContainer1;
  wi.text = "% Correct";
  wi.customDraw = gwinProgressbarDraw_Std;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghProgressbar2 = gwinProgressbarCreate(0, &wi);
  gwinProgressbarSetRange(ghProgressbar2, 0, 100);
  gwinProgressbarSetPosition(ghProgressbar2, 25);

  // Create progressbar widget: ghProgressbar3
  wi.g.show = TRUE;
  wi.g.x = 30;
  wi.g.y = 180;
  wi.g.width = 200;
  wi.g.height = 20;
  wi.g.parent = ghContainer1;
  wi.text = "% Correct";
  wi.customDraw = gwinProgressbarDraw_Std;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghProgressbar3 = gwinProgressbarCreate(0, &wi);
  gwinProgressbarSetRange(ghProgressbar3, 0, 100);
  gwinProgressbarSetPosition(ghProgressbar3, 25);

  // Create label widget: ghLabel1
  wi.g.show = TRUE;
  wi.g.x = 50;
  wi.g.y = 70;
  wi.g.width = 180;
  wi.g.height = 32;
  wi.g.parent = ghContainerFeedbackPage;
  wi.text = "Feedback";
  wi.customDraw = gwinLabelDrawJustifiedLeft;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghLabel1 = gwinLabelCreate(0, &wi);
  gwinLabelSetBorder(ghLabel1, FALSE);
  gwinSetFont(ghLabel1, dejavu_sans_32);
  gwinRedraw(ghLabel1);

  // Create label widget: ghLabel2
  wi.g.show = TRUE;
  wi.g.x = 30;
  wi.g.y = 40;
  wi.g.width = 120;
  wi.g.height = 20;
  wi.g.parent = ghContainer1;
  wi.text = "Bar1";
  wi.customDraw = gwinLabelDrawJustifiedLeft;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghLabel2 = gwinLabelCreate(0, &wi);
  gwinLabelSetBorder(ghLabel2, FALSE);

  // Create label widget: ghLabel3
  wi.g.show = TRUE;
  wi.g.x = 30;
  wi.g.y = 100;
  wi.g.width = 120;
  wi.g.height = 20;
  wi.g.parent = ghContainer1;
  wi.text = "Bar2";
  wi.customDraw = gwinLabelDrawJustifiedLeft;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghLabel3 = gwinLabelCreate(0, &wi);
  gwinLabelSetBorder(ghLabel3, FALSE);

  // Create label widget: ghLabel4
  wi.g.show = TRUE;
  wi.g.x = 30;
  wi.g.y = 160;
  wi.g.width = 120;
  wi.g.height = 20;
  wi.g.parent = ghContainer1;
  wi.text = "Bar3";
  wi.customDraw = gwinLabelDrawJustifiedLeft;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghLabel4 = gwinLabelCreate(0, &wi);
  gwinLabelSetBorder(ghLabel4, FALSE);

  // Create label widget: ghLabel5
  wi.g.show = TRUE;
  wi.g.x = 30;
  wi.g.y = 220;
  wi.g.width = 120;
  wi.g.height = 20;
  wi.g.parent = ghContainer1;
  wi.text = "Bar4";
  wi.customDraw = gwinLabelDrawJustifiedLeft;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghLabel5 = gwinLabelCreate(0, &wi);
  gwinLabelSetBorder(ghLabel5, FALSE);

  // create button widget: ghButton1
  wi.g.show = TRUE;
  wi.g.x = 680;
  wi.g.y = 380;
  wi.g.width = 120;
  wi.g.height = 20;
  wi.g.parent = ghContainerFeedbackPage;
  wi.text = "Return";
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customParam = 0;
  wi.customStyle = 0;
  ghButton2 = gwinButtonCreate(0, &wi);
}

static void createPagePage0(void)
{
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);


	// create container widget: ghContainerPage0
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = 800;
	wi.g.height = 400;
	wi.g.parent = 0;
	wi.text = "Container";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghContainerPage0 = gwinContainerCreate(0, &wi, 0);

	// create container widget: ghContainer1
	wi.g.show = TRUE;
	wi.g.x = 90;
	wi.g.y = 40;
	wi.g.width = 620;
	wi.g.height = 360;
	wi.g.parent = ghContainerPage0;
	wi.text = "Container1";
	wi.customDraw = gwinContainerDraw_Std;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghContainer1 = gwinContainerCreate(0, &wi, GWIN_CONTAINER_BORDER);

	// create button widget: ghButton1_2
	wi.g.show = TRUE;
	wi.g.x = 10;
	wi.g.y = 260;
	wi.g.width = 600;
	wi.g.height = 75;
	wi.g.parent = ghContainer1;
	wi.text = "Settings";
	wi.customDraw = gwinButtonDraw_Rounded;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghButton1_2 = gwinButtonCreate(0, &wi);
	gwinSetFont(ghButton1_2, dejavu_sans_16_anti_aliased);
	gwinRedraw(ghButton1_2);

	// create button widget: ghButton1_1
	wi.g.show = TRUE;
	wi.g.x = 10;
	wi.g.y = 150;
	wi.g.width = 600;
	wi.g.height = 75;
	wi.g.parent = ghContainer1;
	/* wi.text = "Compose Mode"; */
	wi.text = "Metronome";
	wi.customDraw = gwinButtonDraw_Rounded;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghButton1_1 = gwinButtonCreate(0, &wi);

	// create button widget: ghButton1
	wi.g.show = TRUE;
	wi.g.x = 10;
	wi.g.y = 40;
	wi.g.width = 600;
	wi.g.height = 75;
	wi.g.parent = ghContainer1;
	wi.text = "Practice Mode";
	wi.customDraw = gwinButtonDraw_Rounded;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghButton1 = gwinButtonCreate(0, &wi);

	// Create label widget: ghLabel1
	/* wi.g.show = TRUE; */
	/* wi.g.x = 280; */
	/* wi.g.y = 290; */
	/* wi.g.width = 120; */
	/* wi.g.height = 20; */
	/* wi.g.parent = ghButton1_2; */
	/* wi.text = "Settings"; */
	/* wi.customDraw = gwinLabelDrawJustifiedLeft; */
	/* wi.customParam = 0; */
	/* wi.customStyle = 0; */
	/* ghLabel1 = gwinLabelCreate(0, &wi); */

}

void guiShowPage(unsigned pageIndex)
{
	// Hide all pages
	gwinHide(ghContainerPage0);
	gwinHide(ghContainerFeedbackPage);

	// Show page selected page
	switch (pageIndex) {
	case 0:
		gwinShow(ghContainerPage0);
		break;
	case 1: 
	  	gwinShow(ghContainerFeedbackPage);
		break;

	default:
		break;
	}
}

void guiCreate(void)
{
	GWidgetInit wi;

	// Prepare fonts
	dejavu_sans_16 = gdispOpenFont("DejaVuSans16");
	dejavu_sans_16_anti_aliased = gdispOpenFont("DejaVuSans16_aa");
	dejavu_sans_32 = gdispOpenFont("DejaVuSans32");

	// Prepare images

	// GWIN settings
	gwinWidgetClearInit(&wi);
	gwinSetDefaultFont(dejavu_sans_16);
	gwinSetDefaultStyle(&white, FALSE);
	gwinSetDefaultColor(black_studio);
	gwinSetDefaultBgColor(white_studio);

	// Create all the display pages
	createPagePage0();
	createPageFeedbackPage();

	// Select the default display page
	colour_screen(GDISP->board, WHITE);
	guiShowPage(0);

}

void guiEventLoop(void)
{
	GEvent* pe;
	void (*menu_item)(char* fbp, ScreenInput *si);
	char* fbp = GDISP->board;

	while (1) {
		// Get an event
		pe = geventEventWait(&glistener, 0);
		if (!pe)
			continue;
		switch (pe->type) {
		case GEVENT_GWIN_BUTTON:
		  gwinHide(ghContainerPage0);
		  gwinHide(ghContainerFeedbackPage);
		  	if (((GEventGWinButton*)pe)->gwin == ghButton1) {
				menu_item = play_song_menu;
				menu_item(fbp, NULL);
				gwinShow(ghContainerFeedbackPage);
			} else if (((GEventGWinButton*)pe)->gwin == ghButton1_1) {
				menu_item = metronome_menu;
				menu_item(fbp, NULL);
			} else if (((GEventGWinButton*)pe)->gwin == ghButton2) {
			  	gwinShow(ghContainerPage0);
			} else {
			  	gwinShow(ghContainerPage0);
			}
			break;
		default:
			break;
		}
	}
}
