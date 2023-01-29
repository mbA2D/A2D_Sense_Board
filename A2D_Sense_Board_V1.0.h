//Board Description File for A2D_Sense_Board_V1.0 Board

#include "ADS1219.h"
#include "Arduino.h"

//MUX settings
#define A2D_SENSE_BOARD_MUX_CURRENT		ADS1219_MUX_P_AIN0_N_AIN1
#define A2D_SENSE_BOARD_MUX_VOLTAGE		ADS1219_MUX_P_AIN2_N_AGND
#define A2D_SENSE_BOARD_MUX_TEMP		ADS1219_MUX_P_AIN3_N_AGND

//Valid I2C Address
#define A2D_SENSE_BOARD_MIN_I2C_ADDR	0x40
#define A2D_SENSE_BOARD_MAX_I2C_ADDR	0x4F

#define A2D_SENSE_BOARD_LED_PIN         LED_BUILTIN

//scaling
#define A2D_SENSE_BOARD_V_SCALING       10.35 // (18.7k + 2k)/2k
#define A2D_SENSE_BOARD_I_SCALING       10.0 //2mOhm and 50V/V amp
#define A2D_SENSE_BOARD_T_SCALING       1.0 //the voltage scaling on the temp input
#define A2D_SENSE_BOARD_T_I_SOURCE_A    0.0000225 //22.5uA
