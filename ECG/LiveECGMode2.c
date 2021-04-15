#include "LiveECGMode.h"
#include "../main.h"
#include "../driverlib/MSP430FR5xx_6xx/driverlib.h"
#include "../ADC/fr5969adc.h"
#include "../UART/fr5969uart.h"
#include "../TIMER/fr5969timer.h"
#include "../LcdFunc/LcdFunc.h"
#include <stdio.h>

void ECGSensor_init2(int delayCycle)
{
	/*
	 * select Vref = ADC12_B_VREFPOS_INTBUF_VREFNEG_VSS(=3.6V)  or ADC12_B_VREFPOS_AVCC_VREFNEG_VSS(1.2v/2.0v/2.5v)
	 * select clockSource = ADC12_B_CLOCKSOURCE_ADC12OSC(1MHz) or ADC12_B_CLOCKSOURCE_ACLK(32KHz)
	 * */
	adc_init(ADC12_B_INPUT_A12, ADC12_B_NOINTCH, ADC12_B_VREFPOS_AVCC_VREFNEG_VSS, ADC12_B_CLOCKSOURCE_ADC12OSC);
	adc_mem0_clear();
	adc_mem0_int_enable();

    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P3,
        GPIO_PIN0,
		GPIO_SECONDARY_MODULE_FUNCTION
        );

    /* delay clocksource = 32KHz/1, delay for 1024 cycles=1024/clkSource(ACLK=32768)=1/32 s*/
	timerA0_init(TIMER_A_CLOCKSOURCE_ACLK, TIMER_A_CLOCKSOURCE_DIVIDER_1, delayCycle);
}

void liveECG2()
{


}
