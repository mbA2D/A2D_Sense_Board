/* A2D Electronics Relay Board Library
 * Written By: Micah Black
 * Date: Dec 9, 2022
 *
 * This class will deal with interfacing with the board
 * 
 */


#ifndef A2D_Relay_Board_h
#define A2D_Relay_Board_h

#include "Arduino.h"
#include <A2D_Relay_Board_V1.0.h> //header file with pins, etc
#include <Wire.h>
#include <TCA9539.h>

class A2D_Relay_Board
{
	public:
		A2D_Relay_Board(); //constructor
		
		//Configuration
		void init();
		void reset();
		
		//Interface
		void set_dig_out(uint8_t channel, bool output_val);
		uint8_t get_num_channels();
		void set_led(bool state);
		void set_i2c_expander_addr(uint8_t addr);
		
	private:
		//************METHODS****************
		bool _valid_channel(uint8_t channel);
		
		//*********VARIABLES/CLASSES*********
		TCA9539 *io;
};

#endif