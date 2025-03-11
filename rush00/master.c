#include <util/twi.h>
#include <util/delay.h>

#define SLA 0x20

void init_I2C() {
	/** TWEN for enablue I2C
	 *  TWINT for clear TWINT flag
	 * 	TWSTA for transmit START condition 
	 */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while(!(TWCR & ))

}


int main() {


}
