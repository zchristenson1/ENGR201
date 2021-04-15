#ifndef OUTOFBOX_FR5969_NEWD_MAIN_H_
#define OUTOFBOX_FR5969_NEWD_MAIN_H_

#define CAL_ADC_12T30_L  *(int8_t *)(0x1A1E) // Temperature Sensor Calibration-30 C 2.0V ref
#define CAL_ADC_12T30_H  *(int8_t *)(0x1A1F)
#define CAL_ADC_12T85_L  *(int8_t *)(0x1A20) // Temperature Sensor Calibration-85 C 2.0V ref
#define CAL_ADC_12T85_H  *(int8_t *)(0x1A21)

extern int mode;
extern int pingHost;

#endif /* OUTOFBOX_FR5969_NEWD_MAIN_H_ */
