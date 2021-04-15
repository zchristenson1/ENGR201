#include "ADC/fr5969adc.h"
#include "BUTTON/fr5969button.h"
#include "CALIBRATION/fr5969calibration.h"
#include "CLOCK/fr5969clock.h"
#include "driverlib/MSP430FR5xx_6xx/driverlib.h"
#include "driverlib/MSP430FR5xx_6xx/inc/hw_memmap.h"
#include "FRAM/FRAMLogMode.h"
#include "GPIO/fr5969gpio.h"
#include "GrLib/grlib/grlib.h"
#include "images/images.h"
#include "LcdDriver/Sharp96x96.h"
#include "LcdDriver/HAL_MSP_EXP430FR5969_Sharp96x96.h"
#include "RTC/fr5969rtc.h"
#include "ECG/LiveECGMode.h"
#include "TIMER/fr5969timer.h"
#include "UART/fr5969uart.h"
#include "LcdFunc/LcdFunc.h"

#include "main.h"
#include <stdint.h>

int mode = 0;                                     // mode selection variable
int pingHost = 0;                                 // ping request from PC GUI

#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = 0x9000
__no_init uint16_t dataArray[12289];
#endif

//-----------------------------------------------------------------------------
int _system_pre_init(void)
{
//	// Disable the GPIO power-on default high-impedance mode to activate
//	// previously configured port settings
//	PM5CTL0 &= ~LOCKLPM5;

	// Stop Watchdog timer
     WDT_A_hold(WDT_A_BASE);     // Stop WDT

    /*==================================*/
    /* Choose if segment initialization */
    /* should be done or not. */
    /* Return: 0 to omit initialization */
    /* 1 to run initialization */
    /*==================================*/
    return 1;
}

void main(void)
{
	// stop watchdog
	_system_pre_init();

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();

    // Board initializations
	gpio_init();
	clock_init();
	uart_init();
	Lcd_init();

	// You can call your initial LED blinking here

	blinkingLED();

	// Draw Images on the display
	PreGraphics();

	// ADC A12 sampling interval ECGSensor_init1(x); => x/32768 sec
	ECGSensor_init1(256);

    __enable_interrupt();

	// Display ECG signal
    liveECG1();

}
