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

#ifndef ArduinoSMBus_h
#define ArduinoSMBus_h

#include <Arduino.h>
#include <Wire.h>

#define BLOCKLENGTH 255 // Maximum of data stream to be read

 //Usable Commands
#define MANUFACTURER_ACCESS 0x00
#define REMAINING_CAPACITY_ALARM 0x01
#define REMAINING_TIME_ALARM 0x02
#define BATTERY_MODE 0x03
#define ATRATE 0x04 // Nog implementeren
#define ATRATETIMETOFULL 0x05 // Nog implementeren
#define ATRATETIMETOEMPTY 0x06 // Nog implementeren
#define ATRATEOK 0x07 // Nog implementeren
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
#define SPECIFICATIONINFO 0x1a // Nog implementeren
#define MANUFACTURE_DATE 0x1b
#define SERIAL_NUMBER 0x1c
//reserved 0x1d - 0x1f
#define MANUFACTURER_NAME 0x20
#define DEVICE_NAME 0x21
#define DEVICE_CHEMISTRY 0x22
#define MANUFACTURERDATA 0x23 // Nog implementeren
#define OPTIONALMFGFUNCTIONS 0x2f // Nog implementeren
#define VOLTAGECELLFOUR 0x3c // Nog implementeren
#define VOLTAGECELLTHREE 0x3d // Nog implementeren
#define VOLTAGECELLTWO 0x3e // Nog implementeren
#define VOLTAGECELLONE 0x3f // Nog implementeren
#define STATE_OF_HEALTH 0x4f

 /**
 * @struct BatteryMode
 * @brief A struct to hold various battery mode flags.
 */
struct BatteryMode {
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
 * @struct Errorcode
 * @brief A struct to hold various return error codes.
 *
 * After sending a command, a status is requested. The Error (or not) is being placed here.
 */
struct Errorcode {
  String Error;
};

class ArduinoSMBus {
public:
  ArduinoSMBus(uint8_t batteryAddress);
  void setBatteryAddress(uint8_t batteryAddress);
  uint16_t manufacturerAccess(); // command 0x00
  uint16_t remainingCapacityAlarm(); // command 0x01
  uint16_t remainingTimeAlarm(); // command 0x02
  BatteryMode batteryMode(); // command 0x03
  int16_t atRate(); // command 0x04
  uint16_t atRateTimeToFull(); // command 0x05
  uint16_t atRateTimeToEmpty(); // command 0x06
  bool atRateOK(); // command 0x07 
  uint16_t temperature(); // command 0x08, in Kelvin
  uint16_t temperatureC();
  uint16_t temperatureF();
  uint16_t voltage(); // command 0x09
  uint16_t current(); // command 0x0a
  uint16_t averageCurrent(); // command 0x0b
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
  bool statusOK();
  uint16_t cycleCount(); // command 0x17
  uint16_t designCapacity(); // command 0x18
  uint16_t designVoltage(); // command 0x19
  uint16_t specificationInfo(); // command 0x1a
  uint16_t manufactureDate(); // command 0x1b
  uint8_t manufactureDay();
  uint8_t manufactureMonth();
  int manufactureYear();
  uint16_t serialNumber(); // command 0x1c
  const char* manufacturerName(); // command 0x20
  const char* deviceName(); // command 0x21
  const char* deviceChemistry(); // command 0x22
  const char* manufacturerData(); // command 0x23
  const char* optionalMfgFunction(); // command 0x2f
  uint16_t voltageCellFour(); // command 0x3c
  uint16_t voltageCellThree(); // command 0x3d
  uint16_t voltageCellTwo(); // command 0x3e
  uint16_t voltageCellOne(); // command 0x3f
  uint16_t stateOfHealth(); // command 0x4f
  String* ErrorCode(void);
private:
  Errorcode ErrorCodes[6];
  uint8_t _batteryAddress;
  uint16_t readRegister(uint8_t reg);
  void readBlock(uint8_t reg, uint8_t* data, uint8_t len);
};

#endif