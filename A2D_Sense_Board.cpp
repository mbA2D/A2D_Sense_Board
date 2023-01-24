/* A2D Electronics Relay Board Module Library
 * Written By: Micah Black
 * Date: Jan 22, 2021
 *
 *
 *
 *
 */

#include <A2D_Relay_Board.h>

//constructor and initialization list
A2D_Relay_Board::A2D_Relay_Board()
{	
	if (TWCR == 0) Wire.begin();
	
	
}

void A2D_Relay_Board::init()
{
	io = new TCA9539();
	io->TCA9539_init(A2D_RELAY_BOARD_IO_EXP_ADDR_DEFAULT);
	
	reset();
}

void A2D_Relay_Board::set_i2c_expander_addr(uint8_t addr)
{
	if (addr > A2D_RELAY_BOARD_IO_EXP_ADDR_MIN && addr < A2D_RELAY_BOARD_IO_EXP_ADDR_MAX)
	io->TCA9539_init(addr);

	reset();
}

void A2D_Relay_Board::reset()
{
	//LED
	pinMode(A2D_RELAY_BOARD_LED_PIN, OUTPUT);
	digitalWrite(A2D_RELAY_BOARD_LED_PIN, LOW);
	
	io->TCA9539_init();
	
	//Channels off
	io->TCA9539_set_pin_val(A2D_RELAY_BOARD_CH0_PIN, TCA9539_PIN_OUT_LOW);
	io->TCA9539_set_pin_val(A2D_RELAY_BOARD_CH1_PIN, TCA9539_PIN_OUT_LOW);
	io->TCA9539_set_pin_val(A2D_RELAY_BOARD_CH2_PIN, TCA9539_PIN_OUT_LOW);
	
	//Channels pin Mode
	io->TCA9539_set_dir(A2D_RELAY_BOARD_CH0_PIN, TCA9539_PIN_DIR_OUTPUT);
	io->TCA9539_set_dir(A2D_RELAY_BOARD_CH1_PIN, TCA9539_PIN_DIR_OUTPUT);
	io->TCA9539_set_dir(A2D_RELAY_BOARD_CH2_PIN, TCA9539_PIN_DIR_OUTPUT);
	
}

void A2D_Relay_Board::set_led(bool state)
{
	digitalWrite(A2D_RELAY_BOARD_LED_PIN, state);
}

void A2D_Relay_Board::set_dig_out(uint8_t channel, bool output_val)
{
	//check valid channel
	if (!_valid_channel(channel)) return;
	
	TCA9539_pin_val_t val_to_write = TCA9539_PIN_OUT_LOW;
	if (output_val) val_to_write = TCA9539_PIN_OUT_HIGH;
	
	if (channel == 0) io->TCA9539_set_pin_val(A2D_RELAY_BOARD_CH0_PIN, val_to_write);
	if (channel == 1) io->TCA9539_set_pin_val(A2D_RELAY_BOARD_CH1_PIN, val_to_write);
	if (channel == 2) io->TCA9539_set_pin_val(A2D_RELAY_BOARD_CH2_PIN, val_to_write);
}

uint8_t A2D_Relay_Board::get_num_channels()
{
	return A2D_RELAY_BOARD_NUM_CHANNELS;
}

bool A2D_Relay_Board::_valid_channel(uint8_t channel)
{
	return (channel < A2D_RELAY_BOARD_NUM_CHANNELS);
}

//TODO - add a command to change the I2C Address of the IO expander.
//I don't want to have to recompile firmware whenever a jumper is changed.
//change I2C addr, reset expander and re-initialize

