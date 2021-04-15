#include "LcdFunc.h"

#include "../driverlib/MSP430FR5xx_6xx/inc/hw_memmap.h"
#include "../GrLib/grlib/grlib.h"
#include "../LcdDriver/Sharp96x96.h"
#include "../LcdDriver/HAL_MSP_EXP430FR5969_Sharp96x96.h"
#include "../images/images.h"
#include "../GPIO/fr5969gpio.h"
#include "../CLOCK/fr5969clock.h"
#include "../TIMER/fr5969timer.h"
#include "../ECG/LiveECGMode.h"
#include "../driverlib/MSP430FR5xx_6xx/driverlib.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

tContext g_sContext;
tRectangle g_sRect;
tRectangle myRectangle1 = { 5, 15, 65, 45};
tRectangle myRectangle2 = { 10, 40, 90, 90};
tRectangle myRectangle3 = { 0, 0, 95, 95};

static unsigned int frame=0;

static void FlushLcd();
static void IntroLcd();
static void DrawLine();
static void DrawCircle();
static void DrawRectangle();
static void InvertColor();
static void DrawRocket();

static void FlushLcd()
{
    GrContextInit(&g_sContext, &g_sharp96x96LCD);
    GrContextForegroundSet(&g_sContext, ClrBlack);
    GrContextBackgroundSet(&g_sContext, ClrWhite);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    GrClearDisplay(&g_sContext);
    GrFlush(&g_sContext);
}

static void IntroLcd()
{
    // Intro Screen
    GrStringDrawCentered(&g_sContext,
                         "How to use",
                         AUTO_STRING_LENGTH,
                         48,
                         15,
                         TRANSPARENT_TEXT);
    GrStringDrawCentered(&g_sContext,
                         "the MSP430",
                         AUTO_STRING_LENGTH,
                         48,
                         35,
                         TRANSPARENT_TEXT);
    GrStringDraw(&g_sContext,
                 "Graphics Library",
                 AUTO_STRING_LENGTH,
                 1,
                 51,
                 TRANSPARENT_TEXT);
    GrStringDrawCentered(&g_sContext,
                         "Primitives",
                         AUTO_STRING_LENGTH,
                         48,
                         75,
                         TRANSPARENT_TEXT);

    GrFlush(&g_sContext);
    Delay(); //call a delay
    GrClearDisplay(&g_sContext);
}

static void DrawLine()
{
    // Draw pixels and lines on the display
    GrStringDrawCentered(&g_sContext,
                         "Draw Pixels",
                         AUTO_STRING_LENGTH,
                         48,
                         5,
                         TRANSPARENT_TEXT);
    GrStringDrawCentered(&g_sContext,
                         "& Lines",
                         AUTO_STRING_LENGTH,
                         48,
                         15,
                         TRANSPARENT_TEXT);
    GrPixelDraw(&g_sContext, 30, 30);
    GrPixelDraw(&g_sContext, 30, 32);
    GrPixelDraw(&g_sContext, 32, 32);
    GrPixelDraw(&g_sContext, 32, 30);
    GrLineDraw(&g_sContext, 35, 35, 90, 90);
    GrLineDraw(&g_sContext, 5, 80, 80, 20);
    GrLineDraw(&g_sContext,
               0,
               GrContextDpyHeightGet(&g_sContext) - 1,
               GrContextDpyWidthGet(&g_sContext) - 1,
               GrContextDpyHeightGet(&g_sContext) - 1);
    GrFlush(&g_sContext);
    Delay(); //call a delay
    GrClearDisplay(&g_sContext);
}

static void DrawCircle()
{
    // Draw circles on the display
    GrStringDraw(&g_sContext,
                 "Draw Circles",
                 AUTO_STRING_LENGTH,
                 10,
                 5,
                 TRANSPARENT_TEXT);
    GrCircleDraw(&g_sContext,
                 30,
                 70,
                 20);
    GrCircleFill(&g_sContext,
                 60,
                 50,
                 30);
    GrFlush(&g_sContext);
    Delay(); //call a delay
    GrClearDisplay(&g_sContext);
}

static void DrawRectangle()
{
    // Draw rectangles on the display
    GrStringDrawCentered(&g_sContext,
                         "Draw Rectangles",
                         AUTO_STRING_LENGTH,
                         48,
                         5,
                         TRANSPARENT_TEXT);
    GrRectDraw(&g_sContext, &myRectangle1);
    GrRectFill(&g_sContext, &myRectangle2);
    // Text below won't be visible on screen due to transparency
    // (foreground colors match)
    GrStringDrawCentered(&g_sContext,
                         "Normal Text",
                         AUTO_STRING_LENGTH,
                         50,
                         50,
                         TRANSPARENT_TEXT);
    // Text below draws foreground and background for opacity
    GrStringDrawCentered(&g_sContext,
                         "Opaque Text",
                         AUTO_STRING_LENGTH,
                         50,
                         65,
                         OPAQUE_TEXT);
    GrContextForegroundSet(&g_sContext, ClrWhite);
    GrContextBackgroundSet(&g_sContext, ClrBlack);
    GrStringDrawCentered(&g_sContext,
                         "Invert Text",
                         AUTO_STRING_LENGTH,
                         50,
                         80,
                         TRANSPARENT_TEXT);
    GrFlush(&g_sContext);
    Delay(); //call a delay
}

static void InvertColor()
{
    // Invert the foreground and background colors
    GrContextForegroundSet(&g_sContext, ClrBlack);
    GrContextBackgroundSet(&g_sContext, ClrWhite);
    GrRectFill(&g_sContext, &myRectangle3);
    GrContextForegroundSet(&g_sContext, ClrWhite);
    GrContextBackgroundSet(&g_sContext, ClrBlack);
    GrStringDrawCentered(&g_sContext,
                         "Invert Colors",
                         AUTO_STRING_LENGTH,
                         48,
                         5,
                         TRANSPARENT_TEXT);
    GrRectDraw(&g_sContext, &myRectangle1);
    GrRectFill(&g_sContext, &myRectangle2);
    // Text below won't be visible on screen due to
    // transparency (foreground colors match)
    GrStringDrawCentered(&g_sContext,
                         "Normal Text",
                         AUTO_STRING_LENGTH,
                         50,
                         50,
                         TRANSPARENT_TEXT);
    // Text below draws foreground and background for opacity
    GrStringDrawCentered(&g_sContext,
                         "Opaque Text",
                         AUTO_STRING_LENGTH,
                         50,
                         65,
                         OPAQUE_TEXT);
    // Text below draws with inverted foreground color to become visible
    GrContextForegroundSet(&g_sContext, ClrBlack);
    GrContextBackgroundSet(&g_sContext, ClrWhite);
    GrStringDrawCentered(&g_sContext,
                         "Invert Text",
                         AUTO_STRING_LENGTH,
                         50,
                         80,
                         TRANSPARENT_TEXT);
    GrFlush(&g_sContext);
    Delay(); //call a delay
    GrContextForegroundSet(&g_sContext, ClrBlack);
    GrContextBackgroundSet(&g_sContext, ClrWhite);
    GrClearDisplay(&g_sContext);
}

static void DrawRocket()
{
    // Draw Images on the display
    GrImageDraw(&g_sContext, &Bell11BPP_UNCOMP, 0, 0);
    GrFlush(&g_sContext);
    Delay(); //call a delay
    GrClearDisplay(&g_sContext);
    GrImageDraw(&g_sContext, &Bell21BPP_UNCOMP, 0, 0);
    GrFlush(&g_sContext);
    Delay();
    GrClearDisplay(&g_sContext);
    GrImageDraw(&g_sContext, &Bell31BPP_UNCOMP, 0, 0);
    GrFlush(&g_sContext);
    Delay(); //call a delay
    GrClearDisplay(&g_sContext);
    GrImageDraw(&g_sContext, &Bell41BPP_UNCOMP, 0, 0);
    GrFlush(&g_sContext);
    Delay();
    GrClearDisplay(&g_sContext);
    GrImageDraw(&g_sContext, &Bell51BPP_UNCOMP, 0, 0);
    GrFlush(&g_sContext);
    Delay(); //call a delay
    GrClearDisplay(&g_sContext);
}

void Lcd_init()
{
    //set up LCD updating frequency
//  timerA1_init(TIMER_A_CLOCKSOURCE_SMCLK, TIMER_A_CLOCKSOURCE_DIVIDER_10, 10000); //delay timer=1MHz/10=0.1MHz, delay for 10000 cycles=10000/0.1MHz=0.1s

    // Set up the LCD
    HAL_LCD_initDisplay();
    FlushLcd();

    GrStringDrawCentered(&g_sContext,
                         "ECG Output",
                         AUTO_STRING_LENGTH,
                         48,
                         5,
                         TRANSPARENT_TEXT);
}

void ClearFrameNum()
{
    char _time_str[15];

// Clear part of buffer used for no. of frames
    unsigned char i = 0, j = 0;

    for (i = 10; i < 20; i++)
    {
        for (j = 0; j < 12; j++)
        {
            DisplayBuffer[i][j] = 0xFF;
        }
    }

    sprintf(_time_str, "%d frame(s)", frame);
    frame+=1;

    GrStringDrawCentered(&g_sContext,
                        (signed char *)_time_str,
                         AUTO_STRING_LENGTH,
                         48,
                         15,
                         TRANSPARENT_TEXT);

    GrFlush(&g_sContext);

    if(frame==65536)
    {
        frame=0;
    }
}

void ShowFrameNum()
{
    char _time_str[15];

// Clear part of buffer used for no. of frame

    sprintf(_time_str, "%d frame(s)", frame);
    frame+=1;

    GrStringDrawCentered(&g_sContext,
                        (signed char *)_time_str,
                         AUTO_STRING_LENGTH,
                         48,
                         15,
                         TRANSPARENT_TEXT);

    GrFlush(&g_sContext);
}

void DrawAdc(unsigned int *adcArray)
{
    // Clear part of buffer used for adcArray
    unsigned char i = 0, j = 0;

    for (i = 0; i < 128; i++)
    {
        for (j = 0; j < 12; j++)
        {
            DisplayBuffer[i][j] = 0xFF;
        }
    }

    ShowFrameNum();

// Draw pixels and lines on the display

    int p=0;

    while(p<96)
    {
        if(p<95)
        {
            GrLineDraw(&g_sContext, p, 95-adcArray[p], p, 95-adcArray[p+1]);
        }
        else
        {
          GrPixelDraw(&g_sContext, p, 95-adcArray[p]); //p=0~95, 76-adcArray[p]=25~127, x_baseline=76
        }
        p++;
    }

    GrFlush(&g_sContext);
}

void LcdDrawTest()
{
    FlushLcd();
    IntroLcd();
    DrawLine();
    DrawCircle();
    DrawRectangle();
    InvertColor();
    DrawRocket();
}

