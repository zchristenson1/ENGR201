#include "fr5969clock.h"

#include "../main.h"
#include "../driverlib/MSP430FR5xx_6xx/driverlib.h"

void clock_init()
{
	/*CS_DCOFSEL_0=0x00: DCORSEL=CS_DCOFSEL_0=0x00
	 * If DCORSEL = 0, and DCOFSEL = 1: 1 MHz
	 * If DCORSEL = 1, and DCOFSEL = 3: 8 MHz
	 * */
    CS_setDCOFreq(CS_DCORSEL_1, CS_DCOFSEL_3); // Set DCO frequency to 8 MHz
    CS_setExternalClockSource(32768, 0); //Set external clock frequency to 32.768 KHz
    CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1); //Set ACLK=LFXT

    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);	// Set SMCLK = DCO with frequency divider of 1 = 8 MHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); // Set MCLK = DCO with frequency divider of 1 = 8 MHz
    CS_turnOnLFXT(CS_LFXT_DRIVE_0); //Start XT1 with no time out
}

void sharplcd_clock_Init()
{
	/* CS_DCORSEL_1=DCORSEL=0x0040,
	 * CS_DCOFSEL_3=0x0006 --> DCOFSEL=011b
	 * If DCORSEL = 0: 4 MHz;
	 * If DCORSEL = 1: 8 MHz
	 * */
    CS_setDCOFreq(CS_DCORSEL_1,CS_DCOFSEL_3); // Set DCO frequency to 8 MHz

    /*
     * CS_VLOCLK_SELECT=SELM__VLOCLK=0x0001
     * VLOCLK=10KHz
     * */
    CS_initClockSignal(CS_ACLK, CS_VLOCLK_SELECT, CS_CLOCK_DIVIDER_1); //set ACLK=10KHz
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);	// Set SMCLK = DCO with frequency divider of 1 = 8MHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); // Set MCLK = DCO with frequency divider of 1 = 8MHz
}


