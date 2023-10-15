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
#include <EEPROM.h>

class A2D_Sense_Board
{
	public:
		A2D_Sense_Board(); //constructor
		
		
		//scaling for voltages
		float _v_scaling; // V/V
		float _i_scaling; // A/V
		float _t_scaling; // V/V
		float _t_current_source; //A
		
		float _v_offset; //V
		float _i_offset; //A
		
		
		//Configuration
		void init();
		void reset();
		
		//Interface
		float measure_temperature();
		float measure_current();
		float measure_voltage();
		float measure_raw_temperature();
		float measure_raw_current();
		float measure_raw_voltage();
		void calibrate_adc_offset();
		void calibrate_current(float p1_meas, float p1_act, float p2_meas, float p2_act);
		void calibrate_voltage(float p1_meas, float p1_act, float p2_meas, float p2_act);
		void reset_calibration();
		void reset_voltage_calibration();
		void reset_current_calibration();
		void save_calibration();
		void set_led(bool state);
		void set_adc_i2c_addr(uint8_t addr);
		void set_sh_constants(float sh_a, float sh_b, float sh_c);

	private:
		//************METHODS****************
		float _convert_adc_voltage_to_current(float voltage);
		float _convert_adc_voltage_to_temperature(float voltage);
		float _convert_adc_voltage_to_voltage(float voltage);
		void _init_cal_from_eeprom();
		
		//*********VARIABLES/CLASSES*********
		float _v_ref;

		
		uint32_t _serial;
		int _ee_initialized;
		
		int _ee_addr_initialized;
		int _ee_addr_serial;
		int _ee_addr_v_off;
		int _ee_addr_i_off;
		int _ee_addr_v_scale;
		int _ee_addr_i_scale;
		
		
		//steinhart-hart constants for 10K NTC
		float _sh_a;
		float _sh_b;
		float _sh_c;

		ADS1219 *_adc;
};

#endif