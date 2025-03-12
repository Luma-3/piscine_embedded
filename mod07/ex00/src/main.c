#include "util.h"
#include "uart.h"
#include "i2c.h"

#include <stdlib.h>

#define SLA 0x38

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

float humidity_percent(uint32_t val) {
	return ((val / 1048576.0) * 100.0); // calcule in AHT20 doc
}

float temperature_celsius(uint32_t val) {
	return (((val / 1048576.0) * 200.0) - 50.0); // calcule in AHT20 doc
}

void read_AHT(float *temp, float *hum) {

	float sum_temp = 0.0;
	float sum_hum = 0.0;
	uint32_t tmp_hum, tmp_temp;


	for (uint8_t i = 3; i; i--) {
		start_AHT();
		wait_data_AHT();
		i2c_start((SLA << 1) | 1);

		i2c_read();

		//get 20bit value
		uint8_t high_hum = i2c_read();
		uint8_t mid_hum = i2c_read();
		uint8_t low_hum = i2c_read();
		
		tmp_hum = ((uint32_t)high_hum << 12) | ((uint32_t)mid_hum << 4) | (low_hum >> 4);
		
		//get 20bit value
		uint8_t hidh_temp = low_hum & 0x0F;  // Sauvegarde du dernier octet
		uint8_t mid_temp = i2c_read();
		uint8_t low_temp = i2c_read();
		
		tmp_temp = ((uint32_t)hidh_temp << 16) | ((uint32_t)mid_temp << 8) | low_temp;
		i2c_read();

		sum_hum += humidity_percent(tmp_hum);
		sum_temp += temperature_celsius(tmp_temp);

	}
	*temp = sum_temp / 3; //average
	*hum = sum_hum / 3;
}

int main() {
	uart_init(UBRR);
	i2c_init();

	_delay_ms(40);

	if (check_calibration() != 1) start_calibration();

	float temp, hum;

	_loop() {
		char buffer[10];
		read_AHT(&temp, &hum);
		dtostrf(temp, 6, 2, buffer);

		uart_printstr("\b\rTemperature: ");
		uart_printstr(buffer);
		uart_printstr("°C, Humidity: ");

		dtostrf(hum, 6, 3, buffer);
		uart_printstr(buffer);
		uart_printstr("%\r\n");
	}
}