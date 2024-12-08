/**
 * @file SBSCommands.h
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
#include "CommandsClassifiers.h"

struct {
  struct command{
    String name;
    uint8_t reg;
    uint8_t monitor_group;
  };
  command ManufacturerAccess {"ManufacturerAccess()", 0x00, DEVICEINFO}; /**> This function is optional and its meaning is implementation specific.  It may be used by a battery
  manufacturer or silicon supplier to return specific version information, internal calibration information, or some other manufacturer specific function.  There is no implied or 
  required use for this function and therefore it may be used for multiple purposes.  The only requirement is the data protocol listed below: read word or write word. */
  command RemainingCapacityAlarm {"RemainingCapacityAlarm()", 0x01, DEVICEINFO}; /**> Sets or gets the Low Capacity alarm threshold value.  Whenever the RemainingCapacity() falls below the
  Low Capacity value, the Smart Battery sends AlarmWarning() messages to the SMBus Host with the REMAINING_CAPACITY_ALARM bit set.  A Low Capacity value of 0 disables this alarm.
  (If the ALARM_MODE bit is set in BatteryMode() then the AlarmWarning() message is disabled for a set period of time. */
  command RemainingTimeAlarm {"RemainingTimeAlarm()", 0x02, DEVICEINFO}; /**> Sets or gets the Remaining Time alarm value.  Whenever the AverageTimeToEmpty() falls below the Remaining Time value, the Smart Battery sends
  AlarmWarning() messages to the SMBus Host with the REMAINING_TIME_ALARM bit set.  A Remaining Time value of 0 effectively disables this alarm. (If the ALARM_MODE bit is set in 
  BatteryMode() then the AlarmWarning() message is disabled for a set period of time. */
  command BatteryMode {"BatteryMode()", 0x03, STATUSBITS}; /**> This function selects the various battery operational modes and reports the battery’s capabilities, modes, and flags minor conditions requiring attention. */
   /**
   * @union batterymode
   * @brief A struct to hold various battery mode flags.
   */
  union {
    uint16_t raw;
    struct  {
      uint16_t internal_charge_controller : 1; /**< True if the internal charge controller is supported, false otherwise. BIT 0 (Read only) */
      uint16_t primary_battery_support    : 1; /**< True if the primary battery support is supported, false otherwise. BIT 1 (Read only) */
      uint16_t                            : 5; /**< Undefined */
      uint16_t condition_flag             : 1; /**< False if condition is ok, true if battery conditioning cycle is needed. BIT 7 (Read only) */
      uint16_t charge_controller_enabled  : 1; /**< True if the charge controller is enabled, false otherwise. BIT 8 (Read/Write) */
      uint16_t primary_battery            : 1; /**< True if the primary battery is enabled, false otherwise. BIT 9 (Read/Write) */
      uint16_t                            : 3; /**< Undefined */
      uint16_t alarm_mode                 : 1; /**< True to enable alarmWarning broadcasts to host, false to disable. BIT 13 (Read/Write) */
      uint16_t charger_mode               : 1; /**< True to enable chargingCurrent and chargingVoltage broadcasts to host, false to disable. BIT 14 (Read/Write) */
      uint16_t capacity_mode              : 1; /**< True to report in mA or mAh, false to report in 10mW or 10mWh units. BIT 15 (Read/Write) */
    } bits;
  }batterymode;
  command AtRate {"AtRate()", 0x04, ATRATES}; /**> The AtRate() function is the first half of a two-function call-set used to set the AtRate value used in calculations made by the AtRateTimeToFull(),
  AtRateTimeToEmpty(), and AtRateOK() functions.  The AtRate value may be expressed in either current (mA) or power (10mW) depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.*/
  command AtRateTimeToFull {"AtRateTimeToFull()", 0x05, ATRATES}; /**> Returns the predicted remaining time to fully charge the battery at the previously written AtRate value in mA. */
  command AtRateTimeToEmpty {"AtRateTimeToEmpty()", 0x06, ATRATES}; /**> Returns the predicted remaining operating time if the battery is discharged at the previously written AtRate 
  value. (Result will depend on the setting of CAPACITY_MODE bit.)*/
  command AtRateOK {"AtRateOK()", 0x07, ATRATES}; /**> Returns a Boolean value that indicates whether or not the battery can deliver the previously written AtRate value of additional 
  energy for 10 seconds (Boolean).  If the AtRate value is zero or positive, the AtRateOK() function will ALWAYS return true. */
  command Temperature {"Temperature()", 0x08, USAGEINFO}; /**> Returns the cell-pack's internal temperature (°K).  The actual operational temperature range will be defined at a pack level 
  by a particular manufacturer.  Typically it will be in the range of -20°C to +75°C. */
  command Voltage {"Voltage()", 0x09, USAGEINFO}; /**> Returns the cell-pack voltage (mV).*/
  command Current {"Current()", 0x0a, USAGEINFO}; /**> Returns the current being supplied (or accepted) through the battery's terminals (mA).*/
  command AverageCurrent {"AverageCurrent()", 0x0b, USAGEINFO}; /**> Returns a one-minute rolling average based on the current being supplied (or accepted) through the battery's
  terminals (mA).  The AverageCurrent() function is expected to return meaningful values during the battery's first minute of operation. */
  command MaxError {"MaxError()", 0x0c, USAGEINFO}; /**> Returns the expected margin of error (%) in the state of charge calculation. */
  command RelativeStateOfCharge {"RelativeStateOfCharge()", 0x0d, COMPUTEDINFO}; /**> Returns the predicted remaining battery capacity expressed as a percentage of FullChargeCapacity() (%).*/
  command AbsoluteStateOfCharge {"AbsoluteStateOfCharge()", 0x0e, COMPUTEDINFO}; /**> Returns the predicted remaining battery capacity expressed as a percentage of DesignCapacity() (%).  Note
  that AbsoluteStateOfCharge() can return values greater than 100%.*/
  command RemainingCapacity {"RemainingCapacity()", 0x0f, USAGEINFO}; /**> Returns the predicted remaining battery capacity.  The RemainingCapacity() capacity value is expressed in
  either current (mAh at a C/5 discharge rate) or power (10mWh at a P/5 discharge rate) depending on the setting of the BatteryMode()'s CAPACITY_MODE bit. */
  command FullChargeCapacity {"FullChargeCapacity() ", 0x10, USAGEINFO}; /**> Returns the predicted pack capacity when it is fully charged.  The FullChargeCapacity() value is expressed
  in either current (mAh at a C/5 discharge rate) or power (10mWh at a P/5 discharge rate) depending on the setting of the BatteryMode()'s CAPACITY_MODE bit. */
  command RunTimeToEmpty {"RunTimeToEmpty()", 0x11, COMPUTEDINFO}; /**> Returns the predicted remaining battery life at the present rate of discharge (minutes).  The RunTimeToEmpty() 
  value is calculated based on either current or power depending on the setting of the BatteryMode()'s CAPACITY_MODE bit. */
  command AverageTimeToEmpty {"AverageTimeToEmpty()", 0x12, COMPUTEDINFO}; /**> Returns a one-minute rolling average of the predicted remaining battery life (minutes).  The AverageTimeToEmpty() 
  value is calculated based on either current or power depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.*/
  command AverageTimeToFull {"AverageTimeToFull()", 0x13, COMPUTEDINFO}; /**> Returns a one minute rolling average of the predicted remaining time until the Smart Battery reaches full charge (minutes).*/
  command ChargingCurrent{"ChargingCurrent()", 0x14, USAGEINFO}; /**> Sends the desired charging rate to the Smart Battery Charger (mA). This represents the maximum current which may be
  provided by the Smart Battery Charger to permit the Smart Battery to reach a Fully Charged state.*/
  command ChargingVoltage {"ChargingVoltage()", 0x15, USAGEINFO}; /**> Sends the desired charging voltage to the Smart Battery Charger (mV). This represents the maximum voltage which may
  be provided by the Smart Battery Charger to permit the Smart Battery to reach a Fully Charged state. */
  command BatteryStatus {"BatteryStatus()", 0x16, STATUSBITS }; /**> Returns the Smart Battery's status word  which contains Alarm and Status bit flags.  Some of the BatteryStatus() 
  flags (REMAINING_CAPACITY_ALARM and REMAINING_TIME_ALARM) are calculated based on either current or power depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.*/
  /**
   * @union batterystatus
   * @brief A struct to hold various battery status flags.
   *
   * This struct holds various flags that represent the battery status.
   */
  union  {
    uint16_t raw;
    struct  {
      uint16_t error_codes                : 4; /**< */
      uint16_t fully_discharged           : 1; /**< True if the battery is fully discharged, false otherwise. Corresponds to bit 4 of the BatteryStatus register. */
      uint16_t fully_charged              : 1; /**< True if the battery is fully charged, false otherwise. Corresponds to bit 5 of the BatteryStatus register. */
      uint16_t discharging                : 1; /**< True if the battery is discharging, false otherwise. Corresponds to bit 6 of the BatteryStatus register. */
      uint16_t initialized                : 1; /**< True if the battery is initialized, false otherwise. Corresponds to bit 7 of the BatteryStatus register. */
      uint16_t rem_time_alarm             : 1; /**< True if the remaining time alarm is set, false otherwise. Corresponds to bit 8 of the BatteryStatus register. */
      uint16_t rem_capacity_alarm         : 1; /**< True if the remaining capacity alarm is set, false otherwise. Corresponds to bit 9 of the BatteryStatus register. */
      uint16_t                            : 1; /**< Reserved */
      uint16_t term_discharge_alarm       : 1; /**< True if the termination discharge alarm is set, false otherwise. Corresponds to bit 11 of the BatteryStatus register. */
      uint16_t over_temp_alarm            : 1; /**< True if the battery temperature is over the limit, false otherwise. Corresponds to bit 12 of the BatteryStatus register. */
      uint16_t                            : 1; /**< Reserved */
      uint16_t term_charge_alarm          : 1; /**< True if the termination charge alarm is set, false otherwise. Corresponds to bit 14 of the BatteryStatus register. */
      uint16_t over_charged_alarm         : 1; /**< True if the battery is overcharged, false otherwise. Corresponds to bit 15 of the BatteryStatus register. */
    } bits;
  }batterystatus;
  command CycleCount {"CycleCount()", 0x17, USAGEINFO}; // Returns the number of cycles the battery has experienced.  A cycle is defined as: An amount of discharge approximately equal to the value of DesignCapacity.
  command DesignCapacity {"DesignCapacity()", 0x18, DEVICEINFO}; /**> Returns the theoretical capacity of a new pack.  The DesignCapacity() value is expressed in either current (mAh at 
  a C/5 discharge rate) or power (10mWh at a P/5 discharge rate) depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.*/
  command DesignVoltage {"DesignVoltage()", 0x19, DEVICEINFO}; /**> Returns the theoretical voltage of a new pack (mV).*/
  command SpecificationInfo {"SpecificationInfo()", 0x1a, DEVICEINFO}; /**>Returns the version number of the Smart Battery specification the battery pack supports, as well as voltage
  and current and capacity scaling information in a packed unsigned integer.  Power scaling is the product of the voltage scaling times the current scaling.*/
  command ManufactureDate {"ManufactureDate()", 0x1b, DEVICEINFO}; /**> This function returns the date the cell pack was manufactured in a packed integer.  The date is packed in the
  // following fashion: (year-1980) * 512 + month * 32 + day.*/
  command SerialNumber {"SerialNumber()", 0x1c, DEVICEINFO}; /**> This function is used to return a serial number.  This number when combined with the ManufacturerName(), the DeviceName(), 
  // and the ManufactureDate() will uniquely identify the battery (unsigned int).*/
  command ManufacturerName {"ManufacturerName()", 0x20, DEVICEINFO}; /**> This function returns a character array containing the battery's manufacturer's name.  For example,
  // "MyBattCo" would identify the Smart Battery's manufacturer as MyBattCo.*/
  command DeviceName {"DeviceName()", 0x21, DEVICEINFO}; /**> This function returns a character string that contains the battery's name.  For example, a DeviceName() of "MBC101" 
  // would indicate that the battery is a model MBC101.*/
  command DeviceChemistry {"DeviceChemistry()", 0x22, DEVICEINFO}; /**> This function returns a character string that contains the battery's chemistry.  For example, if the
  // DeviceChemistry() function returns "NiMH," the battery pack would contain nickel metal hydride cells.*/
  command ManufacturerData {"ManufacturerData()", 0x23, DEVICEINFO}; /**> This function allows access to the manufacturer data contained in the battery (data).*/
  command OptionalMfgFunction4 {"OptionalMfgFunction4)", 0x3c, DEVICEINFO}; /**> */
  command OptionalMfgFunction3 {"OptionalMfgFunction3)", 0x3d, DEVICEINFO}; /**> */
  command OptionalMfgFunction2 {"OptionalMfgFunction2)", 0x3e, DEVICEINFO}; /**> */
  command OptionalMfgFunction1 {"OptionalMfgFunction1)", 0x3f, DEVICEINFO}; /**> */
}SBS_COMMAND;

/**
 * @union errorcodes
 * @brief Holds the description of the various battery status error_codes (see batterystatus).
 *
 */
static String errorcodes[8] {
  "ok",                 /**< The Smart Battery processed the function code without detecting any errors. */
  "busy",               /**< The Smart Battery is unable to process the function code at this time. */
  "reserved",           /**< The Smart Battery detected an attempt to read orwrite to a function code reserved by this version of the specification.
The Smart Battery detected an attempt to access an unsupported optional manufacturer function code. */
  "unsupported",         /**< The Smart Battery does not support this function code which is defined in version 1.1 of the specification. */
  "access denied",       /**< The Smart Battery detected an attempt to write to a read only function code. */
  "over-, under-flow",  /**< The Smart Battery detected a data overflow or under flow. */
  "badSize",            /**< The Smart Battery detected an attempt to write to a function code with an incorrect size data block. */
  "unknown"             /**< The Smart Battery detected an unidentifiable error. */
};
