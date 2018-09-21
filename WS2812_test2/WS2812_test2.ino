// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      15

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 200; // delay for half a second

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  scanner_top(25);
  scanner_top(25);
  scanner_top(25);
  all_off();
  test_all(15);
}

void test_all(int brightness)
{
  for (int i = 0 ; i < NUMPIXELS ; i++ ) test_loop(i, 0, brightness, 0, 100);
  for (int i = 0 ; i < NUMPIXELS ; i++ ) test_loop(i, brightness, 0, 0, 100);
  for (int i = 0 ; i < NUMPIXELS ; i++ ) test_loop(i, 0, 0, brightness, 100);
}

void test_loop(int pixel, int red, int green, int blue, int delay_val)
{

  pixels.setPixelColor(pixel, pixels.Color(red, green, blue)); // Moderately bright green color.

  pixels.show(); // This sends the updated pixel color to the hardware.

  delay(delay_val);

}

void single_top(int pixel, int red, int green, int blue)
{
  all_off();

  pixel += 4; // get me to the top row of pixels

  pixels.setPixelColor(pixel, pixels.Color(red, green, blue)); // Moderately bright green color.

  pixels.show(); // This sends the updated pixel color to the hardware.

}

void all_off(void)
{
  for (int i = 0 ; i < NUMPIXELS ; i++ ) test_loop(i, 0, 0, 0, 0);
}

void scanner_top(int brightness)
{
  for (int i = 0 ; i < 7 ; i++)
  {
    single_top(i, 0, brightness, 0);
    delay(100);
  }
  for (int i = 6 ; i > 0 ; i--)
  {
    single_top(i, 0, brightness, 0);
    delay(100);
  }

}

