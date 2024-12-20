/**
 * @file ArduinoSMBus.h
 * @author 
 * @brief 
 * @version 
 * @date 12-2024
 *
 * @copyright
 *
 */
#pragma once

//#include <Arduino.h>
#include <Wire.h>
#include <string.h>
#include "../ansi/ansi.h"
#include "SBSCommands.h"
#include "BQ20Z9xx.h"

#define BLOCKLENGTH 20 /**< Maximum of data stream bytes which could be read */
#define UNSEALA 0x0414 /**< Unseal Key a */
#define UNSEALB 0x3672 /**< Unseal Key b */
#define PFCLEARA 0x2673 /**< Permanent Failure Clear Key A or 0x0001 , 0x0102*/
#define PFCLEARB 0x1712 /**< Permanent Failure Clear Key B */

class ArduinoSMBus {
public:
  ArduinoSMBus();
  void setBatteryAddress(uint8_t batteryAddress);
  void manufacturerAccessUnseal(uint16_t UnSealKey_a, uint16_t UnSealKey_b); // command 0x00
  uint16_t manufacturerAccessType(); // command 0x00
  uint16_t manufacturerAccessFirmware(); // command 0x00
  uint16_t manufacturerAccessHardware(); // command 0x00
  void manufacturerStatus(); // command 0x00
  void manufacturerAccessSeal(); // command 0x00
  uint16_t remainingCapacityAlarm(); // command 0x01
  uint16_t remainingTimeAlarm(); // command 0x02
  void batteryMode(); // command 0x03
  int16_t atRate(); // command 0x04
  uint16_t atRateTimeToFull(); // command 0x05
  uint16_t atRateTimeToEmpty(); // command 0x06
  bool atRateOK(); // command 0x07 
  float temperature(); // command 0x08, in Kelvin
  float temperatureC();
  float temperatureF();
  uint16_t voltage(); // command 0x09
  int16_t current(); // command 0x0a
  int16_t averageCurrent(); // command 0x0b
  uint16_t maxError(); // command 0x0c
  uint16_t relativeStateOfCharge(); // command 0x0d
  uint16_t absoluteStateOfCharge(); // command 0x0e
  uint16_t remainingCapacity(); // command 0x0f
  uint16_t fullCapacity(); // command 0x10
  uint16_t runTimeToEmpty(); // command 0x11
  uint16_t avgTimeToEmpty(); // command 0x12
  uint16_t avgTimeToFull(); // command 0x13
  uint16_t chargingCurrent(); // command 0x14
  uint16_t chargingVoltage(); // command 0x15
  void batteryStatus(); // command 0x16
  uint16_t cycleCount(); // command 0x17
  uint16_t designCapacity(); // command 0x18
  uint16_t designVoltage(); // command 0x19
  String specificationInfo(); // command 0x1a
  uint16_t manufactureDate(); // command 0x1b
  uint8_t manufactureDay();
  uint8_t manufactureMonth();
  int manufactureYear();
  uint16_t serialNumber(); // command 0x1c
  const char* manufacturerName(); // command 0x20
  const char* deviceName(); // command 0x21
  const char* deviceChemistry(); // command 0x22
  void manufacturerData(); // command 0x23
  uint16_t voltageCellFour(); // command 0x3c
  uint16_t voltageCellThree(); // command 0x3d
  uint16_t voltageCellTwo(); // command 0x3e
  uint16_t voltageCellOne(); // command 0x3f
  // following are extended SBS commands which are only available when the bq20z90/bq20z95 device is in unsealed mode.
  void FETControl(); // command 0x46
  uint16_t stateOfHealth(); // command 0x4f
  void Safetyalert(); // command 0x50
  void Safetystatus(); // command 0x51
  void PFalert(); // command 0x52
  void PFstatus(); // command 0x53
  void Operationstatus(); // command 0x54
  uint32_t unsealKey(); // command 0x60
  void ClearPermanentFailure(uint16_t key_a, uint16_t key_b);
  uint8_t baterrorcode; // Error code returned by battery
  uint8_t i2ccode; // Error code returned by I2C
  sbs_command SBS_COMMAND;
  bq20z9xx_command BQ20Z9xx_COMMAND;
  private:
  uint8_t _batteryAddress;
  int16_t readRegister(uint8_t reg);
  void writeRegister(uint8_t reg, uint16_t data);
  void readBlock(uint8_t reg, uint8_t* data, uint8_t len);
  String device;
};

void Display_standard(ArduinoSMBus&);
void Display_bq2020z9xx(ArduinoSMBus&);
