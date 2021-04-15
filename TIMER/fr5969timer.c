#include "fr5969timer.h"

#include "../driverlib/MSP430FR5xx_6xx/driverlib.h"
#include "../main.h"
#include "../Temp/LiveTempMode.h"
#include <stdint.h>
#include "../LcdDriver/Sharp96x96.h"

/*
 * set timerA0 interrupt for ADC
 **/
void timerA0_init(unsigned int clockSource, unsigned int clkDivider, unsigned int delayCycle)
{
    // Start timer
    Timer_A_initUpModeParam param = {0};
    param.clockSource = clockSource; //select TIMER_A_CLOCKSOURCE_ACLK=TASSEL__ACLK=32KHz as clocksource
    param.clockSourceDivider = clkDivider; //select TIMER_A_CLOCKSOURCE_DIVIDER_1=0x00
    param.timerPeriod = 4; //hold data for 13 cycles
    param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear = TIMER_A_DO_CLEAR;
    param.startTimer = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &param);

    //set TAxCCTL0(TIMER_A_CAPTURECOMPARE_REGISTER_0) for delay n cycles/clkSource
    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, delayCycle);
}

/*
 * Timer0_A3 Interrupt Vector (TAIV) handler
 * Used to trigger ADC conversion every 0.125 seconds
 *
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    __bic_SR_register_on_exit(LPM3_bits); // Exit active CPU
}
