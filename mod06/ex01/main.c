#include "util.h"
#include "uart.h"

#include "util/twi.h"

#define F_SCL 100000

#define SLA 0x38

void i2c_init() {
	TWBR = ((F_CPU / F_SCL) - 16) / 2; // calcule for clock speed find in (p-180)
	TWCR = BV(TWEN); // Enable i2c
}

void i2c_start(uint8_t adress_rw) {
	/**
	 * TWINT (Interupt Flag (ACK-NACK)) 
	 * TWEM Enable i2c
	 * TWSTA for send bit start
	 */
	TWCR  = BV(TWINT) | BV(TWSTA) | BV(TWEN); 
	while (!(TWCR & BV(TWINT))); // Wait TWINT flag is up

	/**
	 *  Send 0x38 (slave adress of  AHT20 (p-12 in https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf))
	 *  bit shift 1 to keep space for READ/WRITE bit
	 */
	TWDR  = adress_rw; 

	/**
	 * TWINT Clear TWINT flag for continue
	 * TWEM Enable i2c
	 */
	TWCR = BV(TWINT) | BV(TWEN);
	while (!(TWCR & BV(TWINT))); // WAIT ACK

}

void i2c_stop() {
	TWCR = BV(TWINT) |  BV(TWEN) | BV(TWSTO); // TWSTO for send STOP condition
	while ((TWCR) & BV(TWSTO)); // Wait STOP bit is send
}

void i2c_write(unsigned char data) {
	TWDR = data; // Transmite data
	TWCR = BV(TWINT) | BV(TWEN); // Clear TWINT flag for continue
	while (!(TWCR & BV(TWINT))); // Wait ACK
}

uint8_t i2c_read() {
	TWCR = BV(TWINT) | BV(TWEN) | BV(TWEA); // TWEA for active ACK
	while (!(TWCR & BV(TWINT)));
	return TWDR;
}

void print_hex_value(char c) {
	uart_print_hex(c);
	uart_tx(' ');
}

uint8_t check_calibration() {
	uint8_t val;

	i2c_start((SLA << 1) | 0);
	i2c_write(0x71);

	i2c_start((SLA << 1) | 1);
	val = i2c_read();

	return val & (1 << 3);
}

void start_calibration() {
	i2c_start((SLA << 1) | 0);
	i2c_write(0xBE);
	i2c_write(0x08);
	i2c_write(0x00);
	_delay_ms(10);
}

void start_AHT() {
	i2c_start((SLA << 1) | 0);
	i2c_write(0xAC);
	i2c_write(0x33);
	i2c_write(0x00);
	_delay_ms(80);
}

uint8_t wait_data_AHT() {
	uint8_t val;

	do {
		i2c_start((SLA << 1) | 0);
		i2c_write(0x71);
		i2c_start((SLA << 1) | 1);
		val = i2c_read();
	}
	while ((val & (1 << 7)));
}

int main() {
	uart_init(UBRR);
	i2c_init();

	_delay_ms(40);

	if (check_calibration() != 1) start_calibration();

	_loop() {
		start_AHT();
		wait_data_AHT();
		i2c_start((SLA << 1) | 1);
		for (uint8_t  i = 0; i <= 6 ;i++) {
			print_hex_value(i2c_read());
		}
		uart_printstr("\r\n");
	}
}