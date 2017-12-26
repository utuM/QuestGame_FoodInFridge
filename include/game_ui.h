#ifndef GAME_UI_H_INCLUDED
#define GAME_UI_H_INCLUDED

/**************************************/

#include <iostream>

/**************************************/

#include "game_defines.h"

#include "VG/openvg.h"
#include "VG/vgu.h"

#include "fontinfo.h"
#include "shapes.h"

/**************************************/

#define     STD_LABEL_HEIGHT_PX    (VGfloat)80.0f
#define     STD_STATUS_HEIGHT_PX   (VGfloat)60.0f

#define     STATUSBAR_MARGIN_PX    (VGfloat)10.0f
#define     STATUSBAR_POLY_PTS          6

#define     FIRST_STATUS_COLOR      255,  0,  0,  1.0f
#define     SECOND_STATUS_COLOR      0,   0, 255, 1.0f
#define     THIRD_STATUS_COLOR      255, 127, 0,  1.0f
#define     FORTH_STATUS_COLOR      255,  0, 255, 1.0f

#define     GREETINGS_IMAGE_SIZE        600

#define     DEFAULT_IMAGE_SIZE          240
#define     DEFAULT_LABEL_WIDTH         260
#define     DEFAULT_LABEL_HEIGHT         70

#define     MENU_ITEMS_AMOUNT            4
#define     MENU_LIST_WIDTH_COEF        0.21875f
#define     MENU_ITEM_SPACE_PX          10.0f
#define     MENU_ITEM_HEIGHT            80.0f

/**************************************/

unsigned char DrawGreetingsSign(void);

unsigned char BlackscreenDraw(bool isGreetings);

unsigned char DrawMenuSeparator();
unsigned char DrawMenuItem(unsigned char uIndex);
unsigned char DrawMenuObject(unsigned char uRow, unsigned char uCol);

/**************************************/

unsigned char DrawLabelBar(unsigned char uIndex);
unsigned char FillLabelBar(unsigned char uIndex, char* sText);

unsigned char DrawImageBar(unsigned char uIndex);
unsigned char DrawImageDirectly(unsigned char uIndex);

unsigned char DrawStatusBar(unsigned char uIndex);
unsigned char FillStatusBar(unsigned char uIndex);

#endif // GAME_UI_H_INCLUDED
