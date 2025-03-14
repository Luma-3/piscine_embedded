#include "util.h"

#include "i2c.h"

#define  SLA 0x20

void led_init() {
	i2c_start((SLA << 1) | 0);
	i2c_write(0x06);
	i2c_write(0x00);
	i2c_stop();
}

void led_set(uint8_t led, uint8_t state) {
	uint8_t val = 0xff;
	if (!state) {
		val |= BV(led);
	}
	else {
		val &= ~BV(led);
	}

	i2c_start((SLA << 1) | 0);
	i2c_write(0x02);
	i2c_write(val);
	i2c_stop();
}

int main() {
	i2c_init();
	led_init();

	_loop() {
		led_set(3,1);
		_delay_ms(500);
		led_set(3, 0);
		_delay_ms(500);
	}
}