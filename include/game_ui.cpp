#include "game_ui.h"

/**************************************/

extern TGraphics *oGraphics;
extern TQuest *oQuest;

extern bool isObjectDetected;

VGfloat vgfStatFieldWidth;
VGfloat vgfImageBarHeight;

VGfloat vgfImageBias = ((uUserWidth / 4.0f) -
                       (2 * STATUSBAR_MARGIN_PX) -
                        DEFAULT_IMAGE_SIZE) / 2.0f;
VGfloat vgfLabelBias = ((uUserWidth / 4.0f) -
                       (2 * STATUSBAR_MARGIN_PX) -
                       DEFAULT_LABEL_WIDTH) / 2.0f;

VGfloat vgfXItemsField = uUserWidth * MENU_LIST_WIDTH_COEF;
VGfloat vgfXObjectsField = uUserWidth - vgfXItemsField;

char sFilePath[256] = {0};

/**************************************/

unsigned char DrawMenuSeparator()
{
    Fill(220, 220, 220, 1.0f);
    Rect(0, 0, vgfXItemsField, uUserHeight);

    Fill(112, 146, 190, 1.0f);
    Rect(vgfXItemsField, 0, uUserWidth - vgfXItemsField, uUserHeight);

    Fill(150, 150, 150, 1.0f);
    Rect(vgfXItemsField - 1, 0, 1, uUserHeight);
}

unsigned char DrawMenuItem(unsigned char uIndex)
{
    if(uIndex <= 6)
    {
        struct TEMP_POLY_COORDS
        {
            VGfloat x[STATUSBAR_POLY_PTS];
            VGfloat y[STATUSBAR_POLY_PTS];
        } stTempPolygon;

        VGfloat vgfTempStartYPos = uUserHeight - (MENU_ITEM_SPACE_PX + MENU_ITEM_HEIGHT) * (uIndex + 1);
        VGfloat vgfTempXLength = vgfXItemsField - 2 * MENU_ITEM_SPACE_PX;

        Fill(190, 190, 190, 1.0f);
        Rect(MENU_ITEM_SPACE_PX, vgfTempStartYPos, vgfTempXLength, MENU_ITEM_HEIGHT);

        Fill(150, 150, 150, 1);
        stTempPolygon.x[0] = MENU_ITEM_SPACE_PX;
        stTempPolygon.y[0] = vgfTempStartYPos + MENU_ITEM_HEIGHT;

        stTempPolygon.x[1] = MENU_ITEM_SPACE_PX + 5;
        stTempPolygon.y[1] = vgfTempStartYPos + MENU_ITEM_HEIGHT - 5;

        stTempPolygon.x[2] = vgfTempXLength + 5;
        stTempPolygon.y[2] = vgfTempStartYPos + MENU_ITEM_HEIGHT - 5;

        stTempPolygon.x[3] = vgfTempXLength + 5;
        stTempPolygon.y[3] = vgfTempStartYPos + 5;

        stTempPolygon.x[4] = vgfTempXLength + 10;
        stTempPolygon.y[4] = vgfTempStartYPos;

        stTempPolygon.x[5] = MENU_ITEM_SPACE_PX;
        stTempPolygon.y[5] = vgfTempStartYPos;

        Polygon(stTempPolygon.x, stTempPolygon.y, 6);

        if(oQuest->GetItemIndex() == (short)(uIndex))
        {
            if(oQuest->GetItemState() == CLICK_STATE_PRES)
                sprintf(sFilePath, "resource/%u_item_pressed.jpg", uIndex + 1);
            else if(oQuest->GetItemState() == CLICK_STATE_RELE)
                sprintf(sFilePath, "resource/%u_item_release.jpg", uIndex + 1);
            else
                sprintf(sFilePath, "resource/%u_item_normal.jpg", uIndex + 1);
        }
        else
            sprintf(sFilePath, "resource/%u_item_normal.jpg", uIndex + 1);

        Image(MENU_ITEM_SPACE_PX + 5.0f, vgfTempStartYPos + 5.0f,
              194, MENU_ITEM_HEIGHT - MENU_ITEM_SPACE_PX,
              sFilePath);
    }

    return 0;
}


unsigned char DrawMenuObject(unsigned char uRow, unsigned char uCol)
{
    VGfloat vgfTempXLength = (vgfXObjectsField - 3 * MENU_ITEM_SPACE_PX) * 0.5f;

    VGfloat vgfTempYLength = (uUserHeight - 3 * MENU_ITEM_SPACE_PX) * 0.5f;

    VGfloat vgfTempYPos = uUserHeight - vgfTempYLength - MENU_ITEM_SPACE_PX;

    if((oQuest->GetObjState() == CLICK_STATE_RELE) &&
       ((oQuest->GetObjectIndex() % 2 == uCol) &&
       ((int)((oQuest->GetObjectIndex() - uCol) / 2.0f) == uRow)))
    {
        Fill(255, 0, 0, 1.0f);
        Rect(vgfXItemsField + MENU_ITEM_SPACE_PX + (vgfTempXLength + MENU_ITEM_SPACE_PX) * uCol - 4,
          vgfTempYPos - (MENU_ITEM_SPACE_PX + vgfTempYLength) * uRow - 4,
          vgfTempXLength + 8, vgfTempYLength + 8);
    }


    Fill(110, 110, 110, 1.0f);
    Rect(vgfXItemsField + MENU_ITEM_SPACE_PX + (vgfTempXLength + MENU_ITEM_SPACE_PX) * uCol,
          vgfTempYPos - (MENU_ITEM_SPACE_PX + vgfTempYLength) * uRow,
          vgfTempXLength, vgfTempYLength);

    Fill(255, 255, 255, 1.0f);
    Rect(vgfXItemsField + MENU_ITEM_SPACE_PX + (vgfTempXLength + MENU_ITEM_SPACE_PX) * uCol + 2,
          vgfTempYPos - (MENU_ITEM_SPACE_PX + vgfTempYLength) * uRow + 2,
          vgfTempXLength - 4, vgfTempYLength - 4);

    Fill(110, 110, 110, 1.0f);
    Rect(vgfXItemsField + MENU_ITEM_SPACE_PX + (vgfTempXLength + MENU_ITEM_SPACE_PX) * uCol + 10,
          vgfTempYPos - (MENU_ITEM_SPACE_PX + vgfTempYLength) * uRow + 70,
          vgfTempXLength - 20, vgfTempYLength - 80);

    if(oQuest->GetItemIndex() >= 0)
    {
        unsigned char uTempIndex = 2 * uRow + uCol;
        sprintf(sFilePath, "resource/%i_%i_object_image.jpg", oQuest->GetItemIndex() + 1,
                                                          uTempIndex + 1);

        Image(vgfXItemsField + MENU_ITEM_SPACE_PX + (vgfTempXLength + MENU_ITEM_SPACE_PX) * uCol + 12,
              vgfTempYPos - (MENU_ITEM_SPACE_PX + vgfTempYLength) * uRow + 72,
              vgfTempXLength - 24, vgfTempYLength - 84, sFilePath);

        sprintf(sFilePath, "resource/%i_%i_object.jpg", oQuest->GetItemIndex() + 1,
                                                        uTempIndex + 1);
        Image(vgfXItemsField + MENU_ITEM_SPACE_PX + (vgfTempXLength + MENU_ITEM_SPACE_PX) * uCol + 12,
              vgfTempYPos - (MENU_ITEM_SPACE_PX + vgfTempYLength) * uRow + 2,
                    vgfTempXLength - 24, 70, sFilePath);
    }

    if(oQuest->GetItemIndex() == SUCCESS_ITEM &&
       oQuest->GetObjectIndex() == SUCCESS_OBJECT)
            isObjectDetected = true;

    return 0;
}

/**************************************/

unsigned char DrawGreetingsSign()
{
    Image((uUserWidth - GREETINGS_IMAGE_SIZE) / 2.0f,
          (uUserHeight - GREETINGS_IMAGE_SIZE) / 2.0f,
          GREETINGS_IMAGE_SIZE, GREETINGS_IMAGE_SIZE,  "resource/greetings.jpg");
}

unsigned char BlackscreenDraw(bool isGreetings)
{
    Background(BACKGROUND_COLOR);
    if(isGreetings)
    {
        Fill(255, 255, 255, 1.0f);
        Text(0, oGraphics->GetWindowWidth() / 2.0f, "Hello there. Pass your RFID-card behind and wait for sum.", SansTypeface, 24.0f);
    }

    return FUNC_RESULT_OK;
}

unsigned char DrawLabelBar(unsigned char uIndex)
{
    if(uIndex >= STATS_AMOUNT)
    {
        std::cout << "ERROR DrawLabelBar(): unknown player status index (can be from 0 to 3)" << std::endl;
        return FUNC_RESULT_ERROR;
    }

    vgfStatFieldWidth = (VGfloat)(oGraphics->GetWindowWidth() / 4.0f);

    VGfloat vgfTempStartXPos = vgfStatFieldWidth * uIndex;
    VGfloat vgfTempHeight = STD_LABEL_HEIGHT_PX;
    VGfloat vgfTempStartYPos = (VGfloat)oGraphics->GetWindowHeight() - STATUSBAR_MARGIN_PX - vgfTempHeight;

    struct TEMP_POLY_COORDS
    {
        VGfloat x[STATUSBAR_POLY_PTS];
        VGfloat y[STATUSBAR_POLY_PTS];
    } stTempPolygon;

    VGfloat vgfTempWidth = vgfStatFieldWidth - 2 * STATUSBAR_MARGIN_PX;

    /* base rectangle of percents status bar */
    Fill(180, 180, 180, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX, vgfTempStartYPos, vgfTempWidth, vgfTempHeight);

    Fill(120, 120, 120, 1);
    stTempPolygon.x[0] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[0] = vgfTempStartYPos + vgfTempHeight;

    stTempPolygon.x[1] = vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5;
    stTempPolygon.y[1] = vgfTempStartYPos + 75;

    stTempPolygon.x[2] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[2] = vgfTempStartYPos + 75;

    stTempPolygon.x[3] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[3] = vgfTempStartYPos + 5;

    stTempPolygon.x[4] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX;
    stTempPolygon.y[4] = vgfTempStartYPos;

    stTempPolygon.x[5] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[5] = vgfTempStartYPos;

    Polygon(stTempPolygon.x, stTempPolygon.y, 6);

    Fill(255, 255, 255, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5.0f, vgfTempStartYPos + 5.0f, vgfTempWidth - STATUSBAR_MARGIN_PX, 70.0f);

    return FUNC_RESULT_OK;
}

unsigned char FillLabelBar(unsigned char uIndex, char* sText)
{
    Fill(0, 0, 0, 1.0f);

    vgfStatFieldWidth = (VGfloat)(oGraphics->GetWindowWidth() / 4.0f);

    VGfloat vgfTempStartXPos = vgfStatFieldWidth * uIndex;
    VGfloat vgfTempHeight = STD_LABEL_HEIGHT_PX;
    VGfloat vgfTempStartYPos = (VGfloat)oGraphics->GetWindowHeight() - STATUSBAR_MARGIN_PX - vgfTempHeight;

    VGfloat vgfTempWidth = vgfStatFieldWidth - 2 * STATUSBAR_MARGIN_PX;

    Text(vgfTempStartXPos + vgfTempWidth / 4.0f, vgfTempStartYPos + STD_LABEL_HEIGHT_PX / 3.5f, sText, SansTypeface, STD_LABEL_HEIGHT_PX / 2.25f);

    return FUNC_RESULT_OK;
}

unsigned char DrawImageBar(unsigned char uIndex)
{
    if(uIndex >= STATS_AMOUNT)
    {
        std::cout << "ERROR DrawLabelBar(): unknown player status index (can be from 0 to 3)" << std::endl;
        return FUNC_RESULT_ERROR;
    }

    vgfStatFieldWidth = (VGfloat)(oGraphics->GetWindowWidth() / 4.0f);

    VGfloat vgfTempStartXPos = vgfStatFieldWidth * uIndex;
    VGfloat vgfTempHeight = (VGfloat)oGraphics->GetWindowHeight() - 4 * STATUSBAR_MARGIN_PX - STD_LABEL_HEIGHT_PX;
    vgfImageBarHeight = vgfTempHeight;

    VGfloat vgfTempStartYPos = STATUSBAR_MARGIN_PX;

    struct TEMP_POLY_COORDS
    {
        VGfloat x[STATUSBAR_POLY_PTS];
        VGfloat y[STATUSBAR_POLY_PTS];
    } stTempPolygon;

    VGfloat vgfTempWidth = vgfStatFieldWidth - 2 * STATUSBAR_MARGIN_PX;

    /* base rectangle of percents status bar */
    Fill(230, 230, 230, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX, vgfTempStartYPos, vgfTempWidth, vgfTempHeight);

    Fill(170, 170, 170, 1);
    stTempPolygon.x[0] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[0] = vgfTempStartYPos + vgfTempHeight;

    stTempPolygon.x[1] = vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5;
    stTempPolygon.y[1] = vgfTempStartYPos + vgfTempHeight - 5;

    stTempPolygon.x[2] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[2] = vgfTempStartYPos + vgfTempHeight - 5;

    stTempPolygon.x[3] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[3] = vgfTempStartYPos + 5;

    stTempPolygon.x[4] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX;
    stTempPolygon.y[4] = vgfTempStartYPos;

    stTempPolygon.x[5] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[5] = vgfTempStartYPos;

    Polygon(stTempPolygon.x, stTempPolygon.y, 6);

    Fill(80, 80, 80, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5.0f, vgfTempStartYPos + 5.0f, vgfTempWidth - STATUSBAR_MARGIN_PX, vgfTempHeight - STATUSBAR_MARGIN_PX);

    Fill(250, 250, 250, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 7.0f, vgfTempStartYPos + 5.0f, vgfTempWidth - STATUSBAR_MARGIN_PX - 2.0f, vgfTempHeight - STATUSBAR_MARGIN_PX - 2.0f);

    return FUNC_RESULT_OK;
}

unsigned char DrawImageDirectly(unsigned char uIndex)
{
    VGfloat vgfXPos = (uUserWidth / 4.0f) * uIndex +
                       STATUSBAR_MARGIN_PX,
            vgfYPos = 80.0f;

    switch(uIndex)
    {
        case 0 :
        {
            Image(vgfXPos + vgfImageBias, vgfYPos, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE,
                                            "resource/health_icon.jpg");
            Image(vgfXPos + vgfLabelBias, uUserHeight - STD_LABEL_HEIGHT_PX - 5,
                                            DEFAULT_LABEL_WIDTH, DEFAULT_LABEL_HEIGHT,
                                            "resource/health_label.jpg");
            break;
        }
        case 1 :
        {
            Image(vgfXPos + vgfImageBias, vgfYPos, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE,
                                            "resource/shield_icon.jpg");
            Image(vgfXPos + vgfLabelBias, uUserHeight - STD_LABEL_HEIGHT_PX - 5,
                                            DEFAULT_LABEL_WIDTH, DEFAULT_LABEL_HEIGHT,
                                            "resource/shield_label.jpg");
            break;
        }
        case 2 :
        {
            Image(vgfXPos + vgfImageBias, vgfYPos, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE,
                                            "resource/food_icon.jpg");
            Image(vgfXPos + vgfLabelBias, uUserHeight - STD_LABEL_HEIGHT_PX - 5,
                                            DEFAULT_LABEL_WIDTH, DEFAULT_LABEL_HEIGHT,
                                            "resource/food_label.jpg");
            break;
        }
        case 3 :
        {
            Image(vgfXPos + vgfImageBias, vgfYPos, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE,
                                            "resource/sleep_icon.jpg");
            Image(vgfXPos + vgfLabelBias, uUserHeight - STD_LABEL_HEIGHT_PX - 5,
                                            DEFAULT_LABEL_WIDTH, DEFAULT_LABEL_HEIGHT,
                                            "resource/sleep_label.jpg");
            break;
        }
    }
}

unsigned char DrawStatusBar(unsigned char uIndex)
{
    if(uIndex >= STATS_AMOUNT)
    {
        std::cout << "ERROR DrawStatusBar(): unknown player status index (can be from 0 to 3)" << std::endl;
        return FUNC_RESULT_ERROR;
    }

    vgfStatFieldWidth = (VGfloat)(oGraphics->GetWindowWidth() / 4.0f);
    VGfloat vgfTempStartXPos = vgfStatFieldWidth * uIndex;
    struct TEMP_POLY_COORDS
    {
        VGfloat x[STATUSBAR_POLY_PTS];
        VGfloat y[STATUSBAR_POLY_PTS];
    } stTempPolygon;

    VGfloat vgfTempWidth = vgfStatFieldWidth - 2 * STATUSBAR_MARGIN_PX;

    /* base rectangle of percents status bar */
    Fill(230, 230, 230, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX, STATUSBAR_MARGIN_PX, vgfTempWidth, 60.0f);

    Fill(170, 170, 170, 1);
    stTempPolygon.x[0] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[0] = 70;

    stTempPolygon.x[1] = vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5;
    stTempPolygon.y[1] = 65;

    stTempPolygon.x[2] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[2] = 65;

    stTempPolygon.x[3] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[3] = 15;

    stTempPolygon.x[4] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX;
    stTempPolygon.y[4] = 10;

    stTempPolygon.x[5] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[5] = STATUSBAR_MARGIN_PX;

    Polygon(stTempPolygon.x, stTempPolygon.y, 6);

    Fill(80, 80, 80, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5.0f, 15.0f, vgfTempWidth - STATUSBAR_MARGIN_PX, 50.0f);

    Fill(110, 110, 110, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 7.0f, 15.0f, vgfTempWidth - STATUSBAR_MARGIN_PX - 2.0f, 48.0f);

    return FUNC_RESULT_OK;
}
