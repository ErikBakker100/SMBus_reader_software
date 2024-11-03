// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include "i2cscanner.h"

/*
* Nodemcu board : pin number is equal to GPIO
* pin 1 = GPIO1 = TX
* pin 2 = GPIO2 = D4
* pin 3 = GPIO3 = RX
* pin 4 = GPIO4 = D2  (SDA)
* pin 5 = GPIO5 = D1  (SCL)
* pin 12 = GPIO12 = D6
* pin 13 = GPIO13 = D7
* pin 14 = GPIO14 = D5
* pin 15 = GPIO15 = D8
* pin 16 = GPIO16 = D0
*/

uint8_t i2cscan() {
  byte error{0}, address{0}, prev{5};
  Serial.println("Scanning...");
  String message {""};
  for(address = 0; address < 128; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error != prev) {
      switch (error) {
        case 0:
          message = "found";
          break;
        case 1:
          message = "data too long";
          break;
        case 2:
          message = "NACK";
          break;
        case 3:
          message = "NACK";
          break;
        default:
          message = "other";
      }
      prev = error;
    }
    Serial.print(address < 0x10 ? "0x0": "0x");
    Serial.print(address, HEX);
    Serial.println(" " + message);
    if (error == 0) break;
  }
  if (address >= 0x7f) {
    Serial.println("No I2C devices found\n");
    address=0;
  }
  return address;
}
