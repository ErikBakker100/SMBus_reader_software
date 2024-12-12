/**
 * @file SMBus.cpp
 * @author 
 * @brief Function definitions for the smbus base class.
 * @version 2.0
 * @date 12-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "SMBus.h"
#define CLOCKSPEED 130000  /**< Roughly 100kHz */

/**
 * @brief Constructor for a new smbus object.
 * 
 * @param none
 */
smbus::smbus() {
  Wire.begin();
  Wire.setClock(CLOCKSPEED);                  
}

/**
 * @brief Read a register from the battery.
 * Reads a standard 16-bit register from the battery.
 * @param reg 
 * @return uint16_t 
 */
int16_t smbus::readRegister(uint8_t reg) {
  Wire.beginTransmission(_batteryAddress);
  Wire.write(reg);
  i2ccode = Wire.endTransmission(false);
  uint8_t datalength = 2;
  Wire.requestFrom(_batteryAddress, datalength); // Read 2 bytes
  if(Wire.available()) {
    return (Wire.read() | Wire.read() << 8);
  } else {
    return 0;
  }
}

/**
 * @brief Write word to a register.
 * @param reg
 * @param data 
 * @return void 
 */
void smbus::writeRegister(uint8_t reg, uint16_t data) {
  Wire.beginTransmission(_batteryAddress);
  Wire.write(reg);
  Wire.write(lowByte(data));
  Wire.write(highByte(data));
  i2ccode = Wire.endTransmission(true);
}

/**
 * @brief Reads a block of data from the battery.
 * Length of block is specified by the length parameter.
 * @param reg 
 * @param data 
 * @param length 
 */
void smbus::readBlock(uint8_t reg, uint8_t* data, uint8_t length) {
  Wire.beginTransmission(_batteryAddress);
  Wire.write(reg);
  i2ccode = Wire.endTransmission(false);
  uint8_t datalength = length + 1; // Request one extra byte for the length byte
  uint8_t count = Wire.requestFrom(_batteryAddress, datalength); // returns the number of bytes returned from the peripheral device
  if (Wire.available()) {
    count = Wire.read(); // The first byte is the length of the block, it returns the number of bytes received.
  }
  for (uint8_t i = 0; i < count && i < length; i++) {
    if (Wire.available()) {
      data[i] = Wire.read();
    }
  }
  data[count+1] = '\0'; //terminate the string
}
