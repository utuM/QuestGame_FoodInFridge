#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**************************************/

/* User libraries including section */

#include "include/game_defines.h"
#include "include/game_ui.h"

#include "wiringPi.h"

/**************************************/

using namespace std;

/* Variable definition section */

TGraphics *oGraphics;			// Graphics drawer
TQuest *oQuest;					// Quest mechanics object

bool needToBeFinished = NO;		// Main flag breaks all threads
pthread_t pInputThread;			// Thread, keyhooker
pthread_t pDisplayClick;		// Thread, display event waiter

char sTemp[4] = {0};			// Key buffer (can be much more simple, but still enough)

FILE *fsTestFile;				// Test log file, writes some useful info
char sTestInfo[256] = {0};		// Messages buffer guy

/**************************************/


void* InputCycle(void* ptrEmpty)
{
    char s[3];
    fgets(s, 2, stdin);
    finish();

    needToBeFinished = YES;
}

char sTest[64] = {0};
bool needToBeStopped = false;
bool isObjectDetected = false;

/* MAIN PROJECT FUNCTION */
int main()
{
    bool isStarted = false;

	// Graphic library initialization and few object constractors calling
    oGraphics = new TGraphics(YES, 0, 0, 480, 480);
    oQuest = new TQuest();

    // initializes OpenVG graphics library; draws window in
    // fullscreen mode if oQuest object field 'isFullscreen'
    // defines by 'true', otherwise - uses biases on X and Y
    // axes and user-defined window size
    oGraphics->InitializeWindow();

    // configures RaspberryPi functional library
    wiringPiSetup();
    // configures GPIOs state detection
    wiringPiSetupGpio();
    pinMode(WPI_GPIO19_PIN, INPUT);

    // configure GPIOs checking thread
    oQuest->SetThread();
    // start this thread
    //oQuest->StartThread();
    // main game cycle
    while(YES)
    {
        // check game mode:
        // MODE_GREETINGS - first loading display that allow
        // to start the game after RFID-card was swing behind
        // sensor;
        // MODE_GAMING - main mode that draws general information
        // on screen;
        // MODE_WAITING - low-powered mode that arrives after
        // defined time-out time; can be changed to MODE_GREETINGS

        // configures library to allow drawing inside created
        // window
        oGraphics->StartDrawing();

        switch(oQuest->GetMode())
        {
            case MODE_WAITING :
            {
                BlackscreenDraw(NO);

                break;
            }

            case MODE_GAMING :
            {
                if(!isStarted)
                {
                    isStarted = true;

                    pthread_create(&pInputThread, NULL, InputCycle, NULL);
                    pthread_create(&pDisplayClick, NULL, DisplayClickDetect, NULL);
                }

                DrawMenuSeparator();
                for(unsigned char i = 0; i < MENU_ITEMS_AMOUNT; i ++)
                {
                    DrawMenuItem(i);
                    if(i < 2)
                    {
                        for(unsigned char j = 0; j < 2; j ++)
                        {
                            DrawMenuObject(i, j);
                        }
                    }
                }

                if(isObjectDetected)
                {
                    needToBeStopped = true;

                    pinMode(WPI_GPIO19_PIN, OUTPUT);
					
                    digitalWrite(oQuest->GetUsingPin(), LOW);
                    usleep(500000);

                    digitalWrite(oQuest->GetUsingPin(), HIGH);

                    oQuest->SetMode(MODE_WAITING);

                    pinMode(WPI_GPIO19_PIN, INPUT);

                    sleep(60 * 10);

                    oQuest->SetItemIndex(-1);
                    oQuest->SetObjectIndex(-1);
                    oQuest->SetItemState(CLICK_STATE_NONE);
                    oQuest->SetObjState(CLICK_STATE_NONE);

                    oQuest->SetXClicked(0);
                    oQuest->SetYClicked(0);

                    isObjectDetected = false;

                    needToBeStopped = false;
                }

                break;
            }
        }

        sprintf(sTemp, "%.2X", (int)oQuest->GetMode());

        std::cout << sTemp << "; " << (int)oQuest->GetRisingEdges() << std::endl;
        oGraphics->EndDrawing();

        usleep(DRAW_SLEEPING_MS);

        if(needToBeFinished)
            break;
    }

	return 0;
}
