/**
 * @file bq40z6xx.h
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
#include "../SMB/SMBCommands.h"

#define BQICTYPE bq40z6xx

#define UNSEALA                         0x0414 /**< Unseal Key a */
#define UNSEALB                         0x3672 /**< Unseal Key b */
#define FULLACCESSA                     0xffff
#define FULLACCESSB                     0xffff
#define PFCLEARA                        0x2673 /**< Permanent Failure Clear Key A or 0x0001 , 0x0102*/
#define PFCLEARB                        0x1712 /**< Permanent Failure Clear Key B */

// following commands are part of the SBS manufactureraccess command 0x00 (so to used as f.e 0x00 0x0001)
#define MANUFACTURERACCESSTYPE          0x0001
#define MANUFACTURERACCESSFIRMWARE      0x0002
#define MANUFACTURERACCESSHARDWARE      0x0003
//#define MANUFACTURERACCESSTATUS       0x0006
#define MANUFACTURERACCESSCHEMISTRY     0x0006
#define MANUFACTURERACCESSSHUTDOWN      0x0010
#define MANUFACTURERACCESSSLEEP         0x0011
#define MANUFACTURERACCESSFETCONTROL    0x0022
#define MANUFACTURERACCESSSEAL          0x0030
#define MANUFACTURERACCESSSECURITYKEYS  0x0035
#define MANUFACTURERACCESSCHARGINGSTATUS 0x0055

#define MANUFACTURERACCESSSTATEOFHEALTH 0x0077

// following commands are direct SBS commands
#define ALTERNATEMANUFACTURERACCESS     0x44
#define SAFETYALERT                     0x50
#define SAFETYSTATUS                    0x51
#define PFALERT                         0x52
#define PFSTATUS                        0x53
#define OPERATIONSTATUS                 0x54
#define CHARGINGSTATUS                  0x54
#define MANUFACTURINGSTATUS             0x57
//#define UNSEALKEY                     0x60
#define MANUFACTURERINFO                0x70

/**
 * @class command
 * @brief Extended commands pecific for the bq40z6xx
 */
class bq40z6xx : protected smbuscommands{
  protected:
  bq40z6xx(uint8_t address);
  char* manufacturerAccessType();      // command 0x00 0x0001 read data via 0x23 (manufacturerData)
  char* manufacturerAccessFirmware();     // command 0x00 0x0002 read data via 0x23 (manufacturerData)
  char* manufacturerAccessHardware();     // command 0x00 0x0003 read data via 0x23 (manufacturerData)
  char* manufacturerAccessChemistryID();  // command 0x00 0x0006 read data via 0x23 (manufacturerData)
  void manufacturerAccessShutdown();      // command 0x0010
  void manufacturerAccessSleep();         // command 0x0011

  /**
  * @union manufacturerstatus
  * @brief 
  */
  union {
    uint16_t raw;
    struct  {
      uint16_t lowbyte : 8; /**< {0b00001010} Fixed, not allowed to write to */
      uint16_t state   : 4; /**< Indicates the battery state */
      uint16_t pf      : 2; /**< Indicates permanent failure cause when permanent failure indicated by STATE3..STATE0 */
      uint16_t fet     : 2; /**< Indicates the state of the charge and discharge FETs */
    } bits;
  }manufacturerstatus;
  /**> The commands in this section cause the bq20z90/bq20z95 to take actions when written. No data is returned.*/
  void manufacturerAccessSeal();
  char* manufacturerSecurityKeys();
  void manufacturerAccessPermanentFailClear(uint16_t key_a, uint16_t key_b); 
  void manufacturerAccessUnseal(uint16_t key_a, uint16_t key_b);
  void manufacturerAccessFullAccess(uint16_t key_a, uint16_t key_b);
  char* manufacturerData();       // command 0x23
  /**
  * @struct manufacturerdata
  * @brief This read- or write-word function allows direct control of the FETs for test purposes.
  */
  union {
    char raw[17];                 /**< Data read */
  }manufacturerdata;
  
  // following are extended SBS commands which are only available when the bq20z90/bq20z95 device is in unsealed mode.
  uint16_t fetControl(); // command 0x46
  /**
  * @union fetcontrol
  * @brief This read- or write-word function allows direct control of the FETs for test purposes.
  */
  union {
    uint16_t raw;                 /**< Data read */
    struct {
      uint16_t       : 1;         /**> Reserved and must be programmed to 0 */
      uint16_t dsg   : 1;         /**> Discharge FET control 0 = turn OFF DSG FET. DSG FET doesn't turn of in charge mode to protect the FET body diode. 1 = turn ON DSG FET */
      uint16_t chg   : 1;         /**> Charge FET Control, 0 = turn OFF CHG FET. CHG FET doesn't turn off in discharge mode to protect the FET body diode. 1 = turn ON CHG FET */
      uint16_t zvchg : 1;         /**> Zero-Volt (Pre-Charge) charge FET Control */
      uint16_t od    : 1;         /**> bq29330 GPOD pin control. 0 = disable GPOD pin (high-Z), 1 = enable GPOD pin (open drain) */
      uint16_t       : 11;        /**> Reserved and must be programmed to 0 */
    } bits;
  }fetcontrol;
  uint16_t stateOfHealth();
  void safetyAlert(); // command 0x50
  /**
 * @union safetyalert
 * @brief A struct to hold the flags of the pending safety issues register.
 */
  union {
    uint32_t raw;                 /**< Data read */
    struct {
      uint32_t cuv  : 1;          /**< Cell undervoltage alert. */
      uint32_t cov  : 1;          /**< Cell overvoltage alert. */
      uint32_t occ1 : 1;          /**< Over Current during Charge 1. */
      uint32_t occ2 : 1;          /**< Over Current during Charge 2. */
      uint32_t ocd1 : 1;          /**< Over Current during Discharge 1. */
      uint32_t ocd2 : 1;          /**< Over Current during Discharge 2. */
      uint32_t aold : 1;          /**< Overload during Discharge. */
      uint32_t aoldl : 1;         /**< Overload during Discharge Latch. */
      uint32_t rsvd0 : 1;          /**< Reserved. */
      uint32_t asccl : 1;         /**< Short-circuit during Charge Latch. */
      uint32_t rsvd1 : 1;          /**< Reserved. */
      uint32_t ascdl : 1;         /**< Short-circuit during Discharge Latch. */
      uint32_t otc  : 1;          /**< Over Temperature during Charge. */
      uint32_t otd  : 1;          /**< Over Temperature during Discharge. */
      uint32_t cuvc : 1;          /**< Cell Undervoltage Compensated. */
      uint32_t rsvd2 : 1;          /**< Reserved. */
      uint32_t otf  : 1;          /**< ?. */
      uint32_t rsvd3 : 2;          /**< Reserved. */
      uint32_t ptos  : 1;         /**< Precharge Timeout Suspend. */
      uint32_t cto  : 1;          /**< Charge Timeout. */
      uint32_t rsvd4 : 1;          /**< Reserved. */
      uint32_t oc   : 1;          /**< Over Charge. */
      uint32_t chgc : 1;          /**< Over Charging Current. */
      uint32_t chgv : 1;          /**< Over Charging Voltage. */
      uint32_t pchgc : 1;         /**< Over Pre-Charge Current. */
      uint32_t utc  : 1;          /**< Under temperature During Charge. */
      uint32_t utd  : 1;          /**< Under temperature During Discharge. */
      uint32_t rsvd5 : 2;          /**< Reserved. */
    } bits;
  }safetyalert;

  void safetyStatus();        // command 0x51
  /**
  * @union safetystatus
  * @brief A struct to hold the flags of the 1st level safety features register.
  */
  union {
    uint32_t raw;                 /**< Data read */
    struct {
      uint32_t cuv  : 1;          /**< Cell undervoltage alert. */
      uint32_t cov  : 1;          /**< Cell overvoltage alert. */
      uint32_t occ1 : 1;          /**< Over Current during Charge 1. */
      uint32_t occ2 : 1;          /**< Over Current during Charge 2. */
      uint32_t ocd1 : 1;          /**< Over Current during Discharge 1. */
      uint32_t ocd2 : 1;          /**< Over Current during Discharge 2. */
      uint32_t aold : 1;          /**< Overload during Discharge. */
      uint32_t aoldl : 1;         /**< Overload during Discharge Latch. */
      uint32_t rsvd : 1;          /**< Reserved. */
      uint32_t asccl : 1;         /**< Short-circuit during Charge Latch. */
      uint32_t rsvd0 : 1;          /**< Reserved. */
      uint32_t ascdl : 1;         /**< Short-circuit during Discharge Latch. */
      uint32_t otc  : 1;          /**< Over Temperature during Charge. */
      uint32_t otd  : 1;          /**< Over Temperature during Discharge. */
      uint32_t cuvc : 1;          /**< Cell Undervoltage Compensated. */
      uint32_t rsvd1 : 1;          /**< Reserved. */
      uint32_t otf  : 1;          /**< ?. */
      uint32_t rsvd2 : 2;          /**< Reserved. */
      uint32_t ptos  : 1;         /**< Precharge Timeout Suspend. */
      uint32_t cto  : 1;          /**< Charge Timeout. */
      uint32_t rsvd3 : 1;          /**< Reserved. */
      uint32_t oc   : 1;          /**< Over Charge. */
      uint32_t chgc : 1;          /**< Over Charging Current. */
      uint32_t chgv : 1;          /**< Over Charging Voltage. */
      uint32_t pchgc : 1;         /**< Over Pre-Charge Current. */
      uint32_t utc  : 1;          /**< Under temperature During Charge. */
      uint32_t utd  : 1;          /**< Under temperature During Discharge. */
      uint32_t rsvd4 : 2;          /**< Reserved. */
    } bits;
  }safetystatus;

  void pfAlert(); // command 0x52
  /**
   * @union pfalert
   * @brief A struct to hold the flags of the pending safety issues register.
   * The permanent failure status register indicates the source of the bq20z90/bq20z95 permanent-failure condition.
   */
  union {
    uint32_t raw;                /**< Data read */
    struct  {
      uint32_t suv  : 1;         /**< Safety Cell Under-Voltage Failure. */
      uint32_t sov  : 1;         /**< Safety Cell Over-Voltage Failure. */
      uint32_t socc : 1;         /**< ?. */
      uint32_t socd : 1;         /**< ?. */
      uint32_t sot  : 1;         /**< Safety Over-Temperature Cell Failure. */
      uint32_t rsvd0 : 1;         /**< Reserved. */
      uint32_t sotf : 1;         /**< Safety Over-Temperature Failure. */
      uint32_t qim  : 1;         /**< QMax Imbalance Failure. */
      uint32_t cb   : 1;         /**< Cell Balancing Failure. */
      uint32_t imp  : 1;         /**< Impedance Failure. */
      uint32_t cd   : 1;         /**< Capacity Degradation Failure. */
      uint32_t vimr : 1;         /**< Voltage Imbalance while Pack Resting. */
      uint32_t vima : 1;         /**< Voltage Imbalance while Pack Active. */
      uint32_t rsvd1 : 3;         /**< Reserved. */
      uint32_t cfetf : 1;        /**< Charge FET Failure. */
      uint32_t dfetf : 1;        /**< Discharge FET Failure. */
      uint32_t rsvd2 : 1;         /**< Reserved. */
      uint32_t fuse : 1;         /**< Chemical Fuse Failure. */
      uint32_t afer : 1;         /**< AFE Register Failure. */
      uint32_t afec : 1;         /**< AFE Communication Failure. */
      uint32_t lvl2 : 1;         /**< Second Level Protector Failure. */
      uint32_t rsvd3 : 2;         /**< Reserved. */
      uint32_t opnc : 1;         /**< Open Cell Tab Connection Failure. */
      uint32_t rsvd4 : 2;         /**< Reserved. */
      uint32_t ts1  : 1;         /**< Open Thermistor - TS1 Failure. */
      uint32_t ts2  : 1;         /**< Open Thermistor - TS2 Failure. */
      uint32_t ts3  : 1;         /**< Open Thermistor - TS3 Failure. */
      uint32_t ts4  : 1;         /**< Open Thermistor - TS4 Failure. */
    } bits;
  }pfalert;

  void pfStatus(); // command 0x53
  /**
   * @union pfstatus
   * @brief A struct to hold the permanent failure status register flags.
   * The permanent failure status register indicates the source of the bq20z90/bq20z95 permanent-failure condition.
   */
  union {
    uint32_t raw;                 /**< Data read */
    struct  {
      uint32_t suv  : 1;         /**< Safety Cell Under-Voltage Failure. */
      uint32_t sov  : 1;         /**< Safety Cell Over-Voltage Failure. */
      uint32_t socc : 1;         /**< ?. */
      uint32_t socd : 1;         /**< ?. */
      uint32_t sot  : 1;         /**< Safety Over-Temperature Cell Failure. */
      uint32_t rsvd0 : 1;         /**< Reserved. */
      uint32_t sotf : 1;         /**< Safety Over-Temperature Failure. */
      uint32_t qim  : 1;         /**< QMax Imbalance Failure. */
      uint32_t cb   : 1;         /**< Cell Balancing Failure. */
      uint32_t imp  : 1;         /**< Impedance Failure. */
      uint32_t cd   : 1;         /**< Capacity Degradation Failure. */
      uint32_t vimr : 1;         /**< Voltage Imbalance while Pack Resting. */
      uint32_t vima : 1;         /**< Voltage Imbalance while Pack Active. */
      uint32_t rsvd1 : 3;         /**< Reserved. */
      uint32_t cfetf : 1;        /**< Charge FET Failure. */
      uint32_t dfetf : 1;        /**< Discharge FET Failure. */
      uint32_t rsvd2 : 1;         /**< Reserved. */
      uint32_t fuse : 1;         /**< Chemical Fuse Failure. */
      uint32_t afer : 1;         /**< AFE Register Failure. */
      uint32_t afec : 1;         /**< AFE Communication Failure. */
      uint32_t lvl2 : 1;         /**< Second Level Protector Failure. */
      uint32_t rsvd3 : 2;         /**< Reserved. */
      uint32_t opnc : 1;         /**< Open Cell Tab Connection Failure. */
      uint32_t rsvd4 : 2;         /**< Reserved. */
      uint32_t ts1  : 1;         /**< Open Thermistor - TS1 Failure. */
      uint32_t ts2  : 1;         /**< Open Thermistor - TS2 Failure. */
      uint32_t ts3  : 1;         /**< Open Thermistor - TS3 Failure. */
      uint32_t ts4  : 1;         /**< Open Thermistor - TS4 Failure. */
    } bits;
  }pfstatus;

  void operationStatus();     // command 0x54
  /**
   * @union operationstatus
   * @brief A struct to hold the operation status flags.
   *
   * This struct holds various flags that represent the operation status.
   */
  union {
    uint32_t raw;                 /**< Data read */
    struct {
      uint32_t pres : 1;         /**< . */
      uint32_t dsg  : 1;         /**< . */
      uint32_t chg  : 1;         /**< . */
      uint32_t pchg : 1;         /**< . */
      uint32_t acfet : 1;        /**< . */
      uint32_t fuse : 1;         /**< . */
      uint32_t aclw : 1;         /**< . */
      uint32_t btp_int : 1;      /**< . */
      uint32_t sec  : 2;         /**< . */
      uint32_t sdv  : 1;         /**< . */
      uint32_t ss   : 1;         /**< . */
      uint32_t pf   : 1;         /**< . */
      uint32_t xdsg : 1;         /**< . */
      uint32_t xchg : 1;         /**< . */
      uint32_t sleep : 1;        /**< . */
      uint32_t sdm  : 1;         /**< . */
      uint32_t led  : 1;         /**< . */
      uint32_t auth : 1;         /**< . */
      uint32_t autocalm : 1;     /**< . */
      uint32_t cal  : 1;         /**< . */
      uint32_t cal_offset : 1;   /**< . */
      uint32_t xl   : 1;         /**< . */
      uint32_t sleepm : 1;       /**< . */
      uint32_t init : 1;         /**< . */
      uint32_t smblcal : 1;      /**< . */
      uint32_t slpad : 1;        /**< . */
      uint32_t slpcc : 1;        /**< . */
      uint32_t cb   : 1;         /**< . */
      uint32_t emshut : 1;       /**< . */
      uint32_t rsvd : 2;         /**< Reserved. */
    } bits;
  }operationstatus;

uint32_t unsealKey();           // command 0x60
//  private:
};

uint16_t manufacturerStatus(); // command 0x57

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
