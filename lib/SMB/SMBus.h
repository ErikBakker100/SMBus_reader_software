/**
 * @file SMBus.h
 * @author 
 * @brief defines the bus itself and the read, write function.
 * @version 
 * @date 12-2024
 *
 * @copyright
 *
 */
#pragma once

#include <Arduino.h>
#include <Wire.h>

#define BLOCKLENGTH 20 /**< Maximum of data stream bytes which could be read */

class smbus{
  smbus();
  int16_t readRegister(uint8_t reg);
  void writeRegister(uint8_t reg, uint16_t data);
  void readBlock(uint8_t reg, uint8_t* data, uint8_t len);
};
