//******************************************************************************
//!  EUSCI_A0 External Loopback test using EUSCI_A_UART_init API
//!
//!  Description: This demo connects TX to RX of the MSP430 UART
//!  The example code shows proper initialization of registers
//!  and interrupts to receive and transmit data.
//!
//!  ACLK = BRCLK = 32.768kHz, MCLK = SMCLK = DCO = ~1MHz
//!
//!
//!      Tested on MSP430FR5969
//!             -----------------
//!       RST -|     P2.0/UCA0TXD|----|
//!            |                 |    |
//!            |                 |    |
//!            |     P2.1/UCA0RXD|----|
//!            |                 |
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - UART peripheral
//! - GPIO Port peripheral (for UART pins)
//! - UCA0TXD
//! - UCA0RXD
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - USCI_A0_VECTOR.
//******************************************************************************
#include "fr5969uart.h"

#include "../main.h"
#include "../driverlib/MSP430FR5xx_6xx/driverlib.h"
#include <stdlib.h>
#include <stdio.h>

#include "../CLOCK/fr5969clock.h"
#include "../GPIO/fr5969gpio.h"

static void BR115200();
static void uart_delay();
static void mark_LFCR();
static void mark_comma();
static void mark_space();

uint16_t i;
uint8_t RXData = 0, TXData = 0;
uint8_t check = 0;

static void BR115200()
{
	// Configure UART baudrate 115200
    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK; //1MHz
    param.clockPrescalar = 8;   //3
    param.firstModReg = 0;
    param.secondModReg = 0xd6;  //92
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;  //EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION: not support sampling a UART bitstream with higher input clk
//    param.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;  //EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION: not support sampling a UART bitstream with higher input clk

    if(STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A0_BASE, &param))
    {
        return;
    }

    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    EUSCI_A_UART_clearInterrupt(EUSCI_A0_BASE,
                                EUSCI_A_UART_RECEIVE_INTERRUPT);

    // Enable USCI_A0 RX interrupt
    EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE,
                                 EUSCI_A_UART_RECEIVE_INTERRUPT); // Enable interrupt
}

void uart_init(void)
{
    BR115200();
}

static void uart_delay()
{
    while(EUSCI_A_UART_queryStatusFlags(EUSCI_A0_BASE, EUSCI_A_UART_BUSY));
	__delay_cycles(1000); //1ms
}

static void mark_LFCR()
{
	EUSCI_A_UART_transmitData(EUSCI_A0_BASE, 13);
	uart_delay();
	EUSCI_A_UART_transmitData(EUSCI_A0_BASE, 10);
	uart_delay();
}

static void mark_comma()
{
	EUSCI_A_UART_transmitData(EUSCI_A0_BASE, 44);
	uart_delay();
}

static void mark_space()
{
	EUSCI_A_UART_transmitData(EUSCI_A0_BASE, 32);
	uart_delay();
}

void uart_print(char *c_str)
{
	int i=0;
	while(*(c_str+i)!='\0')
	{
		EUSCI_A_UART_transmitData(EUSCI_A0_BASE, c_str[i]);
		i++;
		uart_delay();
	}
	return;
}

void uart_data(unsigned int num, char mark)
{
	char str[10];

	sprintf(str, "%d", num);
	uart_print(str);
	switch(mark)
	{
		case 0:
			mark_comma();
			break;
		case 1:
			mark_space();
			break;
		case 2:
			mark_LFCR();
			break;

	    default:
	    	break;
	}
}

static unsigned int txdata=0;
void whoami()
{
	char *c_str="FR5969=";
	uart_print(c_str);
	uart_data(txdata, LFCR);
	txdata++;
	if(txdata==255)
	{
		txdata=0;
	}
}

//******************************************************************************
//
//This is the USCI_A0 interrupt vector service routine.
//
//******************************************************************************
#ifdef UARTCOM

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
    {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
        RXData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
		GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN6); //LED1
		__delay_cycles(100000);
        break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    }
}

#else
/*
 * USCI_A0 Interrupt Service Routine that receives PC GUI's commands
 */
#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	int i;
    switch (__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG)) {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            i = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
        	if (i == '5')
                pingHost = 1;
            else
                mode = i;
            __bic_SR_register_on_exit(LPM3_bits); // Exit active CPU
            break;
        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;
    }
}
#endif


