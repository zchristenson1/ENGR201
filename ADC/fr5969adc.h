#pragma once

void ADCEX3(void);
void adcex1(void);

void adc_init(int inputSelect, int channelMap, int vrefSource, int clkSource);
void temp_sensor_adc_init();
void adc_mem0_clear();
void adc_mem0_int_enable();

