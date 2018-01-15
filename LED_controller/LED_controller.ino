/*

This Micro simply takes in commands via software serial, and turns on the correct LEDS.

Battery Voltage indiction LEDs are on the following:

D6: GREEN
D7: YELLOW
D8: RED
D9: GND (bus from all LEDS). With PWM on this line, we can control the brightness.

  Software serial multple serial test
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
 
 The circuit: 
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)
 
 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts, 
 so only the following can be used for RX: 
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
 
 Not all pins on the Leonardo support change interrupts, 
 so only the following can be used for RX: 
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
 
 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example
 
 This example code is in the public domain.
 
 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(12, 13); // RX, TX

int ledPin [] = {6,7,8};


void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
  
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  
  for(int i ; i < 3 ; i++)
  {
   pinMode(ledPin[i], OUTPUT);
   digitalWrite(ledPin[i], LOW); 
  }
  fade_all();
  digitalWrite(9, LOW);
}

void loop() // run over and over
{
  if (mySerial.available())
  {
    int inByte = mySerial.read();
    Serial.write(inByte);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    
    switch (inByte) {
    case 'a':    
      digitalWrite(6, HIGH);
      break;
    case 'b':    
      digitalWrite(7, HIGH);
      break;
    case 'c':    
      digitalWrite(8, HIGH);
      break;
    default:
      // turn all the LEDs off:
      for (int thisPin = 6; thisPin < 9; thisPin++) {
        digitalWrite(thisPin, LOW);
      }
    } 
  }
}

void fade_all(void)
{
    for(int i ; i < 3 ; i++)
  {
   digitalWrite(ledPin[i], HIGH); 
   
  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(9, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(5);                            
  } 
  
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(9, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(5);                            
  } 
  
   digitalWrite(ledPin[i], LOW);
  }
}

void blink_all(void)
{
  digitalWrite(9, LOW);
    for(int i ; i < 3 ; i++)
  {
   digitalWrite(ledPin[i], HIGH); 
   delay(1000);  
   digitalWrite(ledPin[i], LOW);
  }
}
