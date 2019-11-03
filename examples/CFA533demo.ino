/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:

#include <Wire.h>
#include <CFA533.h>

// #include <LiquidCrystal.h>
#define wait    2000
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
// const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
// LiquidCrystal lcd2(rs, en, d4, d5, d6, d7);

CFA533 lcd2;
commandPacket test, test2;

void setup() {
  
  Wire.begin();
  Serial.begin(9600);


  // set up the LCD's number of columns and rows:
  // lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd2.print("hello, world!");
  lcd2.resetDisplay();
  pinMode( 13, OUTPUT );
}

void loop() {
  uint8_t contrast, backlight, kp_backlight;
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  // lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  // lcd.print(millis() / 1000);
  lcd2.print("||||+||||1||||+||||2||||+||||3||||+||||4||||+||||5||||+||||6||||+||||7||||+||||8");
  Serial.println("CFA533 Library Demo Begins...");
  delay(wait);
  lcd2.noDisplay();
  Serial.println("No display, backlight on.");
  delay(wait);

  lcd2.display();
  Serial.println("Display back on.");
  delay(wait);
  
  lcd2.noDisplay(1);
  Serial.println("No display & no backlight.");
  delay(wait);

  lcd2.display(1);
  Serial.println("Display back on with backlight.");
  delay(wait);
  lcd2.cursor();
  Serial.println("Cursor");
  delay(wait);
  lcd2.blink();
  Serial.println("Cursor & blink");
  delay(wait);
  lcd2.noCursor();
  Serial.println("Blink");
  delay(wait);
  lcd2.noBlink();
  Serial.println("Blink and cursor both off again");
  delay(wait);

  for(int j = 1; j < 3; j++)
  {
    Serial.print("Loop: ");
    Serial.println(j);
    Serial.println("Scroll Left");
    for(int i = 0; i < 40 ; i++)
    {
      lcd2.scrollDisplayLeft();
      delay(50);
    }
    Serial.println("Scroll Right");
    for(int k = 0; k < 40 ; k++)
    {
      lcd2.scrollDisplayRight();
      delay(50);
    }
  }
  contrast = lcd2.getContrast();
  for(int i = 0; i<201; i += 5)
  {
    lcd2.setContrast(i);
    /*
    Serial.print("Contrast set to: ");
    Serial.print(i);
    Serial.print(", the read value for contrast is: ");
    Serial.println(lcd2.getContrast());
    */
    delay(250);
    
  }
  delay(500);
  lcd2.setContrast(contrast);
  backlight = lcd2.getBacklight_display();
  kp_backlight = lcd2.getBacklight_kp();

  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 101; j += 4)
    {
      lcd2.setBacklight(j, 100 - j);
    }
    for(int j = 0; j < 101; j += 4)
    {
      lcd2.setBacklight(100 - j, j);
    }

  }
  
    for(int i = 0; i < 101; i += 4)
    {
      lcd2.setBacklight(i,i);
      Serial.print("What the hell");
      delay(250);
    }

  
  /*
  lcd2.scrollDisplayLeft();
  lcd2.scrollDisplayLeft();
  lcd2.scrollDisplayLeft();
  */
  delay(wait);

  lcd2.clear();
  // Serial.println("The display should be cleared now!");
  delay(wait);
  lcd2.resetDisplay();
  Serial.println("Display reset");
  Serial.println("");
  Serial.println("");
  delay(wait);
}
