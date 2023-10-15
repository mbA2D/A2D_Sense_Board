/*
AUTHOR: Micah Black, A2D Electronics
DATE: Jul 20, 2023
PURPOSE: This example prints out the voltage, current, and temperature measured by the board every 1s.
CHANGELOG:

*/

#include <A2D_Sense_Board.h>

#define BAUDRATE    57600
#define I2C_ADDRESS 0x40

A2D_Sense_Board sense_board;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(BAUDRATE);
	sense_board.init();
	
	sense_board.set_adc_i2c_addr(I2C_ADDRESS);
  
	delay(2000);
 
}

void loop() {

	Serial.print("V: ");
	Serial.print(sense_board.measure_raw_voltage());
	
	Serial.print(" I: ");
	Serial.print(sense_board.measure_raw_current());
	
	Serial.print(" T: ");
	Serial.println(sense_board.measure_raw_temperature());
	Serial.flush();

	delay(250);

}
