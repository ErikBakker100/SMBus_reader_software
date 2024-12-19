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
#include "../SMB/SMBCommands.h"

#define BQICTYPE bq20z9xx

#define UNSEALA                     0x0414 /**< Unseal Key a */
#define UNSEALB                     0x3672 /**< Unseal Key b */
#define FULLACCESSA                 0xffff
#define FULLACCESSB                 0xffff
#define PFCLEARA                    0x2673 /**< Permanent Failure Clear Key A or 0x0001 , 0x0102*/
#define PFCLEARB                    0x1712 /**< Permanent Failure Clear Key B */

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
class bq20z9xx : public smbuscommands{
  public:
  bq20z9xx(uint8_t address);
  uint16_t manufacturerAccessType(); // command 0x00 0x0001
  uint16_t manufacturerAccessFirmware(); // command 0x00
  uint16_t manufacturerAccessHardware(); // command 0x00
  uint16_t manufacturerAccessStatus(); // command 0x00
  uint16_t manufacturerAccessChemistryID(); // command 0x00 0x0008
  void manufacturerAccessShutdown(); // command 0x0010
  void manufacturerAccessSleep(); // command 0x0011

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
  uint16_t manufacturerAccessSeal();
  uint16_t manufacturerAccessPermanentFailClear(uint16_t key_a, uint16_t key_b); 
  uint16_t manufacturerAccessUnseal(uint16_t key_a, uint16_t key_b);
  uint16_t manufacturerAccessFullAccess(uint16_t key_a, uint16_t key_b);
  char* manufacturerData();       // command 0x23
  /**
  * @struct manufacturerdata
  * @brief This read- or write-word function allows direct control of the FETs for test purposes.
  */
  union {
    char raw[16];                 /**< Data read */
    struct {
      uint16_t PackLotCode;
      uint16_t PCBLotCode;
      uint16_t FirmwareVersion;
      uint16_t HardwareRevision;
      uint16_t CellRevision;
      uint8_t PartialResetCounter;
      uint8_t FullResetCounter;
      uint8_t WatchdogResetCounter;
      uint8_t CheckSum;
      uint8_t Length;
    } bytes;
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
  uint16_t stateOfHealth(); // command 0x4f
  uint16_t safetyAlert(); // command 0x50
  /**
 * @union safetyalert
 * @brief A struct to hold the flags of the pending safety issues register.
 */
  union {
    uint16_t raw;                 /**< Data read */
    struct {
      uint16_t scd  : 1;          /**< Discharge short-circuit alert. */
      uint16_t scc  : 1;          /**< Charge short-circuit alert. */
      uint16_t aocd : 1;          /**< AFE discharge overcurrent alert. */
      uint16_t wdf  : 1;          /**< AFE watchdog alert. */  
      uint16_t hwdg : 1;          /**< Host watchdog alert. */
      uint16_t pf   : 1;          /**< Permanent failure alert. */
      uint16_t cov  : 1;          /**< Cell overvoltage alert. */
      uint16_t cuv  : 1;          /**< Cell undervoltage alert. */
      uint16_t pov  : 1;          /**< Pack overvoltage alert. */
      uint16_t puv  : 1;          /**< Pack undervoltage alert. */
      uint16_t occ2 : 1;          /**< Tier-2 charge overcurrent alert. */
      uint16_t ocd2 : 1;          /**< Tier-2 discharge overcurrent alert. */
      uint16_t occ  : 1;          /**< Charge overcurrent alert. */
      uint16_t ocd  : 1;          /**< Discharge overcurrent alert. */
      uint16_t otc  : 1;          /**< Charge overtemperature alert. */
      uint16_t otd  : 1;          /**< Discharge overtemperature alert. */
    } bits;
  }safetyalert;
  uint16_t safetyStatus();        // command 0x51
  /**
  * @union safetystatus
  * @brief A struct to hold the flags of the 1st level safety features register.
  */
  union {
    uint16_t raw;                 /**< Data read */
    struct {
      uint16_t scd  : 1;          /**< Discharge short-circuit condition. */
      uint16_t scc  : 1;          /**< Charge short-circuit condition. */
      uint16_t aocd : 1;          /**< AFE discharge overcurrent condition. */
      uint16_t wdf  : 1;          /**< AFE watchdog condition. */  
      uint16_t hwdg : 1;          /**< Host watchdog condition. */
      uint16_t pf   : 1;          /**< Permanent failure and SAFE pin has been driven high.. */
      uint16_t cov  : 1;          /**< Cell overvoltage condition. */
      uint16_t cuv  : 1;          /**< Cell undervoltage condition. */
      uint16_t pov  : 1;          /**< Pack overvoltage condition. */
      uint16_t puv  : 1;          /**< Pack undervoltage condition. */
      uint16_t occ2 : 1;          /**< Tier-2 charge overcurrent condition. */
      uint16_t ocd2 : 1;          /**< Tier-2 discharge overcurrent condition. */
      uint16_t occ  : 1;          /**< Charge overcurrent condition. */
      uint16_t ocd  : 1;          /**< Discharge overcurrent condition. */
      uint16_t otc  : 1;          /**< Charge overtemperature condition. */
      uint16_t otd  : 1;          /**< Discharge overtemperature condition. */
    } bits;
  }safetystatus;
  uint16_t pfAlert(); // command 0x52
  /**
   * @union pfalert
   * @brief A struct to hold the flags of the pending safety issues register.
   * The permanent failure status register indicates the source of the bq20z90/bq20z95 permanent-failure condition.
   */
  union {
    uint16_t raw;                /**< Data read */
    struct  {
      uint16_t pfin  : 1;         /**< External Input Indication of permanent failure alert. */
      uint16_t sov   : 1;         /**< Safety-Overvoltage permanent failure alert. */
      uint16_t sotc  : 1;         /**< Charge Safety Overtemperature permanent failure alert. */
      uint16_t sotd  : 1;         /**< Discharge Safety Overtemperature permanent failure alert. */  
      uint16_t cim   : 1;         /**< Cell-Imbalance permanent failure alert. */
      uint16_t cfetf : 1;         /**< Charge-FET-Failure permanent failure alert. */
      uint16_t dfetf : 1;         /**< Discharge-FET-Failure permanent failure alert. */
      uint16_t dff   : 1;         /**< Data Flash Fault permanent failure alert. */
      uint16_t ace_c : 1;         /**< Permanent AFE Communications failure alert. */
      uint16_t afe_p : 1;         /**< Periodic AFE Communications permanent failure alert. */
      uint16_t socc  : 1;         /**< Charge Safety-Overcurrent permanent failure alert. */
      uint16_t socd  : 1;         /**< Discharge Safety Overcurrent permanent failure alert. */
      uint16_t sopt  : 1;         /**< Open Thermistor permanent failure alert. */
      uint16_t       : 2;         /**< Reserved. */   
      uint16_t fbf   : 1;         /**< Fuse Blow Failure alert. */
    } bits;
  }pfalert;
  uint16_t pfStatus(); // command 0x53
  /**
   * @union pfstatus
   * @brief A struct to hold the permanent failure status register flags.
   * The permanent failure status register indicates the source of the bq20z90/bq20z95 permanent-failure condition.
   */
  union {
    uint16_t raw;                 /**< Data read */
    struct  {
      uint16_t raw   : 1;         /**< Data read */
      uint16_t pfin  : 1;         /**< External Input Indication of permanent failure. */
      uint16_t sov   : 1;         /**< Safety-Overvoltage permanent failure. */
      uint16_t sotc  : 1;         /**< Charge Safety Overtemperature permanent failure. */
      uint16_t sotd  : 1;         /**< Discharge Safety Overtemperature permanent failure. */  
      uint16_t cim   : 1;         /**< Cell-Imbalance permanent failure. */
      uint16_t cfetf : 1;         /**< Charge-FET-Failure permanent failure. */
      uint16_t dfetf : 1;         /**< Discharge-FET-Failure permanent failure. */
      uint16_t dff   : 1;         /**< Data Flash Fault permanent failure. */
      uint16_t ace_c : 1;         /**< Permanent AFE Communications failure. */
      uint16_t afe_p : 1;         /**< Periodic AFE Communications permanent failure. */
      uint16_t socc  : 1;         /**< Charge Safety-Overcurrent permanent failure. */
      uint16_t socd  : 1;         /**< Discharge Safety Overcurrent permanent failure. */
      uint16_t sopt  : 1;         /**< Open Thermistor permanent failure. */
      uint16_t       : 2;         /**< Reserved. */   
      uint16_t fbf   : 1;         /**< Fuse Blow Failure. */
    } bits;
  }pfstatus;
  uint16_t operationStatus();     // command 0x54
  /**
   * @union operationstatus
   * @brief A struct to hold the operation status flags.
   *
   * This struct holds various flags that represent the operation status.
   */
  union {
    uint16_t raw;                 /**< Data read */
    struct {
      uint16_t qen   : 1;         /**< QMAX updates are enabled. */
      uint16_t vok   : 1;         /**< Voltages are OK for a QMAX update. */
      uint16_t r_dis : 1;         /**< Ra Table resistance updates are disabled. */
      uint16_t       : 1;         /**> Reserved */
      uint16_t xdsgi : 1;         /**< Discharge disabled due to a current issue. */
      uint16_t xdsg  : 1;         /**< Discharge fault. */
      uint16_t dsg   : 1;         /**< Replica of the SBS:BatteryStatus(0x16)[DSG] flag.. */ 
      uint16_t wake  : 1;         /**< bq20z90/bq20z95 WAKE mode . */          
      uint16_t       : 2;         /**> Reserved */
      uint16_t ldmd  : 1;         /**< Load mode for Impedance Track modeling. 0 = constant current, 1 = constant power. */      
      uint16_t       : 1;         /**> Reserved */
      uint16_t csv   : 1;         /**< Data Flash checksum value has been generated. */   
      uint16_t ss    : 1;         /**< True is Sealed security mode. */    
      uint16_t fas   : 1;         /**< Low means full access security mode. */
      uint16_t pres  : 1;         /**< Low indicating that the system is present (battery inserted). */
    } bits;
  }operationstatus;
  uint32_t unsealKey();           // command 0x60
//  private:
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
