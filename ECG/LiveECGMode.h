#pragma once

#include <stdint.h>

#define LIVE_ECG_MODE        '1'

void ECGSensor_init1(int delayCycle);
void liveECG1();

void ECGSensor_init2(int delayCycle);
void liveECG2();

void led2(int led);
