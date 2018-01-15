// RFM69HCW Example Sketch
// Send serial input characters from one RFM69 node to another
// Based on RFM69 library sample code by Felix Rusu
// http://LowPowerLab.com/contact
// Modified for RFM69HCW by Mike Grusin, 4/16

// This sketch will show you the basics of using an
// RFM69HCW radio module. SparkFun's part numbers are:
// 915MHz: https://www.sparkfun.com/products/12775
// 434MHz: https://www.sparkfun.com/products/12823

// See the hook-up guide for wiring instructions:
// https://learn.sparkfun.com/tutorials/rfm69hcw-hookup-guide

// Uses the RFM69 library by Felix Rusu, LowPowerLab.com
// Original library: https://www.github.com/lowpowerlab/rfm69
// SparkFun repository: https://github.com/sparkfun/RFM69HCW_Breakout

// Include the RFM69 and SPI libraries:

#include <RFM69.h>
#include <SPI.h>

// Addresses for this node. CHANGE THESE FOR EACH NODE!

#define NETWORKID     0   // Must be the same for all nodes (0 to 255)
#define MYNODEID      1   // My node ID (0 to 255)
#define TONODEID      2   // Destination node ID (0 to 254, 255 = broadcast)

// RFM69 frequency, uncomment the frequency of your module:

//#define FREQUENCY   RF69_433MHZ
#define FREQUENCY     RF69_915MHZ

// AES encryption (or not):

#define ENCRYPT       true // Set to "true" to use encryption
#define ENCRYPTKEY    "TOPSECRETPASSWRD" // Use the same 16-byte key on all nodes

// Use ACKnowledge when sending messages (or not):

#define USEACK        true // Request ACKs or not

// Packet sent/received indicator LED (optional):

#define LED           9 // LED positive pin
#define GND           8 // LED ground pin

// Create a library object for our RFM69HCW module:

RFM69 radio;

#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX (not gonna work on pin 3, but I don't need it to), TX

int battAdcInt = 0; // global variable used to store battery ADC readings (voltage)
int battVoltage = 0;

void setup()
{
  // Open a serial port so we can send keystrokes to the module:
  
  Serial.begin(9600);
  Serial.print("Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" ready");  

  // Set up the indicator LED (optional):
  
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  pinMode(GND,OUTPUT);
  digitalWrite(GND,LOW);
    
  // Initialize the RFM69HCW:
  
  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW
  radio.setPowerLevel(0);

  // Turn on encryption if desired:
  
  if (ENCRYPT)
    radio.encrypt(ENCRYPTKEY);
    
  mySerial.begin(4800);
  test_commands();
}

void loop()
{
  // Set up a "buffer" for characters that we'll send:
  
  static char sendbuffer[62];
  static int sendlength = 0;

  // SENDING

  // In this section, we'll gather serial characters and
  // send them to the other node if we (1) get a carriage return,
  // or (2) the buffer is full (61 characters).
  
  // If there is any serial input, add it to the buffer:

  if (Serial.available() > 0)
  {
    char input = Serial.read();
    
    if (input != '\r') // not a carriage return
    {
      sendbuffer[sendlength] = input;
      sendlength++;
    }

    // If the input is a carriage return, or the buffer is full:
    
    if ((input == '\r') || (sendlength == 61)) // CR or buffer full
    {
      // Send the packet!


      Serial.print("sending to node ");
      Serial.print(TONODEID, DEC);
      Serial.print(": [");
      for (byte i = 0; i < sendlength; i++)
        Serial.print(sendbuffer[i]);
      Serial.println("]");
      
      // There are two ways to send packets. If you want
      // acknowledgements, use sendWithRetry():
      
      if (USEACK)
      {
        if (radio.sendWithRetry(TONODEID, sendbuffer, sendlength))
          Serial.println("ACK received!");
        else
          Serial.println("no ACK received :(");
      }

      // If you don't need acknowledgements, just use send():
      
      else // don't use ACK
      {
        radio.send(TONODEID, sendbuffer, sendlength);
      }
      
      sendlength = 0; // reset the packet
      Blink(LED,10);
    }
  }

  // RECEIVING

  // In this section, we'll check with the RFM69HCW to see
  // if it has received any packets:

  if (radio.receiveDone()) // Got one!
  {
    
    // Print out the information:
    
    Serial.print("received from node ");
    Serial.print(radio.SENDERID, DEC);
    Serial.print(": [");

    // The actual message is contained in the DATA array,
    // and is DATALEN bytes in size:
    
    String battAdcString = "";
    
    for (byte i = 0; i < radio.DATALEN; i++)
    {
      char data = char(radio.DATA[i]);
      Serial.print(data);  
      battAdcString += data; // append each new char to the string (for use later as an int with "toInt()"
      //Serial.print((char)radio.DATA[i]); 
    } 
    
    battAdcInt = battAdcString.toInt();
    Serial.print(", "); // space this sucker out, so I know what the heck is going on.
    Serial.print(battAdcInt);
    battVoltage = map(battAdcInt, 860, 974, 111, 126);
    Serial.print(", "); // space this sucker out, so I know what the heck is going on.
    Serial.print(battVoltage);
    
    update_batt_stat();

    // RSSI is the "Receive Signal Strength Indicator",
    // smaller numbers mean higher power.
    
    Serial.print("], RSSI ");
    Serial.println(radio.RSSI);

    // Send an ACK if requested.
    // (You don't need this code if you're not using ACKs.)
    
    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.println("ACK sent");
    }
    Blink(LED,10);
  }
}

void Blink(byte PIN, int DELAY_MS)
// Blink an LED for a given number of ms
{
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}

void test_commands()
{
  for(int i = 0 ; i < 3 ; i++ )
  {
    mySerial.print('a');
    delay(100);
    mySerial.print('b');
    delay(100);
    mySerial.print('c');
    delay(100);
  }
}

void update_batt_stat()
{
  int green = 121;
  int yellow = 117;
  int red = 113;
  if (battVoltage >= green) mySerial.print('a');
  else if (battVoltage <= red) mySerial.print('c');
  else if (battVoltage >= yellow) mySerial.print('b');
}
