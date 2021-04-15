#include "LiveTempMode.h"

#include "../main.h"
#include "../driverlib/MSP430FR5xx_6xx/driverlib.h"
#include <stdio.h>

#include "../ADC/fr5969adc.h"
#include "../UART/fr5969uart.h"
#include "../TIMER/fr5969timer.h"
#include "../LcdFunc/LcdFunc.h"

void TempSensor_init(int adcInput, int delayCycle)
{
	/*
	 * select Vref = ADC12_B_VREFPOS_INTBUF_VREFNEG_VSS(=3.6V)  or ADC12_B_VREFPOS_AVCC_VREFNEG_VSS(1.2v/2.0v/2.5v)
	 * select clockSource = ADC12_B_CLOCKSOURCE_ADC12OSC(1MHz) or ADC12_B_CLOCKSOURCE_ACLK(32KHz)
	 * */
	adc_init(ADC12_B_INPUT_A12, ADC12_B_NOINTCH, ADC12_B_VREFPOS_AVCC_VREFNEG_VSS, ADC12_B_CLOCKSOURCE_ADC12OSC);
	adc_mem0_clear();
	adc_mem0_int_enable();

//	set_Ref_A(0, REF_A_VREF2_0V);
//    __bis_SR_register(LPM3_bits | GIE);       //Enter LPM3. Delay for Ref to settle.

//    /* delay clocksource = 32KHz/1, delay for 1024 cycles=1024/clkSource(ACLK=32768)=1/32 ms*/
    timerA0_init(TIMER_A_CLOCKSOURCE_ACLK, TIMER_A_CLOCKSOURCE_DIVIDER_1, delayCycle);

    /* delay clocksource = 1MHz/1, delay for 250 cycles=250/clkSource(SMCLK=1MHz)=0.25ms*/
//    timerA0_init(TIMER_A_CLOCKSOURCE_SMCLK, TIMER_A_CLOCKSOURCE_DIVIDER_40, 250);
}

void liveTemp()
{
    int i=0;
    unsigned int adcArray[96]={0};
    int j=96;

#if 1
    while(i<j)
    {
    	{
			__bis_SR_register(LPM3_bits | GIE);   // Enter LPM3, wait for ~1024/32768 sec timer

			//Enable/Start sampling and conversion
			ADC12_B_startConversion(ADC12_B_BASE, ADC12_B_MEMORY_0, ADC12_B_SINGLECHANNEL);

			__bis_SR_register(LPM3_bits | GIE);   // Wait for conversion to complete
			__bic_SR_register(GIE);

			int temp_adc;

			temp_adc=ADC12_B_getResults(ADC12_B_BASE, ADC12_B_MEMORY_0)>>6;

			uart_data(temp_adc, LFCR);
			adcArray[i]=temp_adc;
    	}

    	i++;

    	if(i%j==0)
    	{
			DrawAdc(adcArray);	//draw every 1 sec
    	}

		if(i==j){
			i=0;
		}
    }
#endif

    // Disable ADC12 and Timer_A0
    ADC12_B_disable(ADC12_B_BASE);
    Timer_A_stop(TIMER_A1_BASE);
    Timer_A_stop(TIMER_A0_BASE);
}
