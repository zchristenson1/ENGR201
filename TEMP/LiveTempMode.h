#pragma once

#include <stdint.h>

#define LIVE_TEMP_MODE        '1'

void TempSensor_init(int adcInput, int delayCycle);
void liveTemp();

