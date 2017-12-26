#include "game_defines.h"

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <X11/keysymdef.h>

/**************************************/

extern bool needToBeFinished;
extern pthread_t pDisplayClick;

extern TQuest *oQuest;
extern char sTest[64];

extern bool needToBeStopped;

VGfloat vgfXObjectLength;
VGfloat vgfYObjectLength;

/**************************************/

void* DisplayClickDetect(void* ptrEmpty)
{
    bool isDetected = NO;

    float fItemsField = uUserWidth * MENU_LIST_WIDTH_COEF;

    XSetWindowAttributes attribs;
    Window wWindow;
    Display *dDisplay;
    int iDefDisplay;
    XEvent xeMouseKeyboardEvent;

    Window root;

    dDisplay = XOpenDisplay(NULL);
    iDefDisplay = DefaultScreen(dDisplay);

    attribs.override_redirect = True;
    attribs.event_mask = ExposureMask | KeyPressMask | ButtonPress |
                          StructureNotifyMask | ButtonReleaseMask |
                          KeyReleaseMask | EnterWindowMask | LeaveWindowMask |
                          PointerMotionMask | Button1MotionMask | VisibilityChangeMask |
                          ColormapChangeMask;
    wWindow = XCreateWindow(dDisplay, DefaultRootWindow (dDisplay), 0, 0, uUserWidth, uUserHeight, 0,
         CopyFromParent, InputOnly, CopyFromParent,
         CWOverrideRedirect, &attribs);

    XSelectInput(dDisplay, wWindow, ButtonPressMask|ButtonReleaseMask|StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask);
    XMapWindow(dDisplay, wWindow);

    XSync (dDisplay, 0);

    unsigned m;
    int x = 0, y = 0, i;

    while(!needToBeFinished || !needToBeStopped)
    {
        XNextEvent(dDisplay, &xeMouseKeyboardEvent);

        switch(xeMouseKeyboardEvent.type)
        {
            case ButtonPress :
            {
                if(xeMouseKeyboardEvent.xbutton.x < fItemsField)
                {
                    oQuest->SetItemState(CLICK_STATE_PRES);

                    oQuest->SetXClicked(xeMouseKeyboardEvent.xbutton.x);
                    oQuest->SetYClicked(uUserHeight - xeMouseKeyboardEvent.xbutton.y);

                    oQuest->SetObjectIndex(-1);
                }
                else
                {
                    oQuest->SetObjState(CLICK_STATE_PRES);

                    oQuest->SetXClicked(xeMouseKeyboardEvent.xbutton.x);
                    oQuest->SetYClicked(uUserHeight - xeMouseKeyboardEvent.xbutton.y);
                }

                break;
            }
            case ButtonRelease :
            {
                if(xeMouseKeyboardEvent.xbutton.x < fItemsField)
                {
                    oQuest->SetItemState(CLICK_STATE_RELE);
                }
                else
                {
                    oQuest->SetObjState(CLICK_STATE_RELE);
                }

                break;
            }

            case KeyPress :
            {
                if ( xeMouseKeyboardEvent.xkey.keycode == 0x09 )
                {
                    needToBeFinished = YES;
                    XCloseDisplay(dDisplay);
                }

                break;
            }
        }

        for(unsigned char i = 0; i < STATS_AMOUNT; i ++)
        {
            if(((oQuest->GetItemXBeg(i) <= oQuest->GetXClicked()) &&
               (oQuest->GetItemXEnd(i) >= oQuest->GetXClicked())) &&
               ((oQuest->GetItemYBeg(i) <= oQuest->GetYClicked()) &&
               (oQuest->GetItemYEnd(i) >= oQuest->GetYClicked())))
            {
                oQuest->SetItemIndex((short)i);

                isDetected = YES;
                break;
            }
        }
        if(!isDetected)
        {
            for(unsigned char i = 0; i < 4; i ++)
            {
                if(((oQuest->GetObjXBeg(i) <= oQuest->GetXClicked()) &&
                   (oQuest->GetObjXEnd(i) >= oQuest->GetXClicked())) &&
                   ((oQuest->GetObjYBeg(i) <= oQuest->GetYClicked()) &&
                   (oQuest->GetObjYEnd(i) >= oQuest->GetYClicked())) &&
                   (oQuest->GetItemIndex() >= 0 && oQuest->GetItemIndex() <= 3))
                {
                    oQuest->SetObjectIndex((short)i);

                    isDetected = YES;
                    break;
                }
            }
        }

        isDetected = NO;

        usleep(DRAW_SLEEPING_MS);
    }

    XCloseDisplay(dDisplay);
}

/**************************************/

TGraphics::TGraphics(bool _isFullscreen, int _iWinXBias, int _iWinYBias,
                     int _iWinWidth, int _iWinHeight)
{
    isFullscreen = _isFullscreen;
    iWinXBias = _iWinXBias;
    iWinYBias = _iWinYBias;
    iWinWidth = _iWinWidth;
    iWinHeight = _iWinHeight;
}

unsigned char TGraphics::InitializeWindow()
{
    if(isFullscreen)
    {
        init(&iWinWidth, &iWinHeight);
    }
    else
    {
        initWindowSize(iWinXBias, iWinYBias, iWinWidth, iWinHeight);
        init(&iWinWidth, &iWinHeight);
    }

    return FUNC_RESULT_OK;
}

unsigned char TGraphics::StartDrawing()
{
    Start(iWinWidth, iWinHeight);
    Background(BACKGROUND_COLOR);

    return FUNC_RESULT_OK;
}

unsigned char TGraphics::EndDrawing()
{
    End();
    char s[3];

    return FUNC_RESULT_OK;
}

/**************************************/

extern VGfloat vgfXObjectLength;

TQuest::TQuest()
{
    iXPosition = iYPosition = 0;

    uSleepTime = SLEEP_MICROSECS + SLEEP_DEVIATION;

    uMenuItem = uMenuObject = -1;

    uEdgeDuration = 0;

    uMode = MODE_WAITING;
    uRisingEdges = MODE_WAIT_IMP;

    uRepeats = 0;
    iUsingPin = WPI_GPIO19_PIN;

    uItemState = CLICK_STATE_NONE;
    uObjState = CLICK_STATE_NONE;

    pPinLevelThread = NULL;

    vgfXObjectLength = (uUserWidth - (uUserWidth * MENU_LIST_WIDTH_COEF) -
                       (3 * MENU_ITEM_SPACE_PX)) / 2.0f;
    vgfYObjectLength = (uUserHeight - 3 * MENU_ITEM_SPACE_PX) / 2.0f;

    for(unsigned char i = 0; i < 4; i ++)
    {
        iXBegItem[i] = STATUSBAR_MARGIN_PX;
        iXEndItem[i] = uUserWidth * MENU_LIST_WIDTH_COEF -
                        MENU_ITEM_SPACE_PX;
        iYEndItem[i] = uUserHeight - (STATUSBAR_MARGIN_PX +
                       (STATUSBAR_MARGIN_PX + MENU_ITEM_HEIGHT) * i);
        iYBegItem[i] = uUserHeight - (STATUSBAR_MARGIN_PX + MENU_ITEM_HEIGHT +
                       (STATUSBAR_MARGIN_PX + MENU_ITEM_HEIGHT) * i);

        iXBegObj[i] = iXEndItem[i] + 2 * STATUSBAR_MARGIN_PX +
                      (STATUSBAR_MARGIN_PX + vgfXObjectLength) * (i % 2);
        iXEndObj[i] = iXEndItem[i] + 2 * STATUSBAR_MARGIN_PX +
                      (STATUSBAR_MARGIN_PX + vgfXObjectLength) * (i % 2) +
                      vgfXObjectLength;

        if(i < 2)
        {
            iYEndObj[i] = uUserHeight - STATUSBAR_MARGIN_PX;
            iYBegObj[i] = uUserHeight - STATUSBAR_MARGIN_PX - vgfYObjectLength;
        }
        else
        {
            iYEndObj[i] = uUserHeight - (2 * STATUSBAR_MARGIN_PX + vgfYObjectLength);
            iYBegObj[i] = uUserHeight - (2 * (STATUSBAR_MARGIN_PX + vgfYObjectLength));
        }
    }
}

void TQuest::SetThread()
{
    (void)pthread_create(&pPinLevelThread, NULL, StaticCheckPinState, this);
}

void TQuest::StartThread()
{
     (void)pthread_join(pPinLevelThread, NULL);
}

/*
 * @brief   Checking GPIO state by its number; major
 *          checking cycle
 * @param   ptrGpio Pointer to GPIO number that need
 *                  to be checked
 */
void* TQuest::StaticCheckPinState(void* ptrValue)
{
    ((TQuest*) ptrValue)->CheckPinState();
}

void TQuest::CheckPinState()
{
    unsigned char uTryings = 0;
    // infinite cycle
    while(YES)
    {
        if(needToBeFinished)
            break;

        // if GPIO state is 1 (impulse is up-leveled)
        if(!digitalRead(iUsingPin) && !needToBeStopped)
        {
            // increase rising edges amount
            uTryings ++;
        }
        // if GPIO state is 0 (impulse is down-leveled)
        else
        {
            if(uTryings >= GPIO_READ_TRYOUT)
            {
                // check if we got some amount of impulses
                // in a row
                switch(oQuest->GetMode())
                {
                case MODE_WAITING :
                    {
                        oQuest->SetMode(MODE_GAMING);

                        break;
                    }
                default :
                    {

                        break;
                    }
                }
            }

            uTryings = 0;
        }

        // wait time between 2 neighbour front edges of
        // impulses ('uSleepTime' defined in the 'main'
        // function
        usleep(uSleepTime);
    }
}
