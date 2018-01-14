// Pete Lewis
// started 1/13/2018
// The plan is to read the voltage my 3Cell lipo battery on an ADC.
// It should range from 12.6V down to 11.1V
// My ADC logic level is 3.3V, with 1023 steps of detail.
// So if I split 12.6V down with a voltage divider, I should be able to see enough of a change.
// First voltage divider value attempts: lipo batt --> 10K --> 3.3K --> GND
// This should mean that 12.6V hits my ADC at 3.126V, and 11.1V will be at 2.754
// found these values at : http://www.ohmslawcalculator.com/voltage-divider-calculator
// This sketch is intended to show ADC values on a varying input voltage,
// and use those values to know the status of my battery on my RC airplane.

// 638 = 12.6
// 607 = 12.0
// 580 = 11.5
// 562 = 11.1

void setup() {                
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  int val = analogRead(A0);
  Serial.print(val);
  delay(500);
}
