/**
 * @file SMBus.h
 * @author 
 * @brief defines the bus itself and the read, write functions.
 * @version 
 * @date 12-2024
 *
 * @copyright
 *
 */
#pragma once

#include <Arduino.h>
#include <Wire.h>

#define CLOCKSPEED 130000  /**< Roughly 100kHz */
#define BLOCKLENGTH 20 /**< Maximum of data stream bytes which may be read */

class smbus{
protected:
  smbus();
  void setup(uint8_t);
  int16_t readRegister(uint8_t, uint8_t);
  void writeRegister(uint8_t, uint16_t, uint8_t);
  void readBlock(uint8_t, uint8_t); // fills the text array
  static char text[BLOCKLENGTH];                       // used for holding text returned by readBlock
  static uint8_t i2ccode;                       // Error code returned by I2C
  static uint8_t batteryAddress;
};
