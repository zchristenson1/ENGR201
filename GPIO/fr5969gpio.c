#include "fr5969gpio.h"

#include "../main.h"
#include "../driverlib/MSP430FR5xx_6xx/driverlib.h"

#include <stdlib.h>
#include <stdio.h>

void gpio_init()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7|GPIO_PIN8|GPIO_PIN9|GPIO_PIN10|GPIO_PIN11|GPIO_PIN12|GPIO_PIN13|GPIO_PIN14|GPIO_PIN15);

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_PJ, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7|GPIO_PIN8|GPIO_PIN9|GPIO_PIN10|GPIO_PIN11|GPIO_PIN12|GPIO_PIN13|GPIO_PIN14|GPIO_PIN15);

    // LFXT Setup
    //Set PJ.4 and PJ.5 as Primary Module Function Input.
    /*
     * Select Port J
     * Set Pin 4, 5 to input Primary Module Function, LFXT.
     */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_PJ,
        GPIO_PIN4 + GPIO_PIN5,
        GPIO_PRIMARY_MODULE_FUNCTION
        );

    // Configure UART pins
    //Set P2.0(tx) and P2.1(rx) as Secondary Module Function Input.
    /*
     * Select Port 2d
     * Set Pin 0, 1 to input Secondary Module Function, (UCA0TXD/UCA0SIMO, UCA0RXD/UCA0SOMI).
     */

    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P2,
        GPIO_PIN0,
        GPIO_SECONDARY_MODULE_FUNCTION
        );

    gpio_as_vref(GPIO_PORT_P1, GPIO_PIN1);
}

void gpio_as_vref(char target_port, char target_pin)
{
    //Set P1.0 as an output pin.
    /*
     * Select Port 1
     * Set Pin 0 as output LED instrcutor for working adc12mem0
     */
    GPIO_setAsOutputPin(
    	target_port,
		GPIO_PIN0
        );

    //Set P1.0 as Output Low.
    /*
     * Select Port 1
     * Set Pin 0 to output Low.
     */
    GPIO_setOutputLowOnPin(
    	target_port,
		GPIO_PIN0
        );
    //Set P1.1 as Ternary Module Function Output.
    /*
     * Select Port 1
     * Set Pin 1 to output Ternary Module Function, (A1, C1, VREF+, VeREF+).
     */
    GPIO_setAsPeripheralModuleFunctionOutputPin(
    	target_port,
		target_pin,
        GPIO_TERNARY_MODULE_FUNCTION
        );
}


void sharpboardInit()
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
void blinkingLED()

{
    PM5CTL0 &= ~LOCKLPM5;       // for FRAM devices

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= 0x01;    // set P1.0 as output
    P4DIR |= 0x40;    // set P4.6 as output

    int z;

    for(z = 7; z>0; z--)
        {

                P1OUT |= 0x01;   //Green
                __delay_cycles(500000);
                P1OUT &= 0xFE;   //off
                __delay_cycles(500000);
        }
    for(z = 5; z>0; z--)
            {

                    P1OUT |= 0x01;   //Green
                    __delay_cycles(200000);
                    P1OUT &= 0xFE;   //off
                    __delay_cycles(200000);
            }
    for(z = 13; z>0; z--)
    {

            P1OUT |= 0x01;   //Green
            __delay_cycles(100000);
            P1OUT &= 0xFE;   //off
            __delay_cycles(100000);
            P1OUT |= 0x01;   //Green
            __delay_cycles(100000);
            P1OUT &= 0xFE;   //off
            __delay_cycles(100000);
            P4OUT |= 0x40;    //Red
            __delay_cycles(100000);
            P4OUT &= ~0x40; // Off
            __delay_cycles(100000);

    }
    for(z = 8; z>0; z--)
        {
                P4OUT |= 0x40;    //Red
                __delay_cycles(100000);
                P4OUT &= ~0x40; // Off
                __delay_cycles(100000);

        }
    for(z = 3; z>0; z--)
        {

                P1OUT |= 0x01;   //Green
                __delay_cycles(100000);
                P1OUT &= 0xFE;   //off
                __delay_cycles(100000);
                P4OUT |= 0x40;    //Red
                __delay_cycles(100000);
                P4OUT &= ~0x40; // Off
                __delay_cycles(100000);
        }
    for(z = 4; z>0; z--)
            {
                    P4OUT |= 0x40;  //Red
                    __delay_cycles(100000);
                    P4OUT &= ~0x40; // Off
                    __delay_cycles(100000);

            }
    for(z = 10; z>0; z--)
               {
                       P1OUT |= 0x01;    //Green
                       __delay_cycles(100000);
                       P1OUT &= ~0x01; // Off
                       __delay_cycles(100000);


               }
    for(z = 10; z>0; z--)
                {
                        P4OUT |= 0x40;    //Red
                        __delay_cycles(100000);
                        P4OUT &= ~0x40; // Off
                        __delay_cycles(100000);

                }
    for(z = 7; z>0; z--)
            {

                    P1OUT |= 0x01;   //Green
                    __delay_cycles(400000);
                    P1OUT &= 0xFE;   //off
                    __delay_cycles(400000);
                    P4OUT |= 0x40;    //Red
                    __delay_cycles(400000);
                    P4OUT &= ~0x40; // Off
                    __delay_cycles(400000);
            }
}



// Second version using GPIO
/*
GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN6);

int z;

    for(z = 7; z>0; z--)
        {

                // P1OUT |= 0x01;
                GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);   //Green
                __delay_cycles(500000);
                //P1OUT &= 0xFE;   //off
                GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
                __delay_cycles(500000);
        }
    for(z = 5; z>0; z--)
            {

                   // P1OUT |= 0x01;   //Green
                    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                    __delay_cycles(200000);
                   // P1OUT &= 0xFE;   //off
                    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
                    __delay_cycles(200000);
            }
    for(z = 13; z>0; z--)
    {

            //P1OUT |= 0x01;   //Green
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
            __delay_cycles(100000);
           // P1OUT &= 0xFE;   //off
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
            __delay_cycles(100000);
            // P1OUT |= 0x01;   //Green
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
            __delay_cycles(100000);
            //P1OUT &= 0xFE;   //off
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
            __delay_cycles(100000);
            // P4OUT |= 0x40;    //Red
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            __delay_cycles(100000);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6); // Off
            __delay_cycles(100000);

    }
    for(z = 8; z>0; z--)
        {
                //P4OUT |= 0x40;    //Red
                GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
                __delay_cycles(100000);
               // P4OUT &= ~0x40; // Off
                GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
                __delay_cycles(100000);

        }
    for(z = 3; z>0; z--)
        {

                //P1OUT |= 0x01;   //Green
                GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                __delay_cycles(100000);
                //P1OUT &= 0xFE;   //off
                GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
                __delay_cycles(100000);
                //P4OUT |= 0x40;    //Red
                GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
                __delay_cycles(100000);
                //P4OUT &= ~0x40; // Off
                GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
                __delay_cycles(100000);
        }
    for(z = 4; z>0; z--)
            {
                  GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);  //Red
                  __delay_cycles(100000);
                  GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6); // Off
                  __delay_cycles(100000);

            }
    for(z = 10; z>0; z--)
               {
                      // P1OUT |= 0x01;    //Green
                      // __delay_cycles(100000);
                      // P1OUT &= ~0x01; // Off
                       //__delay_cycles(100000);
                       GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
                       __delay_cycles(1000000);

               }
    for(z = 10; z>0; z--)
                {
                        //P4OUT |= 0x40;    //Red
                        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
                        __delay_cycles(100000);
                        //P4OUT &= ~0x40; // Off
                        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
                        __delay_cycles(100000);

                }
    for(z = 7; z>0; z--)
            {

                    //P1OUT |= 0x01;   //Green
                GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                __delay_cycles(400000);
                //P1OUT &= 0xFE;   //off
                GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
                __delay_cycles(400000);
                //P4OUT |= 0x40;    //Red
                GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
                __delay_cycles(400000);
                //P4OUT &= ~0x40; // Off
                GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
                __delay_cycles(400000);
            }
}
*/
