#include "fr5969rtc.h"

#include "../main.h"
#include "../driverlib/MSP430FR5xx_6xx/driverlib.h"
#include <stdlib.h>
#include <stdio.h>


Calendar calendar;                                // Calendar used for RTC

/*
 * Real Time Clock Initialization
 */
void Init_RTC()
{
    //Setup Current Time for Calendar
    calendar.Seconds    = 0x55;
    calendar.Minutes    = 0x30;
    calendar.Hours      = 0x04;
    calendar.DayOfWeek  = 0x01;
    calendar.DayOfMonth = 0x30;
    calendar.Month      = 0x04;
    calendar.Year       = 0x2014;

    // Initialize RTC with the specified Calendar above
    RTC_B_initCalendar(RTC_B_BASE,
                       &calendar,
                       RTC_B_FORMAT_BCD);

    RTC_B_setCalendarEvent(RTC_B_BASE,
    		               RTC_B_CALENDAREVENT_MINUTECHANGE
    		               );

    RTC_B_clearInterrupt(RTC_B_BASE,
                         RTC_B_TIME_EVENT_INTERRUPT
                         );

    RTC_B_enableInterrupt(RTC_B_BASE,
                          RTC_B_TIME_EVENT_INTERRUPT
                          );

    //Start RTC Clock
    RTC_B_startClock(RTC_B_BASE);
}



