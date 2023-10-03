/* A2D Electronics Sense Board Module Library
 * Written By: Micah Black
 * Date: Jan 24, 2023
 *
 * UPDATE Jun 29, 2023: Added calibration commands and storing calibration to EEPROM
 *
 *
 */

#include <A2D_Sense_Board.h>
#include <ADS1219.h>

//constructor and initialization list
A2D_Sense_Board::A2D_Sense_Board()
{	
	if (TWCR == 0) 
	{
		Wire.begin();
	}
	Wire.setWireTimeout(3000, true);
	
	_adc = new ADS1219();
	_v_ref = A2D_SENSE_BOARD_DEFAULT_V_REF;
	_i_scaling = A2D_SENSE_BOARD_I_SCALING;
	_v_scaling = A2D_SENSE_BOARD_V_SCALING;
	_t_scaling = A2D_SENSE_BOARD_T_SCALING;
	_t_current_source = A2D_SENSE_BOARD_T_I_SOURCE_A;
	
	_v_offset = A2D_SENSE_BOARD_DEFAULT_V_OFFSET;
	_i_offset = A2D_SENSE_BOARD_DEFAULT_I_OFFSET;
	
	
	_ee_addr_initialized = 0; //2
	_ee_addr_serial = _ee_addr_initialized + sizeof(int); //4
	_ee_addr_v_off = _ee_addr_serial + sizeof(uint32_t); //4
	_ee_addr_i_off = _ee_addr_v_off + sizeof(float); //4
	_ee_addr_v_scale = _ee_addr_i_off + sizeof(float); //4
	_ee_addr_i_scale = _ee_addr_v_scale + sizeof(float); //4 //total 22 bytes
	
	_sh_a = A2D_SENSE_BOARD_DEFAULT_SH_A;
	_sh_b = A2D_SENSE_BOARD_DEFAULT_SH_B;
	_sh_c = A2D_SENSE_BOARD_DEFAULT_SH_C;
	
}

void A2D_Sense_Board::init()
{
	pinMode(A2D_SENSE_BOARD_LED_PIN, OUTPUT);
	_adc->init(A2D_SENSE_BOARD_MIN_I2C_ADDR);
	
	ads1219_conf_reg adc_conf_reg;
	adc_conf_reg.bits.mux = A2D_SENSE_BOARD_MUX_VOLTAGE;
	adc_conf_reg.bits.gain = ADS1219_GAIN_1;
	adc_conf_reg.bits.dr = ADS1219_DR_20SPS;
	adc_conf_reg.bits.cm = ADS1219_CM_SINGLE_SHOT;
	adc_conf_reg.bits.vref = ADS1219_VREF_EXTERNAL;
	
	
	_adc->set_conf_reg(adc_conf_reg.conf_byte);
	_adc->set_ext_ref_v_for_calc(_v_ref);
	calibrate_adc_offset();
	_init_cal_from_eeprom();
	
}

void A2D_Sense_Board::reset()
{
	//LED
	digitalWrite(A2D_SENSE_BOARD_LED_PIN, LOW);
	
	_adc->reset();
}

float A2D_Sense_Board::measure_temperature()
{
	_adc->set_mux(A2D_SENSE_BOARD_MUX_TEMP);
	float voltage = _adc->measure_voltage();
	return _convert_adc_voltage_to_temperature(voltage);
}

float A2D_Sense_Board::measure_current()
{
	_adc->set_mux(A2D_SENSE_BOARD_MUX_CURRENT);
	float voltage = _adc->measure_voltage();
	return _convert_adc_voltage_to_current(voltage);
}

float A2D_Sense_Board::measure_voltage()
{
	_adc->set_mux(A2D_SENSE_BOARD_MUX_VOLTAGE);
	float voltage = _adc->measure_voltage();
	return _convert_adc_voltage_to_voltage(voltage);
}

void A2D_Sense_Board::calibrate_adc_offset()
{
	_adc->calibrate_offset();
}

void A2D_Sense_Board::calibrate_current(float p1_meas, float p1_act, float p2_meas, float p2_act)
{
	//calculate new offset (b) and scaling (m) in:  actual = m * measured + b
	_i_scaling = (p2_act - p1_act) / (p2_meas - p1_meas); //rise in actual / run in measured
	_i_offset = p2_act - _i_scaling * p2_meas; //b = actual - m * measured
}

void A2D_Sense_Board::calibrate_voltage(float p1_meas, float p1_act, float p2_meas, float p2_act)
{
	//calculate new offset (b) and scaling (m) in:  actual = m * measured + b
	_v_scaling = (p2_act - p1_act) / (p2_meas - p1_meas); //rise in actual / run in measured
	_v_offset = p2_act - _i_scaling * p2_meas; //b = actual - m * measured
}

void A2D_Sense_Board::_init_cal_from_eeprom()
{
	//update all calibration values from EEPROM
	EEPROM.get(_ee_addr_v_off, _v_offset);
	EEPROM.get(_ee_addr_i_off, _i_offset);
	EEPROM.get(_ee_addr_v_scale, _v_scaling);
	EEPROM.get(_ee_addr_i_scale, _i_scaling);
}

//resets to the default calibration - assumes all components have 0% tolerance.
void A2D_Sense_Board::reset_calibration()
{
	reset_current_calibration();
	reset_voltage_calibration();
}

void A2D_Sense_Board::reset_voltage_calibration()
{
	_v_offset = A2D_SENSE_BOARD_DEFAULT_V_OFFSET;
	_v_scaling = A2D_SENSE_BOARD_V_SCALING;
}

void A2D_Sense_Board::reset_current_calibration()
{
	_i_offset = A2D_SENSE_BOARD_DEFAULT_I_OFFSET;
	_i_scaling = A2D_SENSE_BOARD_I_SCALING;
}

void A2D_Sense_Board::save_calibration()
{
	EEPROM.put(_ee_addr_v_off, _v_offset);
	EEPROM.put(_ee_addr_v_scale, _v_scaling);
	
	EEPROM.put(_ee_addr_i_off, _i_offset);
	EEPROM.put(_ee_addr_i_scale, _i_scaling);
}

void A2D_Sense_Board::set_led(bool state)
{
	digitalWrite(A2D_SENSE_BOARD_LED_PIN, state);
}

void A2D_Sense_Board::set_adc_i2c_addr(uint8_t addr)
{
	if (addr >= A2D_SENSE_BOARD_MIN_I2C_ADDR && addr <= A2D_SENSE_BOARD_MAX_I2C_ADDR)
	{
		_adc->init(addr);
	}
	reset();
}

void A2D_Sense_Board::set_sh_constants(float sh_a, float sh_b, float sh_c)
{
	_sh_a = sh_a;
	_sh_b = sh_b;
	_sh_c = sh_c;
}

float A2D_Sense_Board::_convert_adc_voltage_to_current(float voltage)
{
	return (voltage - _i_offset) * _i_scaling;
}

float A2D_Sense_Board::_convert_adc_voltage_to_temperature(float voltage)
{
	float voltage_corrected = voltage * _t_scaling;
	//assume we are using the built-in current source (22.5uA)
	float resistance = voltage_corrected / _t_current_source;
	float temperature_c = (1.0/(_sh_a + _sh_b * log(resistance) + _sh_c * pow(log(resistance),3.0))) - 273.15;
	return temperature_c;
}

float A2D_Sense_Board::_convert_adc_voltage_to_voltage(float voltage)
{
	return (voltage - _v_offset) * _v_scaling;	
}
