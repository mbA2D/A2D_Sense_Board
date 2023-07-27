/*
AUTHOR: Micah Black, A2D Electronics
DATE: Jan 24, 2023
PURPOSE: This example implements some SCPI commands
		(that don't completely follow the susbystem style standard)
		to communicate with the sense board.
CHANGELOG:

*/

#include <A2D_Sense_Board.h>

#define MANUFACTURER  ("A2D Electronics")
#define DESCRIPTION ("Sense Board")
#define VERSION     ("V1.0.0")
#define SERIAL_NUM  ("00001")

//SERIAL DEFINES
#define BAUDRATE    57600
#define SER_BUF_LEN   256
#define CMD_BUF_LEN   32
#define END_CHAR    '\n'
#define NO_CMD      ""

//Macro for finding commands - F to store string literal
//in flash instead of memory
#define CMDIS(i,c) (!strcmp_P(i, PSTR(c)))

//Function Prototypes:
void parse_serial(char ser_buf[], char command[], uint8_t* channel_num, bool* value_bool, int16_t* value_int);

A2D_Sense_Board sense_board;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
  sense_board.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Allocate memory for the serial buffer
  char ser_buf[SER_BUF_LEN];
  char command[CMD_BUF_LEN];
  char* token;
  uint8_t chars_input = 0;
  uint8_t channel_num = 0;
  bool value_bool = 0;
  int16_t value_int = 0;
  
  //if serial data is available
  if(Serial.available()){
	//Read until a full command is received
	chars_input = Serial.readBytesUntil(END_CHAR, ser_buf, SER_BUF_LEN);
	//terminate the input string with NULL
	ser_buf[chars_input] = '\0'; 
	//if(chars_input == 0);//TODO - set a default command to read
	//}
	  parse_serial(ser_buf, command, token, &channel_num, &value_bool, &value_int);
  }
  else{
	strcpy(command, "NOCMD");
  }


  //NOCMD?
  if(CMDIS(command, "NOCMD")){
	;
  }
  
  //*IDN?
  else if(CMDIS(command, "*IDN?")){
	Serial.print(MANUFACTURER);
	Serial.print(",");
	Serial.print(DESCRIPTION);
	Serial.print(",");
	Serial.print(VERSION);
	Serial.print(",");
	Serial.println(SERIAL_NUM);
	Serial.flush();
  }
  
  //*RST
  else if (CMDIS(command, "*RST")){
	sense_board.reset();
  }
  
  //*CLS
  else if (CMDIS(command, "*CLS")){
	; //nothing since we don't have errors yet
  }
  
  //INSTR:SET:LED X VAL  //X is placeholder for channel
  //can be anything, but we need it for the basic parsing currently in use
  //VAL is boolean 0 or 1
  else if (CMDIS(command, "INSTR:SET:LED")){
	sense_board.set_led(value_bool);
  }
  
  //INSTR:SET:ADDR X ADDR  //X is placeholder for channel, ADDR is 7-bit right-justified address
  else if (CMDIS(command, "INSTR:SET:ADDR")){
	sense_board.set_adc_i2c_addr(uint8_t(value_int));
  }

  //MEAS:VOLT:DC?
  else if (CMDIS(command, "MEAS:VOLT:DC?")){
	Serial.println(sense_board.measure_voltage());
	Serial.flush();
  }

  //MEAS:CURR:DC?
  else if (CMDIS(command, "MEAS:CURR:DC?")){
	Serial.println(sense_board.measure_current());
	Serial.flush();
  }

  //MEAS:TEMP_C?
  else if (CMDIS(command, "MEAS:TEMP_C?")){
	Serial.println(sense_board.measure_temperature());
	Serial.flush();
  }
  
  //INSTR:CAL:RESET
  else if (CMDIS(command, "INSTR:CAL:RESET")){
	sense_board.reset_calibration();
  }
  
  //INSTR:CAL:VOLT
  else if (CMDIS(command, "INSTR:CAL:VOLT")){
	//parse all the floats
	char delimeters[] = " ,";
	
	const uint8_t num_floats = 4;
	//create memory to store all the floats
	float float_arr[num_floats];
	for (uint8_t index = 0; index < num_floats, index++)
	{
		token = strtok(NULL, delimeters);
		char str_float[32];
		strcpy(str_float, token);
		float_arr[index] = String(str_float).toFloat();
		Serial.print(float_arr[index]);
		Serial.print(",");
	}
	
	//need to see what format the rest is in.
	Serial.println(ser_buf);
	Serial.flush();
	
	//sense_board.calibrate_voltage(float_arr[0], float_arr[1], float_arr[2], float_arr[3]);
  }
  
}

void parse_serial(char ser_buf[], char command[], char *token, uint8_t *channel_num, bool *value_bool, int16_t *value_int)
{
  //All SCPI commands are terminated with newline '/n'
  //but the Serial.readBytesUntil discards the terminator
  //so do we need to add one to use strcmp?
  
  //we will assume only 1 command is sent at a time
  //so we don't have to deal with SCPI's ';' to send
  //multiple commands on the same line
  
  //split input string on space to extract the command
  //and the parameters
  //strtok replaces the delimeter with NULL to terminate the string
  //strtok maintains a static pointer to the original string passed to it.
  //to get the next token, pass NULL as the first argument.
  
  //we need to be able to pass 4 floats over serial for the calibration commands.
  
  
  char delimeters[] = " ,";
  //char* token;
  char channel_str[8];
  char value_str[8];
  
  token = strtok(ser_buf, delimeters);
  strcpy(command, token); //copies the token into command.
  
  if (CMDIS(command, "INSTR:CAL:VOLT")) //INSTR:CAL:VOLT 123.123,123.123,123.123,123.123
  {
	  //now we want to parse 4 floats separated by commas
	  //do the rest of the parsing in the function above.
  }
  else
  {
	  token = strtok(NULL, delimeters);
	  strcpy(channel_str, token);
	  
	  token = strtok(NULL, delimeters);
	  strcpy(value_str, token);

	  token = strtok(value_str, delimeters);
	  *value_int = atoi(token);
	  *value_bool = 0;
	  if(*value_int >= 1)
		*value_bool = 1;
	  
	  strcpy(delimeters, "@)");
	  token = strtok(channel_str, delimeters); //get rid of "(@"
	  token = strtok(NULL, delimeters);
	  *channel_num = atoi(token);
	  if(*channel_num > 63)
		*channel_num = 63;
	  else if(*channel_num < 0)
		*channel_num = 0;
  }
}
