#ifndef _I2C_H_
#define _I2C_H_

#ifndef F_SCL
#define F_SCL 100000
#endif

#include "util/twi.h"

void i2c_init();

void i2c_start(uint8_t adress_rw);
void i2c_stop();

void i2c_write(unsigned char data);
uint8_t i2c_read();

#endif