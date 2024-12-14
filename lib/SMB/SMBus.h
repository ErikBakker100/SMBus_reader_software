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
  public:
  smbus();
  virtual int16_t readRegister(uint8_t reg, uint8_t address);
  virtual void writeRegister(uint8_t reg, uint16_t data, uint8_t address);
  virtual void readBlock(uint8_t reg, uint8_t* data, uint8_t len, uint8_t address);

  uint8_t i2ccode; // Error code returned by I2C
  private:
};


