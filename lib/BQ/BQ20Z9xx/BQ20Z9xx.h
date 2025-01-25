/**
 * @file BQ20Z9xx.h
 * @author 
 * @brief 
 * @version 1.0
 * @date 12-2024
 *
 * @copyright
 *
 */
#pragma once

#include <Arduino.h>
#include <string.h>
#include "../../lib/SMB/SMBus.h"

#define UNSEALA                     0x0414 /**< Unseal Key a */
#define UNSEALB                     0x3672 /**< Unseal Key b */
#define FULLACCESSA                 0xffff
#define FULLACCESSB                 0xffff
#define PFCLEARA                    0x2673 /**< Permanent Failure Clear Key A or 0x0001 , 0x0102*/
#define PFCLEARB                    0x1712 /**< Permanent Failure Clear Key B */

#define MANUFACTURERACCESS          0X00
#define MANUFACTURERACCESSTYPE      0x01
#define MANUFACTURERACCESSFIRMWARE  0x02
#define MANUFACTURERACCESSHARDWARE  0x03
#define MANUFACTURERACCESSTATUS     0x06
#define MANUFACTURERACCESSCHEMISTRY 0x08
#define MANUFACTURERACCESSSHUTDOWN  0x10
#define MANUFACTURERACCESSSLEEP     0x11
#define MANUFACTURERACCESSSEAL      0x20
#define FETCONTROL                  0x46
#define STATEOFHEALTH               0x4f
#define SAFETYALERT                 0x50
#define SAFETYSTATUS                0x51
#define PFALERT                     0x52
#define PFSTATUS                    0x53
#define OPERATIONSTATUS             0x54
#define UNSEALKEY                   0x60

/**
 * @class command
 * @brief Extended commands pecific for the BQ20Z9xx
 */
class bq20z9xx : public smbus {
  protected:
  bq20z9xx();
  void manufacturerAccessType();            // command 0x00 0x0001

  uint8_t batteryAddress = 0;
private:
  int16_t readRegister(uint8_t reg);
  void writeRegister(uint8_t reg, uint16_t data);
  void readBlock(uint8_t reg);
};

/**
 * @section statuscodes
 * @brief Holds the description of the various state_codes (see ManufacturerStatus).
 *
 */
static String statuscodes[16] {
  "Wake up",                /**> If the 4 bits contain a 0 */
  "Normal Discharge",       /**> If the 4 bits contain a 1 */
  "Not defined",            /**> Code 2 is not defined */
  "Pre-Charge",             /**> If the 4 bits contain a 3 */
  "Not defined",            /**> Code 4 is not defined */
  "Charge",                 /**> If the 4 bits contain a 5 */
  "Not defined",            /**> Code 6 is not defined */
  "Charge Termination",     /**> If the 4 bits contain a 7 */
  "Fault Charge Terminate", /**> If the 4 bits contain a 8 */
  "Permanent Failure",      /**> If the 4 bits contain a 9 */
  "Overcurrent",            /**> If the 4 bits contain a 10 */
  "Overtemperature",        /**> If the 4 bits contain a 11 */
  "Battery Failure",        /**> If the 4 bits contain a 12 */
  "Sleep",                  /**> If the 4 bits contain a 13 */
  "Reserved",               /**> Code 14 is reserved */
  "Battery Removed",        /**> If the 4 bits contain a 15 */
};

/**
 * @section fetcodes
 * @brief Holds the description of the states of the FETs (see Manufacturer Status(0x0006)).
 *
 */
static String fetcodes[4] {
  "Both charge and discharge FETs are on",  /**> If the 2 bits contain a 0 */
  "CHG FET is off, DSG FET is on",          /**> If the 2 bits contain a 1 */
  "Both charge and discharge FETs are off", /**> If the 2 bits contain a 2 */
  "CHG FET is on, DSG FET is off"           /**> If the 2 bits contain a 3 */
};

/**
 * @section permanentfailurecodes
 * @brief Indicates permanent failure cause when permanent failure indicated by STATE3..STATE0 (see Manufacturer Status(0x0006)).
 *
 */
static String permanentfailurecodes[4] {
  "Fuse is blown if enabled via DF:Configuration:Register(64):Permanent Fail Cfg(6)",  /**> If the 2 bits contain a 0 */
  "Cell imbalance failure",                 /**> If the 2 bits contain a 1 */
  "Safety voltage failure",                 /**> If the 2 bits contain a 2 */
  "FET failure"                             /**> If the 2 bits contain a 3 */
};
