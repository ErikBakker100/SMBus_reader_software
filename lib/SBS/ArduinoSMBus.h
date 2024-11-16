/**
 * @file ArduinoSMBus.h
 * @author Christopher Lee (clee@unitedconsulting.com)
 * @brief Function declarations for the ArduinoSMBus class.
 * @version 1.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <string.h>
#include "../ansi/ansi.h"

#define BLOCKLENGTH 20 /**< Maximum of data stream bytes which could be read */
#define UNSEALA 0x0414 /**< Unseal Key a */
#define UNSEALB 0x3672 /**< Unseal Key b */
#define PFCLEARA 0x2673 /**< Permanent Falilure Clear Key A */
#define PFCLEARB 0x1712 /**< Permanent Falilure Clear Key B */
 //Usable Commands
#define MANUFACTURER_ACCESS 0x00
#define REMAINING_CAPACITY_ALARM 0x01
#define REMAINING_TIME_ALARM 0x02
#define BATTERY_MODE 0x03
#define ATRATE 0x04
#define ATRATETIMETOFULL 0x05
#define ATRATETIMETOEMPTY 0x06
#define ATRATEOK 0x07
#define TEMPERATURE 0x08
#define VOLTAGE 0x09
#define CURRENT 0x0a
#define AVERAGE_CURRENT 0x0b
#define MAX_ERROR 0x0c
#define REL_STATE_OF_CHARGE 0x0d
#define ABS_STATE_OF_CHARGE 0x0e
#define REM_CAPACITY 0x0f
#define FULL_CAPACITY 0x10
#define RUN_TIME_TO_EMPTY 0x11
#define AVG_TIME_TO_EMPTY 0x12
#define AVG_TIME_TO_FULL 0x13
#define CHARGING_CURRENT 0x14
#define CHARGING_VOLTAGE 0x15
#define BATTERY_STATUS 0x16
#define CYCLE_COUNT 0x17
#define DESIGN_CAPACITY 0x18
#define DESIGN_VOLTAGE 0x19
#define SPECIFICATIONINFO 0x1a
#define MANUFACTURE_DATE 0x1b
#define SERIAL_NUMBER 0x1c
//reserved 0x1d - 0x1f
#define MANUFACTURER_NAME 0x20
#define DEVICE_NAME 0x21
#define DEVICE_CHEMISTRY 0x22
#define MANUFACTURERDATA 0x23
#define OPTIONALMFGFUNCTIONS 0x2f
#define VOLTAGECELLFOUR 0x3c
#define VOLTAGECELLTHREE 0x3d
#define VOLTAGECELLTWO 0x3e
#define VOLTAGECELLONE 0x3f
#define FETCONTROL 0x46
#define STATE_OF_HEALTH 0x4f
#define SAFETYALERT 0x50
#define SAFETYSTATUS 0x51
#define PFALERT 0x52
#define PFSTATUS 0x53
#define OPERATIONSTATUS 0x54
#define UNSEALKEY 0x60

 /**
 * @struct BatteryMode
 * @brief A struct to hold various battery mode flags.
 */
struct BatteryMode {
  uint16_t raw;                     /**< Data read */
  bool internal_charge_controller;  /**< True if the internal charge controller is supported, false otherwise. BIT 0 (Read only) */
  bool primary_battery_support;     /**< True if the primary battery support is supported, false otherwise. BIT 1 (Read only) */
  bool condition_flag;              /**< False if condition is ok, true if battery conditioning cycle is needed. BIT 7 (Read only) */
  bool charge_controller_enabled;   /**< True if the charge controller is enabled, false otherwise. BIT 8 (Read/Write) */
  bool primary_battery;             /**< True if the primary battery is enabled, false otherwise. BIT 9 (Read/Write) */
  bool alarm_mode;                  /**< True to enable alarmWarning broadcasts to host, false to disable. BIT 13 (Read/Write) */
  bool charger_mode;                /**< True to enable chargingCurrent and chargingVoltage broadcasts to host, false to disable. BIT 14 (Read/Write) */
  bool capacity_mode;               /**< True to report in mA or mAh, false to report in 10mW or 10mWh units. BIT 15 (Read/Write) */
};

/**
 * @struct BatteryStatus
 * @brief A struct to hold various battery status flags.
 *
 * This struct holds various flags that represent the battery status.
 */
struct BatteryStatus {
  uint16_t raw;                  /**< Data Read. */
  bool over_charged_alarm;      /**< True if the battery is overcharged, false otherwise. Corresponds to bit 15 of the BatteryStatus register. */
  bool term_charge_alarm;       /**< True if the termination charge alarm is set, false otherwise. Corresponds to bit 14 of the BatteryStatus register. */
  bool over_temp_alarm;         /**< True if the battery temperature is over the limit, false otherwise. Corresponds to bit 12 of the BatteryStatus register. */
  bool term_discharge_alarm;    /**< True if the termination discharge alarm is set, false otherwise. Corresponds to bit 11 of the BatteryStatus register. */
  bool rem_capacity_alarm;      /**< True if the remaining capacity alarm is set, false otherwise. Corresponds to bit 9 of the BatteryStatus register. */
  bool rem_time_alarm;          /**< True if the remaining time alarm is set, false otherwise. Corresponds to bit 8 of the BatteryStatus register. */
  bool initialized;             /**< True if the battery is initialized, false otherwise. Corresponds to bit 7 of the BatteryStatus register. */
  bool discharging;             /**< True if the battery is discharging, false otherwise. Corresponds to bit 6 of the BatteryStatus register. */
  bool fully_charged;           /**< True if the battery is fully charged, false otherwise. Corresponds to bit 5 of the BatteryStatus register. */
  bool fully_discharged;        /**< True if the battery is fully discharged, false otherwise. Corresponds to bit 4 of the BatteryStatus register. */
};

/**
 * @struct ManufacturerBatStatus
 * @brief A struct to hold flags indicating the battery state generated by writing 0x0006 to the Manufacturer Access Register.
 */
struct ManufacturerBatStatus {
  uint16_t raw;                 /**< Data Read. */
  String failure {""};
  String permfailure {""};
  bool chg_fet;                 /**< On if 1 (True), Off if 0 (False). */
  bool dsg_fet;                 /**< On if 1 (True), Off if 0 (False). */
};

/**
 * @struct SafetyAlert
 * @brief A struct to hold the flags of teh pending safety issues register.
 */
struct SafetyAlert {
  uint16_t raw;                 /**< Data read */
  bool scd;                     /**< Discharge short-circuit alert. */
  bool scc;                     /**< Charge short-circuit alert. */
  bool aocd;                    /**< AFE discharge overcurrent alert. */
  bool wdf;                     /**< AFE watchdog alert. */  
  bool hwdg;                    /**< Host watchdog alert. */
  bool pf;                      /**< Permanent failure alert. */
  bool cov;                     /**< Cell overvoltage alert. */
  bool cuv;                     /**< Cell undervoltage alert. */
  bool pov;                     /**< Pack overvoltage alert. */
  bool puv;                     /**< Pack undervoltage alert. */
  bool occ2;                    /**< Tier-2 charge overcurrent alert. */
  bool ocd2;                    /**< Tier-2 discharge overcurrent alert. */
  bool occ;                     /**< Charge overcurrent alert. */
  bool ocd;                     /**< Discharge overcurrent alert. */
  bool otc;                     /**< Charge overtemperature alert. */
  bool otd;                     /**< Discharge overtemperature alert. */
};

/**
 * @struct SafetyStatus
 * @brief A struct to hold the flags of the 1st level safety features register.
 */
struct SafetyStatus {
  uint16_t raw;                 /**< Data read */
  bool scd;                     /**< Discharge short-circuit condition. */
  bool scc;                     /**< Charge short-circuit condition. */
  bool aocd;                    /**< AFE discharge overcurrent condition. */
  bool wdf;                     /**< AFE watchdog condition. */  
  bool hwdg;                    /**< Host watchdog condition. */
  bool pf;                      /**< Permanent failure and SAFE pin has been driven high.. */
  bool cov;                     /**< Cell overvoltage condition. */
  bool cuv;                     /**< Cell undervoltage condition. */
  bool pov;                     /**< Pack overvoltage condition. */
  bool puv;                     /**< Pack undervoltage condition. */
  bool occ2;                    /**< Tier-2 charge overcurrent condition. */
  bool ocd2;                    /**< Tier-2 discharge overcurrent condition. */
  bool occ;                     /**< Charge overcurrent condition. */
  bool ocd;                     /**< Discharge overcurrent condition. */
  bool otc;                     /**< Charge overtemperature condition. */
  bool otd;                     /**< Discharge overtemperature condition. */
};

/**
 * @struct PFAlert
 * @brief A struct to hold the flags of the pending safety issues register.
 * The permanent failure status register indicates the source of the bq20z90/bq20z95 permanent-failure condition.
 */
struct PFAlert {
  uint16_t raw;                 /**< Data read */
  bool pfin;                    /**< External Input Indication of permanent failure alert. */
  bool sov;                     /**< Safety-Overvoltage permanent failure alert. */
  bool sotc;                    /**< Charge Safety Overtemperature permanent failure alert. */
  bool sotd;                    /**< Discharge Safety Overtemperature permanent failure alert. */  
  bool cim;                     /**< Cell-Imbalance permanent failure alert. */
  bool cfetf;                   /**< Charge-FET-Failure permanent failure alert. */
  bool dfetf;                   /**< Discharge-FET-Failure permanent failure alert. */
  bool dff;                     /**< Data Flash Fault permanent failure alert. */
  bool ace_c;                   /**< Permanent AFE Communications failure alert. */
  bool afe_p;                   /**< Periodic AFE Communications permanent failure alert. */
  bool socc;                    /**< Charge Safety-Overcurrent permanent failure alert. */
  bool socd;                    /**< Discharge Safety Overcurrent permanent failure alert. */
  bool sopt;                    /**< Open Thermistor permanent failure alert. */
  bool fbf;                     /**< Fuse Blow Failure alert. */
};

/**
 * @struct PFStatus
 * @brief A struct to hold the permanent failure status register flags.
 * The permanent failure status register indicates the source of the bq20z90/bq20z95 permanent-failure condition.
 */
struct PFStatus {
  uint16_t raw;                 /**< Data read */
  bool pfin;                    /**< External Input Indication of permanent failure. */
  bool sov;                     /**< Safety-Overvoltage permanent failure. */
  bool sotc;                    /**< Charge Safety Overtemperature permanent failure. */
  bool sotd;                    /**< Discharge Safety Overtemperature permanent failure. */  
  bool cim;                     /**< Cell-Imbalance permanent failure. */
  bool cfetf;                   /**< Charge-FET-Failure permanent failure. */
  bool dfetf;                   /**< Discharge-FET-Failure permanent failure. */
  bool dff;                     /**< Data Flash Fault permanent failure. */
  bool afe_c;                   /**< Permanent AFE Communications failure. */
  bool afe_p;                   /**< Periodic AFE Communications permanent failure. */
  bool socc;                    /**< Charge Safety-Overcurrent permanent failure. */
  bool socd;                    /**< Discharge Safety Overcurrent permanent failure. */
  bool sopt;                    /**< Open Thermistor permanent failure. */
  bool fbf;                     /**< Fuse Blow Failure. */
};

/**
 * @struct OperationStatus
 * @brief A struct to hold the operation status flags.
 *
 * This struct holds various flags that represent the operation status.
 */
struct OperationStatus {
  uint16_t raw;                 /**< Data read */
  bool pres;                    /**< Low indicating that the system is present (battery inserted). */
  bool fas;                     /**< Low means full access security mode. */
  bool ss;                      /**< True  is Sealed security mode. */    
  bool csv;                     /**< Data Flash checksum value has been generated. */   
  bool ldmd;                    /**< Load mode for Impedance Track modeling. 0 = constant current, 1 = constant power. */      
  bool wake;                    /**< bq20z90/bq20z95 WAKE mode . */          
  bool dsg;                     /**< Replica of the SBS:BatteryStatus(0x16)[DSG] flag.. */ 
  bool xdsg;                    /**< Discharge fault. */
  bool xdsgi;                   /**< Discharge disabled due to a current issue. */
  bool r_dis;                   /**< Ra Table resistance updates are disabled. */
  bool vok;                     /**< Voltages are OK for a QMAX update. */
  bool qen;                     /**< QMAX updates are enabled. */
};

/**
 * @struct FETcontrol
 * @brief This read- or write-word function allows direct control of the FETs for test purposes.
 */
struct FETcontrol {
  uint8_t raw;
  bool od;
  bool zvchg;
  bool chg;                     /**< Charge FET Control, 0 = turn OFF CHG FET. CHG FET doesn't turn off in discharge mode to protect the FET body diode. 1 = turn ON CHG FET */
  bool dsg;                     /**< Discharge FET control 0 = turn OFF DSG FET. DSG FET doesn't turn of in charge mode to protect the FET body diode. 1 = turn ON DSG FET */
};

/**
 * @struct Errorcode
 * @brief Struct to hold latest error generated by communication from the battery or the I2C bus 
 */
struct Errorcode {
  uint8_t nr{255};
  String note{""};
};

class ArduinoSMBus {
public:
  ArduinoSMBus();
  void setBatteryAddress(uint8_t batteryAddress);
  void manufacturerAccessUnseal(uint16_t UnSealKey_a, uint16_t UnSealKey_b); // command 0x00
  uint16_t manufacturerAccessType(uint16_t = 0x0001); // command 0x00
  uint16_t manufacturerAccessFirmware(uint16_t = 0x0002); // command 0x00
  uint16_t manufacturerAccessHardware(uint16_t = 0x0003); // command 0x00
  ManufacturerBatStatus manufacturerAccessBatStatus(uint16_t = 0x0006); // command 0x00
  void manufacturerAccessSeal(uint16_t = 0x0020); // command 0x00
  uint16_t remainingCapacityAlarm(); // command 0x01
  uint16_t remainingTimeAlarm(); // command 0x02
  BatteryMode batteryMode(); // command 0x03
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
  BatteryStatus batteryStatus(); // command 0x16
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
  const char* manufacturerData(); // command 0x23
  uint16_t voltageCellFour(); // command 0x3c
  uint16_t voltageCellThree(); // command 0x3d
  uint16_t voltageCellTwo(); // command 0x3e
  uint16_t voltageCellOne(); // command 0x3f
  // following are extended SBS commands which are only available when the bq20z90/bq20z95 device is in unsealed mode.
  FETcontrol FETControl(); // command 0x46
  uint16_t stateOfHealth(); // command 0x4f
  SafetyAlert Safetyalert(); // command 0x50
  SafetyStatus Safetystatus(); // command 0x51
  PFAlert PFalert(); // command 0x52
  PFStatus PFstatus(); // command 0x53
  OperationStatus Operationstatus(); // command 0x54
  uint32_t unsealKey(); // command 0x60
  void ClearPermanentFailure(uint16_t key_a, uint16_t key_b);
  Errorcode I2CError; // Struct that holds last error code generated by I2C write protocol
  Errorcode BatError; // Struct that holds last error code generated by Battery communication
  void BatErrorCode(); // Update Error code returned by battery
private:
  void I2Ccode(uint8_t code); // Update error code returned by I2C
  uint8_t _batteryAddress;
  int16_t readRegister(uint8_t reg);
  void writeRegister(uint8_t reg, uint16_t data);
  void readBlock(uint8_t reg, uint8_t* data, uint8_t len);
};

void Display_standard(ArduinoSMBus&);
void Display_bq2020z9xx(ArduinoSMBus&);
