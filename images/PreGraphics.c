/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#include "inc/hw_memmap.h"
#include "grlib.h"
#include "Sharp96x96.h"
#include "HAL_MSP_EXP430FR5969_Sharp96x96.h"
#include "images.h"
#include "driverlib.h"
#include "stdint.h"


void PreGraphics();
void clockInit(void);
void boardInit();
void timerInit();
void Delay(void);


tContext g_sContext;
uint8_t g_delayCounter = 0;

void PreGraphics(void)
{
    unsigned int i=8;
    boardInit();
    clockInit();
    timerInit();


    // Set up the LCD
    HAL_LCD_initDisplay();

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __enable_interrupt();

    GrContextInit(&g_sContext, &g_sharp96x96LCD);
    GrContextForegroundSet(&g_sContext, ClrBlack);
    GrContextBackgroundSet(&g_sContext, ClrWhite);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    GrClearDisplay(&g_sContext);
    GrFlush(&g_sContext);

    // Draw Images on the display
//      while (1)
//      {
    while (i>1,i--)
    {

        GrImageDraw(&g_sContext, &Bell11BPP_UNCOMP, 0, 0);
        GrFlush(&g_sContext);
        _delay_cycles(3000000);
        GrClearDisplay(&g_sContext);

        GrImageDraw(&g_sContext, &Bell21BPP_UNCOMP, 0, 0);
        GrFlush(&g_sContext);
        _delay_cycles(3000000);
        GrClearDisplay(&g_sContext);

        GrImageDraw(&g_sContext, &Bell31BPP_UNCOMP, 0, 0);
        GrFlush(&g_sContext);
        _delay_cycles(3000000);
        GrClearDisplay(&g_sContext);

        GrImageDraw(&g_sContext, &Bell41BPP_UNCOMP, 0, 0);
        GrFlush(&g_sContext);
        _delay_cycles(3000000);
        GrClearDisplay(&g_sContext);

        GrImageDraw(&g_sContext, &Bell51BPP_UNCOMP, 0, 0);
        GrFlush(&g_sContext);
        _delay_cycles(3000000);
        GrClearDisplay(&g_sContext);


    }

}



void clockInit(void)
{
    CS_setDCOFreq(CS_DCORSEL_1,CS_DCOFSEL_3);

    CS_initClockSignal(
                    CS_ACLK,
                    CS_VLOCLK_SELECT,
                    CS_CLOCK_DIVIDER_1
                    );

    CS_initClockSignal(
                    CS_SMCLK,
                    CS_DCOCLK_SELECT,
                    CS_CLOCK_DIVIDER_1
                    );

    CS_initClockSignal(
                    CS_MCLK,
                    CS_DCOCLK_SELECT,
                    CS_CLOCK_DIVIDER_1
                    );

}

void boardInit()
{
    GPIO_setAsOutputPin(GPIO_PORT_PA,
            GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
            GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
            GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
            GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15);

    GPIO_setAsOutputPin(GPIO_PORT_PB,
                GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
                GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
                GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
                GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15);

    GPIO_setOutputLowOnPin(GPIO_PORT_PA,
                GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
                GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
                GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
                GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15);

    GPIO_setOutputLowOnPin(GPIO_PORT_PB,
                    GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
                    GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
                    GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
                    GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15);

}

void timerInit()
{
    Timer_A_initUpModeParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64;
    param.timerPeriod = 65535-1;
    param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
            TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear = TIMER_A_SKIP_CLEAR;
    param.startTimer = false;
    Timer_A_initUpMode(TIMER_A1_BASE, &param);

    Timer_A_initCompareModeParam initComp1Param = {0};
    initComp1Param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    initComp1Param.compareInterruptEnable =
            TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    initComp1Param.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    initComp1Param.compareValue = 62500;
    Timer_A_initCompareMode(TIMER_A1_BASE, &initComp1Param);

    Timer_A_startCounter(TIMER_A1_BASE,
            TIMER_A_UP_MODE
                        );
}


void Delay(void)
{
    //Reset delayCounter
    g_delayCounter=0;

    Timer_A_enableCaptureCompareInterrupt(TIMER_A1_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_1);

    Timer_A_clear(TIMER_A1_BASE);

    // Enter Low Power Mode 0 with Global Interrupts Enabled
    __bis_SR_register(LPM0_bits+GIE);

    Timer_A_disableCaptureCompareInterrupt(TIMER_A1_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_1);

}

//------------------------------------------------------------------------------
// Timer1 A0 Interrupt Service Routine
//------------------------------------------------------------------------------
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR (void)
{

    Sharp96x96_SendToggleVCOMCommand();

    // Add Offset to TACCR0
    Timer_A_setCompareValue(TIMER_A1_BASE,
                          TIMER_A_CAPTURECOMPARE_REGISTER_0,
                          Timer_A_getCaptureCompareCount(
                                  TIMER_A1_BASE,
                                  TIMER_A_CAPTURECOMPARE_REGISTER_0)+(65535-1));

    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0);

    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);


}

//------------------------------------------------------------------------------
// Timer1 A1 Interrupt Service Routine. This ISR id use to generate delay on
// LCD before changing screen content. The ISR is triggered every 500 msec.
//------------------------------------------------------------------------------
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR (void)
{

    g_delayCounter++;

    // Add Offset to TACCR0
    Timer_A_setCompareValue(TIMER_A1_BASE,
                            TIMER_A_CAPTURECOMPARE_REGISTER_1,
                            Timer_A_getCaptureCompareCount(
                            TIMER_A1_BASE,
                            TIMER_A_CAPTURECOMPARE_REGISTER_1)+(62500));

    // If 30 seconds have elapsed, exit LPM0 mode
    if(g_delayCounter==10){
        __bic_SR_register_on_exit(LPM0_bits);
    }

    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_1);

    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);

}




