#include "fr5969calibration.h"

#include "../driverlib/MSP430FR5xx_6xx/driverlib.h"
#include "../FRAM/FRAMLogMode.h"
#include "../main.h"
#include "../UART/fr5969uart.h"
/*
 * Transmit Internal Temperature Sensor's Calibration constants through UART
 */
void sendCalibrationConstants()
{
    // Send Temp Sensor Calibration Data
	uart_data(CAL_ADC_12T30_H, comma);
	uart_data(CAL_ADC_12T30_L, LFCR);
//    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, CAL_ADC_12T30_H);
//    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, CAL_ADC_12T30_L);

	uart_data(CAL_ADC_12T85_H, comma);
	uart_data(CAL_ADC_12T85_L, LFCR);
//    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, CAL_ADC_12T85_H);
//    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, CAL_ADC_12T85_L);

//    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
//	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
}

/*
 * Transmit FRAMLogMode starting TimeStamp through UART
 */
void sendTimeStamp()
{
	// Select UART TXD on P2.0
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION);

    int i;
    for (i=0;i<6;i++) {
        __delay_cycles(500000);
        EUSCI_A_UART_transmitData(EUSCI_A0_BASE, timeStamp[i]);
    }

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
}

/*
 * Transmit 0xFF to acknowledge PC GUI's ping request
 */
void sendAckToPC()
{
	// Select UART TXD on P2.0
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION);

    // Send Ackknowledgement to Host PC
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, 0xFF);
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, 0xFF);

    while(EUSCI_A_UART_queryStatusFlags(EUSCI_A0_BASE, EUSCI_A_UART_BUSY));
    pingHost = 0;

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
}


