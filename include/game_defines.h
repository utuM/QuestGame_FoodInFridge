#ifndef GAME_DEFINES_H
#define GAME_DEFINES_H

/**************************************/

#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>

/**************************************/

#include "VG/openvg.h"
#include "VG/vgu.h"

#include "fontinfo.h"
#include "game_ui.h"
#include "shapes.h"
#include "wiringPi.h"

/**************************************/

#define     YES     true
#define     NO      false

#define     FUNC_RESULT_OK       0x00
#define     FUNC_RESULT_ERROR    0xFF

#define     MODE_INIT           0x00
#define     MODE_GREETINGS      0x01
#define     MODE_GREETS_IMP      5
#define     MODE_GAMING         0x02
#define     MODE_WIN_IMP         5
#define     MODE_WAITING        0x04
#define     MODE_WAIT_IMP        6

#define     WIN_OPACITY_INC      20

#define     REPEATS_AMOUNT        1

#define     SLEEP_MICROSECS     100000
#define     SLEEP_DEVIATION       0

#define     DRAW_SLEEPING_MS    100000

#define     STATS_AMOUNT          4
#define     STAT_MAX_PERS        100
#define     STAT_MIN_PERS         50

#define     BACKGROUND_COLOR      (unsigned int)0, (unsigned int)0, (unsigned int)0

#define     GPIO_READ_TRYOUT    3

#define     WPI_GPIO4_PIN       4
#define     WPI_GPIO5_PIN       5
#define     WPI_GPIO6_PIN       6

#define     WPI_GPIO12_PIN      12
#define     WPI_GPIO13_PIN      13
#define     WPI_GPIO16_PIN      16
#define     WPI_GPIO17_PIN      17
#define     WPI_GPIO18_PIN      18
#define     WPI_GPIO19_PIN      19
#define     WPI_GPIO20_PIN      20
#define     WPI_GPIO21_PIN      21
#define     WPI_GPIO22_PIN      22
#define     WPI_GPIO23_PIN      23
#define     WPI_GPIO24_PIN      24
#define     WPI_GPIO25_PIN      25
#define     WPI_GPIO26_PIN      26
#define     WPI_GPIO27_PIN      27

#define     CLICK_STATE_NONE    0x00
#define     CLICK_STATE_PRES    0x01
#define     CLICK_STATE_RELE    0x02

#define     SUCCESS_ITEM         3
#define     SUCCESS_OBJECT       3

/**************************************/

const unsigned short uUserWidth = 1024;
const unsigned short uUserHeight = 600;

/**************************************/

using namespace std;

void* DisplayClickDetect(void* ptrEmpty);

class TGraphics
{
    private:
        bool isFullscreen;
        int iWinXBias;
        int iWinYBias;
        int iWinHeight;
        int iWinWidth;

    public:
        TGraphics(bool _isFullscreen, int _iWinXBias,
                  int _iWinYBias, int _iWinWidth,
                  int _iWinHeight);
        ~TGraphics();

        int GetWindowWidth(void)    { return iWinWidth; }
        int GetWindowHeight(void)    { return iWinHeight; }

        unsigned char InitializeWindow(void);
        unsigned char StartDrawing(void);
        unsigned char EndDrawing(void);
};

class TQuest
{
    private:
        short uMenuItem;
        int iXBegItem[4];
        int iXEndItem[4];
        int iYBegItem[4];
        int iYEndItem[4];

        short uMenuObject;
        int iXBegObj[4];
        int iXEndObj[4];
        int iYBegObj[4];
        int iYEndObj[4];

        unsigned char uMode;
        unsigned char uRisingEdges;
        clock_t uEdgeDuration;

        unsigned int uSleepTime;
        unsigned char uRepeats;
        int iUsingPin;
        pthread_t pPinLevelThread;

        int iXPosition;
        int iYPosition;

        unsigned char uItemState;
        unsigned char uObjState;
        int iXClicked;
        int iYClicked;

    public:
        TQuest();
        ~TQuest();

        unsigned int GetSleepTime(void) { return uSleepTime; }
        unsigned char GetMode(void) { return uMode; }
        unsigned char GetRisingEdges(void) { return uRisingEdges; }
        int GetUsingPin(void) { return iUsingPin; }
        unsigned char GetRepeats(void) { return uRepeats; }
        clock_t GetEdgeDuration(void) { return uEdgeDuration; }
        pthread_t* GetThreadHandler(void) { return &pPinLevelThread; }
        int GetXPosition(void) { return iXPosition; }
        int GetYPosition(void) { return iYPosition; }

        unsigned char GetItemState(void) { return uItemState; }
        unsigned char GetObjState(void) { return uObjState; }
        int GetXClicked(void) { return iXClicked;}
        int GetYClicked(void) { return iYClicked;}

        short GetItemIndex(void) { return uMenuItem; }
        short GetObjectIndex(void) { return uMenuObject; }
        int GetItemXBeg(unsigned char uIndex) { return iXBegItem[uIndex]; }
        int GetItemXEnd(unsigned char uIndex) { return iXEndItem[uIndex]; }
        int GetItemYBeg(unsigned char uIndex) { return iYBegItem[uIndex]; }
        int GetItemYEnd(unsigned char uIndex) { return iYEndItem[uIndex]; }

        int GetObjXBeg(unsigned char uIndex) { return iXBegObj[uIndex]; }
        int GetObjXEnd(unsigned char uIndex) { return iXEndObj[uIndex]; }
        int GetObjYBeg(unsigned char uIndex) { return iYBegObj[uIndex]; }
        int GetObjYEnd(unsigned char uIndex) { return iYEndObj[uIndex]; }

        static void* StaticCheckPinState(void* ptrValue);
        void CheckPinState();

        void SetMode(unsigned char uValue) { uMode = uValue; }
        void SetRepeats(unsigned char uValue) { uRepeats = uValue; }
        void SetEdgeDuration(clock_t uDuration) { uEdgeDuration = uDuration; }
        void SetThread(void);
        void SetXPosition(int _iXPos) { iXPosition = _iXPos; }
        void SetYPosition(int _iYPos) { iYPosition = _iYPos; }
        void SetItemIndex(short _uItemIndex) { uMenuItem = _uItemIndex; }
        void SetObjectIndex(short _uObjIndex) { uMenuObject = _uObjIndex; }

        void SetItemState(unsigned char _uItemState) { uItemState = _uItemState; }
        void SetObjState(unsigned char _uObjState) { uObjState = _uObjState; }
        void SetXClicked(int _iXClicked) { iXClicked = _iXClicked; }
        void SetYClicked(int _iYClicked) { iYClicked = _iYClicked; }

        void StartThread(void);
};

#endif
