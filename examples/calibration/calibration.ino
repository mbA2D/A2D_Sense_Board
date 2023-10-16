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

void print_existing_calibration() {
	Serial.print(F("v_scaling:"));
	Serial.print(sense_board._v_scaling, 4);
	Serial.print(F("  v_offset:"));
	Serial.print(sense_board._v_offset, 4);
	Serial.print(F("  i_scaling:"));
	Serial.print(sense_board._i_scaling, 4);
	Serial.print(F("  i_offset:"));
	Serial.println(sense_board._i_offset, 4);
}

void setup() {
	// put your setup code here, to run once:
	Serial.begin(BAUDRATE);
	sense_board.init();
	sense_board.set_adc_i2c_addr(I2C_ADDRESS);
  delay(100); //Delay for ADC to reset after address change

	Serial.println(F("Calibration values:"));
	print_existing_calibration();

	Serial.println(F("Would you like to calibrate? (Y or y to calibrate)"));
	while(Serial.available()==0){}
	String response = Serial.readString();
	response.trim();
	if ((response == "y") || (response == "Y"))
	{
		Serial.println(F("Resetting calibration to default:"));
		sense_board.reset_calibration(); //must reset calibration to default before recalibrating so we know the values.
		print_existing_calibration();

		//the ADC's offset is calibrated separately from the voltage and current offsets
		//It does not matter if anything is connected at this point. The ADC's internal mux disconnects the inputs.
		sense_board.calibrate_adc_offset(); 
		
		//Voltage Calibration - point 1
		Serial.println(F("Short the voltage input. Enter the voltage (should be 0) when complete."));
		while (Serial.available()==0)
    {
      Serial.println(sense_board.measure_raw_voltage());
      delay(500);
    }
		float v_1_act = Serial.parseFloat();
		Serial.print(F("Actual voltage entered: "));
		Serial.println(v_1_act, 4);
		float v_1_meas = sense_board.measure_raw_voltage();
		Serial.print(F("Raw measured voltage (at ADC): "));
		Serial.println(v_1_meas, 4);
		while (Serial.available()){Serial.read();}
		Serial.println(F("Measurement complete. Remove the short."));
		//Voltage Calibration - point 2
		Serial.println(F("Put a voltage source on the input and measure it with a calibrated meter. Enter the voltage when complete."));
		while (Serial.available()==0)
    {
      Serial.println(sense_board.measure_raw_voltage());
      delay(500);
    }
		float v_2_act = Serial.parseFloat();
		Serial.print(F("Actual voltage entered: "));
		Serial.println(v_2_act, 4);
		float v_2_meas = sense_board.measure_raw_voltage();
		Serial.print(F("Raw measured voltage (at ADC): "));
		Serial.println(v_2_meas, 4);
		while (Serial.available()){Serial.read();}
		Serial.println(F("Measurement complete. Remove the voltage source."));
		Serial.println(F("Voltage calibration complete."));
		Serial.println();
		
		//Current Calibration - point 1
		Serial.println(F("Ensure no current is flowing through the board. Enter the current (should be 0) when complete."));
		while (Serial.available()==0)
    {
      Serial.println(sense_board.measure_raw_current());
      delay(500);
    }
		float i_1_act = Serial.parseFloat();
		Serial.print(F("Actual current entered: "));
		Serial.println(i_1_act, 4);
		float i_1_meas = sense_board.measure_raw_current();
		Serial.print(F("Raw measured voltage (at ADC): "));
		Serial.println(i_1_meas, 4);
		while (Serial.available()){Serial.read();}
		Serial.println(F("Measurement complete."));
		//Current Calibration - point 2
		Serial.println(F("Allow a current to flow through the board and measure it with a calibrated meter. Enter the current when complete."));
		while (Serial.available()==0)
    {
      Serial.println(sense_board.measure_raw_current());
      delay(500);
    }
		float i_2_act = Serial.parseFloat();
		Serial.print(F("Actual current entered: "));
		Serial.println(i_2_act, 4);
		float i_2_meas = sense_board.measure_raw_current();
		Serial.print(F("Raw measured voltage (at ADC): "));
		Serial.println(i_2_meas, 4);
		while (Serial.available()){Serial.read();}
		Serial.println(F("Measurement complete. Stop the current flow."));
		Serial.println(F("Current calibration complete."));
		Serial.println();
		
		sense_board.calibrate_voltage(v_1_meas, v_1_act, v_2_meas, v_2_act); //Calculate the new voltage calibration
		sense_board.calibrate_current(i_1_meas, i_1_act, i_2_meas, i_2_act); //Calculate the new current calibration
		
		Serial.println(F("New calibration values:"));
		//The calibration will be loaded from EEPROM in sense_board.init()
		sense_board.save_calibration();
		print_existing_calibration();
	}

}

void loop() {

	delay(1000);

}
