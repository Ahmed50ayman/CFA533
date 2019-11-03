#ifndef CFA533_h
#define CFA533_h

#if (ARDUINO < 10000)
   #include <../Wire/Wire.h>
#else
   #include <Wire.h>
#endif

#include <inttypes.h>
#include <Print.h>

// Default settings for program operation
#define DEFAULT_ADDRESS					0x2A		//Default address of display
#define DEBOUNCE_SHORT					250			// 250mSec debounce of a keypress
#define DEBOUNCE_LONG					5000		// 5000mSec debounce of a keypress
#define MAX_COMMAND						35			// Highest number command allowed for display
#define MAX_DATA_LENGTH					18			// Maximum length of data

// Command set for display
#define CFA533_PING						0x00
#define CFA533_REVISION					0x01
#define	CFA533_WRITEFLASH				0x02
#define CFA533_READFLASH				0x03
#define CFA533_STORESTATE				0x04
#define CFA533_REBOOT					0x05
#define CFA533_CLEAR					0x06
#define CFA533_LINE1					0x07
#define CFA533_LINE2					0x08
#define CFA533_CUSTOMCHARACTER			0x09
#define CFA533_READLCDMEMORY			0x0A
#define CFA533_CURSORPOSITION			0x0B
#define CFA533_CURSORSTYLE				0x0C
#define CFA533_CONTRAST					0x0D
#define CFA533_BACKLIGHT				0x0E
#define CFA533_READTEMPERATURE			0x0F
#define CFA533_READDOW					0x12
#define CFA533_DOWTRANSACTION			0x14
#define CFA533_LIVETEMPERATURE			0x15
#define CFA533_CONTROLLERCOMMAND		0x16
#define CFA533_READKEYPAD				0x18
#define CFA533_SETATXFUNCTION			0x1C
#define CFA533_WATCHDOG					0x1D
#define CFA533_REPORT					0x1E
#define CFA533_PRINT					0x1F
#define CFA533_SETI2CADDRESS			0x21
#define CFA533_SETGPIO					Ox22
#define CFA533_READGPIO					0x23

// Keypad values
#define KP_UP							0x01
#define KP_ENTER						0x02
#define KP_CANCEL						0x04
#define KP_LEFT							0x08
#define KP_RIGHT						0x10
#define KP_DOWN							0x20
#define KP_SETUP						0x30		// Right & Down Arrow combination
#define KP_MFG							0x24		// Down & Cancel combination
#define KP_RESET						0x18		// Right & Left Arrow combination

// flags for function set
#define BUFFER_LENGTH					32
// #define DISPLAY_OFF 8
// #define DISPLAY_ON 12

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define CFA533_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display on/off control
#define CFA533_DISPLAYON 0x04
#define CFA533_DISPLAYOFF 0x00
#define CFA533_CURSORON 0x02
#define CFA533_CURSOROFF 0x00
#define CFA533_BLINKON 0x01
#define CFA533_BLINKOFF 0x00

#define DELAY_5mSec						5
#define DELAY_25mSec					25
#define DELAY_50mSec					50
#define DELAY_500mSec					500
#define DELAY_2Sec						2000
#define REPORT_4						4
#define REPORT_7						7
#define REPORT_19						19
#define REPORT_20						20

union word_or_byte{uint8_t as_bytes[2]; uint16_t as_word;};
struct commandPacket{uint8_t command; uint8_t dataLength; uint8_t data[MAX_DATA_LENGTH]; word_or_byte crc;};

class CFA533 : public Print {
// class CFA533 {
public:
	
	// These commands are to emulate standard LCD library
	CFA533 (uint8_t I2CAddress = DEFAULT_ADDRESS);
	
	// void begin(uint8_t cols = 16, uint8_t rows = 2, uint8_t charsize = 0);
	
	/// Clears the screen and sets cursor to 0, 0
	/*******************************************************************************************************************//**
	 * Empties the contents of the LCD's DDRAM and moves the cursor to the left-most column of the top line.
	 * This function is implemented using the built-in CFA533 command.
	 **********************************************************************************************************************/
	void clear();
	
	/// Sets the cursor to 0, 0
	/*******************************************************************************************************************//**
	* This function is added to give backward compatibility to the standard LCD library. Moves the cursor to home position. 
	***********************************************************************************************************************/
	void home();
	
	/// Turns off the display
	/*******************************************************************************************************************//**
	* This function is added to give backward compatibility to the standard LCD library. It uses direct manipulation
	* of the display register. It's possible that it could corrupt the display as it is not supported by the standard
	* CFA533 commands.
	*
	* + Legacy mode = 0 turns off the display only (default and no argument is required).
	* + Legacy mode = 1 turns off the display and also turns off the backlights.
	***********************************************************************************************************************/
	void noDisplay(bool legacy = 0);
	
	/// Turns on the display
	/*******************************************************************************************************************//**
	* This function is added to give backward compatibility to the standard LCD library. It uses direct manipulation
	* of the display register. It's possible that it could corrupt the display as it is not supported by the standard
	* CFA533 commands.
	*
	* + Legacy mode = 0 turns on the display only (default and no argument is required).
	* + Legacy mode = 1 turns on the display and also turns on the backlights.
	***********************************************************************************************************************/
	void display(bool legacy = 0);
	
	/// Turns off the blinking cursor
	/*******************************************************************************************************************//**
	* This function is added to give backward compatibility to the standard LCD library.
	***********************************************************************************************************************/
	void noBlink();
	
	/// Turns on the blinking cursor
	/*******************************************************************************************************************//**
	* This function is added to give backward compatibility to the standard LCD library.
	***********************************************************************************************************************/
	void blink();
	
	/// Turns off the underline cursor
	/*******************************************************************************************************************//**
	* This function is added to give backward compatibility to the standard LCD library.
	***********************************************************************************************************************/
	void noCursor();
	
	/// Turns on the underline cursor
	/*******************************************************************************************************************//**
	* This function is added to give backward compatibility to the standard LCD library.
	***********************************************************************************************************************/
	void cursor();
	
	/// Scrolls the display left
	/*******************************************************************************************************************//**
	* This function is added to give backward compatibility to the standard LCD library. It uses direct manipulation
	* of the display register. It's possible that it could corrupt the display as it is not supported by the standard
	* CFA533 commands.
	***********************************************************************************************************************/
	void scrollDisplayLeft();
	
	/// Scrolls the display right
	/*******************************************************************************************************************//**
	* This function is added to give backward compatibility to the standard LCD library. It uses direct manipulation
	* of the display register. It's possible that it could corrupt the display as it is not supported by the standard
	* CFA533 commands.
	***********************************************************************************************************************/
	void scrollDisplayRight();
	// void leftToRight();
	// void rightToLeft();
	// void autoscroll();
	// void noAutoscroll();
	
	// void setRowOffsets(int row1, int row2, int row3, int row4);
	
	/// Creates a character
	/*******************************************************************************************************************//**
	* Sets the bitmap for each of the eight special characters.
	* First value is the character index [0-7], second value is an array of 8 bits with the bitmap values for each line.
	***********************************************************************************************************************/
	void createChar(uint8_t, uint8_t[]);
	
	/// Set cursor at desired location
	/*******************************************************************************************************************//**
	* Sets the cursor at the desired location.
	*
	* + row = 0 to 1
	* + column = 0 to 15
	***********************************************************************************************************************/
	void setCursor(uint8_t row, uint8_t column);
	// virtual size_t write(uint8_t);
	
	/// Sends a command directly to the LCD controller
	/*******************************************************************************************************************//**
	* Allows direct manipulation of the HD44780 compatible LCD controller. It is possible to corrupt the display using
	* this command.
	***********************************************************************************************************************/
	void command(bool location, uint8_t data);
	
	// These commands are unique to this library
	
	/// Resets the display to initial state.
	/*******************************************************************************************************************//**
	* The display simmulates a power-on restart.
	***********************************************************************************************************************/
	void resetDisplay();
	
	/// Writes data to reserved flash memory
	/*******************************************************************************************************************//**
	* Writes 16 bytes of data to reserved flash memory. Useful for storing configuration specific data. All 16 bytes
	* are written, there is no option to only write a portion of it.
	***********************************************************************************************************************/
	void writeFlash(uint8_t[16]);
	
	/// Reads data from the reserved flash memory
	/*******************************************************************************************************************//**
	* Reads the 16 bytes from the reserved flash memory and returns it in the supplied array.
	***********************************************************************************************************************/
	void readFlash(uint8_t[16]);
	
	/// Store current state as boot state
	/*******************************************************************************************************************//**
	* Saves characters currently shown on screen, as well as special character definitions, cursor position, cursor
	* style, contrast setting, LCD backlight setting, keypad backlight setting, settings of any live displays, ATX
	* function enable and pulse length, I2C address and GPIO settings.
	***********************************************************************************************************************/
	void saveState();
	// void liveTemperature(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
	
	/// Reads temperature from the specified sensor [0-31]
	/*******************************************************************************************************************//**
	* Returns the raw temperature value for the specified sensor. The sensor must be either a DS1822 or DS12B20.
	***********************************************************************************************************************/
	uint16_t rawTemperature(uint8_t);
	
	/// Reads temperature from the specified sensor [0-31]
	/*******************************************************************************************************************//**
	* Returns the temperature value in degrees Celcius for the specified sensor. The sensor must be either a DS1822
	* or DS12B20.
	***********************************************************************************************************************/
	float temperatureC(uint8_t);
	
	/// Reads temperature from the specified sensor [0-31]
	/*******************************************************************************************************************//**
	* Returns the temperature value in degrees Farenheit for the specified sensor. The sensor must be either a DS1822
	* or DS12B20.
	***********************************************************************************************************************/
	float temperatureF(uint8_t);
	// void scanDOW(uint8_t, uint8_t[]);
	// void writeDOW(uint8_t, uint8_t, uint8_t[]);
	// void readDOW(uint8_t, uint8_t[]);
	
	/// Allows direct setting of the cursor style
	/*******************************************************************************************************************//**
	* Directly sets the cursor style. Four styles are available.
	*
	* + 0 -- no cursor
	* + 1 -- blinking block cursor (equivalent to blink() method
	* + 2 -- underscore cursor (equivalent to cursor() method
	* + 3 -- blinking underscore cursor (equivalent to blink() and cursor() methods both being called
	***********************************************************************************************************************/
	void cursorStyle(uint8_t);
	
	/// Sets the contrast for the display
	/*******************************************************************************************************************//**
	* Sets the contrast. Default is to use enhanced contrast setting and supply only a single value to the command.
	* The lower resolution contrast value may be used by supplying a second value set to 0.
	*
	* enhanced contrast
	* +     0 - 99 = lighter
	* +        100 = no correction
	* +  101 - 200 = darker
	* 
	* standard contrast
	* +          0 = light
	* +         16 = about right
	* +         29 = dark
	* +    30 - 50 = very dark
	***********************************************************************************************************************/
	void setContrast(uint8_t contrast, uint8_t enhanced = 1);
	
	/// Returns the current contrast setting
	/*******************************************************************************************************************//**
	* Reads the contrast value and returns it
	***********************************************************************************************************************/
	uint8_t getContrast();
	
	/// Sets the backlight for the display and keypad
	/*******************************************************************************************************************//**
	* If only one value is supplied, then the display and keypad will both be set to the same value. Otherwise the first
	* value will be the display setting and the second will be the keypad setting. Allowed values are 0 to 100.
	***********************************************************************************************************************/
	void setBacklight(uint8_t backlight, uint8_t kp_backlight = 101);
	
	/// Sets the backlight for the display
	/*******************************************************************************************************************//**
	* Sets the backlight value for the display without changing the value for the keypad.
	***********************************************************************************************************************/
	void setBacklight_display(uint8_t);
	
	/// Sets the backlight for the keypad
	/*******************************************************************************************************************//**
	* Sets the backlight value for the keypad without changing the value for the display.
	***********************************************************************************************************************/
	void setBacklight_kp(uint8_t);
	
	/// Gets the backlight value for the display
	/*******************************************************************************************************************//**
	* Returns the backlight setting of the display.
	***********************************************************************************************************************/
	uint8_t getBacklight_display();
	
	/// Gets the backlight value for the keypad
	/*******************************************************************************************************************//**
	* Returns the backlight setting of the keypad.
	***********************************************************************************************************************/
	uint8_t getBacklight_kp();
	
	uint8_t getPressed();
	uint8_t scanKey();
	uint8_t peekKey();
	
	
	
	/// Pings the display with user supplied data
	/*******************************************************************************************************************//**
	* Pings the display with [0-16] bytes of data. The returned packet is identical to the packet sent, except the
	* type will be 0x40 (normal response, Ping Command).
	***********************************************************************************************************************/
	commandPacket ping(commandPacket);
	
	/// Gets the hardware and firmware version of the display
	/*******************************************************************************************************************//**
	* Returns the hardware and firmware version.
	*
	* Format is data[] = "CFA533:hX.X,cY.Y"
	* + hX.X is the hardware revision.
	* + cY.Y is the firmware version.
	***********************************************************************************************************************/
	commandPacket version();
	
	/// Reads 8 bytes of LCD memory
	/*******************************************************************************************************************//**
	* Returns contents of the LCD's DDRAM or CGROM from the supplied address.
	* + 0x40 to 0x7F for CGROM
	* + 0x80 to 0x8F for DDRAM, line 1
	* + 0xC0 to 0xCF for DDRAM, line 2
	***********************************************************************************************************************/
	commandPacket readLCDMemory(uint8_t);
	// void setATX(commandPacket);
	// void setWatchdog(uint8_t);
	
	/// Gets the backlight value for the keypad
	/*******************************************************************************************************************//**
	* Returns the backlight setting of the keypad.
	***********************************************************************************************************************/
	commandPacket report();
	// void printString(uint8_t, uint8_t, uint8_t[]);
	// void printChar(uint8_t, uint8_t, uint8_t);
	// void setAddress(uint8_t);
	// void setGPIO(uint8_t, uint8_t, uint8_t);
	// uint8_t readGPIO(commandPacket);
	
	virtual size_t write(uint8_t);
	
	using Print::write;
private:
	uint8_t _address, _row, _column, backlight, kp_backlight;
	uint8_t _displaycontrol = 8, _displayshift = 16;
	uint32_t _oldMillis;
	
	struct commandPacket _dataOut, _dataIn;
	struct _key{uint8_t pressed; uint8_t held; uint8_t unsigned long timer;};
	struct _key _keypad[8];
	
	void send_packet(commandPacket);
	commandPacket receive_packet();
	commandPacket send_verify_return_packet(commandPacket, uint16_t);
	uint16_t get_crc(uint8_t, uint8_t[]);
};
#endif
