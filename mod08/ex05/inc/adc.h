#ifndef _ADC_H_
#define _ADC_H_

#include <avr/io.h>

#include "util.h"

void adc_init_8b();
void adc_init_10b();

uint8_t adc_read_8b(uint8_t channel);
uint16_t adc_read_10b(uint8_t channel);

#endif // _ADC_H_