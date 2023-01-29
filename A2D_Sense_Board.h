/* A2D Electronics Relay Board Library
 * Written By: Micah Black
 * Date: Dec 9, 2022
 *
 * This class will deal with interfacing with the board
 * 
 */


#ifndef A2D_Sense_Board_h
#define A2D_Sense_Board_h

#include "Arduino.h"
#include <A2D_Sense_Board_V1.0.h> //header file with pins, etc
#include <Wire.h>
#include <ADS1219.h>

class A2D_Sense_Board
{
	public:
		A2D_Sense_Board(); //constructor
		
		//Configuration
		void init();
		void reset();
		
		//Interface
		float measure_temperature();
		float measure_current();
		float measure_voltage();
		void calibrate_adc_offset(); //different calibration for each channel?
		void calibrate_adc_gain(float input_voltage);
		void set_led(bool state);
		void set_adc_i2c_addr(uint8_t addr);
		void set_sh_constants(float sh_a, float sh_b, float sh_c);

	private:
		//************METHODS****************
		float _convert_adc_voltage_to_current(float voltage);
		float _convert_adc_voltage_to_temperature(float voltage);
		float _convert_adc_voltage_to_voltage(float voltage);

		//*********VARIABLES/CLASSES*********
		float _v_ref;

		//scaling for voltages
		float _v_scaling; // V/V
		float _i_scaling; // A/V
		float _t_scaling; // V/V
		float _t_current_source; //A
		
		//steinhart-hart constants for 10K NTC
		float _sh_a;
		float _sh_b;
		float _sh_c;

		ADS1219 *_adc;
};

#endif