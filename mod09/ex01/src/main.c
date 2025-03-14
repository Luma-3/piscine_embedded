#include "util.h"

#include "i2c.h"

#define  SLA 0x20

void send_ddr(uint8_t val) {
	i2c_start((SLA << 1) | 0);
	i2c_write(0x06);
	i2c_write(val);
	i2c_stop();
}

void set_PB(uint8_t val) {
	i2c_start((SLA << 1) | 0);
	i2c_write(0x02);
	i2c_write(val);
	i2c_stop();
}

uint8_t read_PB() {
	uint8_t value;
	i2c_start((SLA << 1) | 0);
	i2c_write(0x00);
	
	i2c_start((SLA << 1) | 1);
	value = i2c_read_nack();
	i2c_stop();

	return value;
}

int main() {
	i2c_init();
	send_ddr(0b00000001);

	uint8_t val = 0;
	uint8_t last_button_state = 0;

	_loop() {
		uint8_t button_state = read_PB() & BV(0);
		if (!button_state && last_button_state) {
			val = (val + 1) % 8;
		}
		last_button_state = button_state;

		set_PB(
			~(val << 1 | 1)
		);
		_delay_ms(50); 
	}
}