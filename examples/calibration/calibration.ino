/*
AUTHOR: Micah Black, A2D Electronics
DATE: Jul 20, 2023
PURPOSE: This example recalibrates the voltage and current gain and offsets.
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
	
	sense_board.reset_calibration(); //must reset calibration to default before recalibrating.
	
	//the ADC's offset is calibrated separately from the voltage and current offsets
	//It does not matter if anything is connected at this point. The ADC's internal mux disconnects the inputs.
	sense_board.calibrate_adc_offset(); 
	
	Serial.println("Short the voltage input. Enter the voltage (should be 0) when complete.");
	float v_1_act = Serial.parseFloat();
	float v_1_meas = sense_board.measure_voltage();
	Serial.println("Measurement complete. Remove the short.");
	
	Serial.println("Put a voltage source on the input and measure it with a calibrated meter. Enter the voltage when complete.");
	float v_2_act = Serial.parseFloat();
	float v_2_meas = sense_board.measure_voltage();
	Serial.println("Measurement complete. Remove the voltage source.");
	
	//Calculate the new calibration and store it in EEPROM
	sense_board.calibrate_voltage(v_1_meas, v_1_act, v_2_meas, v_2_act);
	Serial.println("Voltage calibration complete.");
	Serial.println();
	
	Serial.println("Ensure no current is flowing through the board. Enter the current (should be 0) when complete.");
	float i_1_act = Serial.parseFloat();
	float i_1_meas = sense_board.measure_current();
	Serial.println("Measurement complete.");
	
	Serial.println("Allow a current to flow through the board and measure it with a calibrated meter. Enter the current when complete.");
	float i_2_act = Serial.parseFloat();
	float i_2_meas = sense_board.measure_current();
	Serial.println("Measurement complete. Stop the current flow.");
	
	//Calculate the new calibration and store it in EEPROM
	sense_board.calibrate_current(i_1_meas, i_1_act, i_2_meas, i_2_act);
	Serial.println("Current calibration complete.");
	Serial.println();
	
	//The calibration will be loaded from EEPROM in sense_board.init()
	
}

void loop() {

	delay(1000);

}
