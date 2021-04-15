#pragma once

#include <stdint.h>

#define CPU_CLOCK (1000000)

#if CPU_CLOCK == 1000000
	#define delay_us(us)    __delay_cycles(1*(us))
	#define delay_ms(ms)    __delay_cycles(1000*(ms))
#else
	#pragma error "CPU_CLOCK is defined implicitly!"
#endif

void clock_init();
void sharplcd_clock_Init();



