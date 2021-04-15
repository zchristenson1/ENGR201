/*******************************************************************************
 *
 * FRAMLogMode.c
 *
 * MSP430FR5969 wakes up every 5 seconds from LPM3.5 to measure and store its
 * internal temperature sensor & battery monitor data to FRAM
 *
 * June 2014
 * E. Chen
 *
 ******************************************************************************/

#pragma once

#include <stdint.h>

#define FRAM_LOG_MODE      '2'
#define TRANSMIT_DATA_MODE '3'

#define FRAM_TEMP_INDEX          0xF000          // FRAM location for Temp data index
#define FRAM_TEMP_RESULTS        0x9000          // FRAM location for Temp Sensor results
#define FRAM_BAT_RESULTS         0xC000          // FRAM location for Battery Monitor results
//#define FRAM_TEMP_RESULTS_END    0xFF7F          // Address following the storage area
#define FRAM_TIME_STAMP          0xF002          // FRAM location for Temp data index

extern uint16_t dataArray[];
extern uint8_t timeStamp[];

void framLog(void);
void transmitFRAMData(void);
void storeTimeStamp(void);
