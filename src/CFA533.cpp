#include "CFA533.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#if (ARDUINO <  100)
   #include <WProgram.h>
#else
   #include <Arduino.h>
#endif

#if defined(__AVR_ATtiny84__) || (__AVR_ATtiny2313__) || defined (__AVR_ATtiny85__)
#include "TinyWireM.h" // include this if ATtiny84 or ATtiny85 or ATtiny2313

#define Wire TinyWireM
#else

#if (ARDUINO < 10000)
   #include <../Wire/Wire.h>
#else
   #include <Wire.h>
#endif

#endif

// CONSTRUCTOR
// ---------------------------------------------------------------------------

CFA533::CFA533( uint8_t I2CAddress )
{
	_address = I2CAddress;
	_row = 0;
	_column = 0;
}

void CFA533::clear()
{
	_row = 0;
	_column = 0;
	_dataOut.command = CFA533_CLEAR;
	_dataOut.dataLength = 0;
	send_verify_return_packet(_dataOut, DELAY_5mSec);
}

void CFA533::setCursor(uint8_t row, uint8_t column)
{
	_dataOut.command = CFA533_CURSORPOSITION;
	_dataOut.dataLength = 2;
	if(row>1)
		row = 1;
	if(column>15)
		column=15;
	_dataOut.data[0] = column;
	_dataOut.data[1] = row;
	send_verify_return_packet(_dataOut, DELAY_5mSec);
}

void CFA533::home()
{
	_row = 0;
	_column = 0;
	setCursor(_row, _column);
}

void CFA533::command(bool location, uint8_t data)
{
	_dataOut.command = CFA533_CONTROLLERCOMMAND;
	_dataOut.dataLength = 2;
	_dataOut.data[0] = location;
	_dataOut.data[1] = data;
	send_verify_return_packet(_dataOut, DELAY_5mSec);
}

void CFA533::resetDisplay()
{
  _dataOut.command = CFA533_REBOOT;
  _dataOut.dataLength = 3;
  _dataOut.data[0] = 8;
  _dataOut.data[1] = 18;
  _dataOut.data[2] = 99;
  send_verify_return_packet(_dataOut, DELAY_500mSec);
  _displaycontrol = 8;
  _displayshift = 16;
}

void CFA533::noDisplay(bool legacy)	
{
	_dataIn = report();
	backlight = _dataIn.data[10];
	kp_backlight = _dataIn.data[14];
	_displaycontrol &= ~CFA533_DISPLAYON;
	command(1, _displaycontrol);
	if(legacy==1)
	{
		setBacklight(0);
		setBacklight_kp(0);
	}
}

void CFA533::display(bool legacy)
{
	_displaycontrol |= CFA533_DISPLAYON;
	command(1, _displaycontrol);
	if(legacy==1)
		setBacklight(backlight, kp_backlight);
}

void CFA533::noBlink()
{
	_displaycontrol &= ~CFA533_BLINKON;
	cursorStyle(_displaycontrol);
}

void CFA533::blink()
{
	_displaycontrol |= CFA533_BLINKON;
	cursorStyle(_displaycontrol);
}

void CFA533::noCursor()
{
	_displaycontrol &= ~CFA533_CURSORON;
	cursorStyle(_displaycontrol);
}

void CFA533::cursor()
{
	_displaycontrol |= CFA533_CURSORON;
	cursorStyle(_displaycontrol);
}

void CFA533::scrollDisplayLeft()
{
	_displayshift = 24;
	command(1, _displayshift);
}

void CFA533::scrollDisplayRight()
{
	_displayshift = 28;
	command(1, _displayshift);
}

commandPacket CFA533::ping(commandPacket transmit)
{
  _dataOut = transmit;
  _dataOut.command = CFA533_PING;
  _dataOut.dataLength = transmit.dataLength;
  return send_verify_return_packet(_dataOut, DELAY_5mSec);
}

commandPacket CFA533::report()
{
  _dataOut.command = CFA533_REPORT;
  _dataOut.dataLength = 0;
  _dataIn = send_verify_return_packet(_dataOut, DELAY_5mSec);
  return _dataIn;
}

uint8_t CFA533::getBacklight_display()
{
  _dataIn = report();
  return _dataIn.data[14];
}

uint8_t CFA533::getBacklight_kp()
{
  _dataIn = report();
  return _dataIn.data[10];
}

/*
void CFA533::setBacklight(uint8_t backlight)
{
	if(backlight > 100)
		backlight = 100;
	_dataOut.data[0] = backlight;
	_dataOut.command = CFA533_BACKLIGHT;
	_dataOut.dataLength = 1;
	send_verify_return_packet(_dataOut, DELAY_50mSec);
}
*/

void CFA533::setBacklight(uint8_t backlight, uint8_t kp_backlight)
{
	if(kp_backlight==101)
		kp_backlight = backlight;
	if(backlight > 100)
		backlight = 100;
	if(kp_backlight > 100)
		kp_backlight = 100;
	_dataOut.data[0] = backlight;
	_dataOut.data[1] = kp_backlight;
	_dataOut.command = CFA533_BACKLIGHT;
	_dataOut.dataLength = 2;
	
	Serial.print(backlight);						// Does not work properly if this is commented out!
	
	send_verify_return_packet(_dataOut, DELAY_50mSec);
}

void CFA533::setBacklight_display(uint8_t backlight)
{
  _dataOut.data[1] = getBacklight_kp();
  _dataOut.command = CFA533_BACKLIGHT;
  _dataOut.dataLength = 2;
  if(backlight > 100)
    backlight = 100;
  _dataOut.data[0] = backlight;
  send_verify_return_packet(_dataOut, DELAY_50mSec);
}

void CFA533::setBacklight_kp(uint8_t backlight)
{
  _dataOut.data[0] = getBacklight_display();
  _dataOut.command = CFA533_BACKLIGHT;
  _dataOut.dataLength = 2;
  if(backlight > 100)
    backlight = 100;
  _dataOut.data[1] = backlight;
  send_verify_return_packet(_dataOut, DELAY_50mSec);
}

void CFA533::cursorStyle(uint8_t style)
{
	_displaycontrol = style & 3;
	_dataOut.data[0] = (_displaycontrol & 3);
	_dataOut.command = CFA533_CURSORSTYLE;
	_dataOut.dataLength = 1;
	send_verify_return_packet(_dataOut, DELAY_5mSec);
}

void CFA533::createChar(uint8_t location, uint8_t charmap[])
{
  _dataOut.command = CFA533_CUSTOMCHARACTER;
  _dataOut.dataLength = 9;
  location &= 0x7; // we only have 8 locations 0-7
  _dataOut.data[0] = location;
  for(uint8_t i = 0; i < 8; i++)
    _dataOut.data[i+1] = charmap[i];
  send_verify_return_packet(_dataOut, DELAY_5mSec);
}

void CFA533::writeFlash(uint8_t data[16])
{
  _dataOut.command = CFA533_WRITEFLASH;
  _dataOut.dataLength = 16;
  for(uint8_t i = 0; i < 16; i++)
    _dataOut.data[i] = data[i];
  send_verify_return_packet(_dataOut, DELAY_25mSec);
}

void CFA533::readFlash(uint8_t data[16])
{
  _dataOut.command = CFA533_READFLASH;
  _dataOut.dataLength = 0;
  _dataIn = send_verify_return_packet(_dataOut, DELAY_5mSec);
  for(uint8_t i = 0; i < 16; i++)
    data[i] = _dataIn.data[i];
}

void CFA533::saveState()
{
  _dataOut.command = CFA533_STORESTATE;
  _dataOut.dataLength = 0;
  send_verify_return_packet(_dataOut, DELAY_50mSec);
}

commandPacket CFA533::version()
{
  _dataOut.command = CFA533_REVISION;
  _dataOut.dataLength = 0;
  return send_verify_return_packet(_dataOut, DELAY_5mSec);
}

void CFA533::setContrast(uint8_t contrast, uint8_t enhanced)
{
	_dataOut.command = CFA533_CONTRAST;
	if(enhanced)
	{
		_dataOut.dataLength = 2;
		if(contrast > 200)
		contrast = 200;
		_dataOut.data[1] = contrast;
	} else
	{
		_dataOut.dataLength = 1;
		if(contrast > 50)
		contrast = 50;
		_dataOut.data[0] = contrast;
	}		
  send_verify_return_packet(_dataOut, DELAY_5mSec);
}

uint8_t CFA533::getContrast()
{
  _dataIn = report();
  return _dataIn.data[9];
}

uint16_t CFA533::rawTemperature(uint8_t sensor)
{
  _dataOut.command = CFA533_READTEMPERATURE;
  _dataOut.data[0] = sensor;
  _dataOut.dataLength = 1;
  int16_t result, highByte;
  uint8_t lowByte;
  uint8_t timeout;
  
  do 
  {
    _dataIn = send_verify_return_packet(_dataOut, DELAY_50mSec);
	timeout++;
	if(timeout>10)
		break;
  } while (_dataIn.data[3] == 0);
  if(timeout>10)
	  return 0;
  highByte = _dataIn.data[2];
  lowByte = _dataIn.data[1];
  result = (highByte << 8) + lowByte;
  return result;
}

float CFA533::temperatureC(uint8_t sensor)
{
  int16_t rawTemp = rawTemperature(sensor);
  float degreesC = float(rawTemp)/16;
  return degreesC;
}

float CFA533::temperatureF(uint8_t sensor)
{
  float rawTemp = temperatureC(sensor);
  float degreesF = (rawTemp * 9)/5 + 32;
  return degreesF;
}

commandPacket CFA533::readLCDMemory(uint8_t address)
{
  _dataOut.command = CFA533_READLCDMEMORY;
  _dataOut.data[0] = address;
  _dataOut.dataLength = 1;
  _dataIn = send_verify_return_packet(_dataOut, DELAY_5mSec);
  return _dataIn;
}

/*
void CFA533::readDOW(uint8_t indexDOW, uint8_t data[])
{
  struct commandPacket _dataIn;
  _dataOut.command = CFA533_READDOW;
  _dataOut.dataLength = 1;
  if(indexDOW > 31)
    indexDOW = 31;
  _dataOut.data[0] = indexDOW;
  _dataIn = send_verify_return_packet(_dataOut, DELAY_5mSec);
  for(uint8_t i = 0; i < 9; i++)
    data[i] = _dataIn.data[i];
}
*/

uint8_t CFA533::getPressed()
{
  uint8_t test = 0;
  _dataOut.command = CFA533_READKEYPAD;
  _dataOut.dataLength = 0;
  _dataIn = send_verify_return_packet(_dataOut, DELAY_5mSec);
  test = _dataIn.data[0];
  if((test & KP_SETUP) == KP_SETUP)
    test ^= 0x40;
  if((test & KP_MFG) == KP_MFG)
    test ^= 0x80;
  if((test & KP_RESET) == KP_RESET)
    resetDisplay();
  return test;
}

uint8_t CFA533::scanKey()
{
  uint8_t test = getPressed();
  uint8_t flag = 1;
  uint8_t result = 0;

  for(int8_t i = 7; i >= 0; i --)
  {
    _keypad[i].pressed = flag * ((test & (0x01 << i)) && 1);
	_keypad[i].timer = _keypad[i].pressed * (_keypad[i].timer + (millis() - _oldMillis));
	if(i > 5)
	  _keypad[i].held = _keypad[i].timer > DEBOUNCE_LONG;
	else
	  _keypad[i].held = _keypad[i].timer > DEBOUNCE_SHORT;
	if(_keypad[i].pressed)
	  flag = 0;
	result = result + (_keypad[i].held * (i + 1));
	if(result)
	  _keypad[i].timer = 0;
  }
  _oldMillis = millis();
  return result;
}

uint8_t CFA533::peekKey()
{
  uint8_t test = getPressed();
  uint8_t flag = 1;
  uint8_t result = 0;

  for(int8_t i = 7; i >= 0; i --)
  {
    _keypad[i].pressed = flag * ((test & (0x01 << i)) && 1);
	_keypad[i].timer = _keypad[i].pressed * (_keypad[i].timer + (millis() - _oldMillis));
	if(i > 5)
	  _keypad[i].held = _keypad[i].timer > DEBOUNCE_LONG;
	else
	  _keypad[i].held = _keypad[i].timer > DEBOUNCE_SHORT;
	if(_keypad[i].pressed)
	  flag = 0;
	result = result + (_keypad[i].held * (i + 1));
  }
  _oldMillis = millis();
  return result;
}

inline size_t CFA533::write(uint8_t value) {
  command(0, value);
  return 1; // assume sucess
}

// Private Methods //////////////////////////////////////////////////////////////

commandPacket CFA533::send_verify_return_packet(commandPacket transmit, uint16_t responseTime)
{
	struct commandPacket response;
	uint16_t notValid;
	while(notValid)
	{
		send_packet(transmit);
		delay(responseTime);
		response = receive_packet();
		notValid = response.crc.as_word ^ (get_crc(response.dataLength+2, (uint8_t *) & response));
	}
	return response;
}

void CFA533::send_packet(commandPacket transmit)
{
	Wire.beginTransmission(_address);
	Wire.write(transmit.command);
	Wire.write(transmit.dataLength);
	for(uint8_t i = 0; i < transmit.dataLength; i++)
		Wire.write(transmit.data[i]);
	transmit.crc.as_word = get_crc(transmit.dataLength+2, (uint8_t *) & transmit);
	Wire.write(transmit.crc.as_bytes[0]);
	Wire.write(transmit.crc.as_bytes[1]);
	Wire.endTransmission();
}

// CFA533::commandPacket CFA533::receive_packet()
commandPacket CFA533::receive_packet()
{
	struct commandPacket response;
	uint8_t requested = MAX_DATA_LENGTH + 4;		// Just request max data, since data gets screwed up otherwise
	Wire.requestFrom(_address, requested);		// Using a variable for requested, otherwise have type issues
	response.command = Wire.read();
	if(MAX_COMMAND<(response.command & 0x3F))
		response.command = 0xC0;
	response.dataLength = Wire.read();
	if(MAX_DATA_LENGTH < response.dataLength)
		response.command = 0xC0;
	if(response.command == 0xC0)
		response.dataLength = 0;
	for(uint8_t i=0; i < response.dataLength ;i++)
		response.data[i] = Wire.read();
	response.crc.as_bytes[0] = Wire.read();
	response.crc.as_bytes[1] = Wire.read();
	return response;
}

  uint16_t CFA533::get_crc(uint8_t count, uint8_t *ptr)
{
	uint16_t crc;				//Calculated CRC
	uint8_t i;				//Loop count, bits in byte
	uint8_t data;				//Current byte being shifted

	crc = 0xFFFF;				// Preset to all 1's, prevent loss of leading zeros
  
	while(count--)
	{
		data = *ptr++;
		i = 8;
		do
		{
			if((crc ^ data) & 0x01)
			{
				crc >>= 1;
				crc ^= 0x8408;
			}
			else
				crc >>= 1;
			data >>= 1;
		} while(--i != 0);
	}
	return (~crc);
}
