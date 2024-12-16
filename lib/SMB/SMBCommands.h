/**
 * @file SBSCommands.h
 * @author 
 * @brief defines the functions for the Smart Battery Specifications. To be used as a base class.
 * @version 1.0
 * @date 12-2024
 *
 * @copyright
 *
 */
#pragma once

#include <Arduino.h>
#include <string.h>
#include "CommandClassifiers.h"
#include "SMBus.h"
#include <variant>
#include <vector>

#define MANUFACTURERACCESS     0X00
#define REMAININGCAPACITYALARM 0x01
#define REMAININGTIMEALARM     0x02
#define BATTERYMODE            0x03
#define ATRATE                 0x04
#define ATRATETIMETOFULL       0x05
#define ATRATETIEMTOEMPTY      0x06
#define ATRATEOK               0x07
#define TEMPERATURE            0x08
#define VOLTAGE                0x09
#define CURRENT                0x0a
#define AVERAGECURRENT         0x0b
#define MAXERROR               0x0c
#define RELATIVESTATEOFCHARGE  0x0d
#define ABSOLUTESTATEOFCHARGE  0x0e
#define REMAININGCAPACITY      0x0f
#define FULLCAPACITY           0x10
#define RUNTIMETOEMPTY         0x11
#define AVGTIMETOEMPTY         0x12
#define AVGTIMETOFULL          0x13
#define CHARGINGCURRENT        0x14
#define CHARGINGVOLTAGE        0x15
#define BATTERYSTATUS          0x16
#define CYCLECOUNT             0x17
#define DESIGNCAPACITY         0x18
#define DESIGNVOLTAGE          0x19
#define SPECIFICATIONINFO      0x1a
#define MANUFACTURERDATE       0x1b
#define SERIALNUMBER           0x1c
                               // 0x1d - 0x1f are reserved
#define MANUFACTURERNAME       0x20
#define DEVICENAME             0x21
#define DEVICECHEMISTRY        0x22
#define MANUFACTURERDATA       0x23
                               // 0x25 - 0x2e are reserved
#define OPTIONALMFGFUNCTION5   0x2f
#define OPTIONALMFGFUNCTION4   0x3c
#define OPTIONALMFGFUNCTION3   0x3d
#define OPTIONALMFGFUNCTION2   0x3e
#define OPTIONALMFGFUNCTION1   0x3f

class smbuscommands;
template <class T>
using myfunc = void(T::*)();

using boolfunction = bool(smbuscommands::*)();
using uint16function = uint16_t (smbuscommands::*)();
using int16function = int16_t (smbuscommands::*)();
using floatfunction = float (smbuscommands::*)();
using charfunction = char* (smbuscommands::*)();

class smbuscommands : public smbus {
public:
  smbuscommands(uint8_t address);
  virtual uint16_t manufacturerAccess();  // command 0x00
  uint16_t remainingCapacityAlarm();      // command 0x01
  uint16_t remainingTimeAlarm();          // command 0x02
  uint16_t batteryMode();                 // command 0x03
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
  int16_t atRate();                       // command 0x04
  uint16_t atRateTimeToFull();            // command 0x05
  uint16_t atRateTimeToEmpty();           // command 0x06
  bool atRateOK();                        // command 0x07 
  uint16_t temperature();                 // command 0x08, in Kelvin
  float temperatureC();
  float temperatureF();
  uint16_t voltage();                     // command 0x09
  int16_t current();                      // command 0x0a
  int16_t averageCurrent();               // command 0x0b
  uint16_t maxError();                    // command 0x0c
  uint16_t relativeStateOfCharge();       // command 0x0d
  uint16_t absoluteStateOfCharge();       // command 0x0e
  uint16_t remainingCapacity();           // command 0x0f
  uint16_t fullCapacity();                // command 0x10
  uint16_t runTimeToEmpty();              // command 0x11
  uint16_t avgTimeToEmpty();              // command 0x12
  uint16_t avgTimeToFull();               // command 0x13
  uint16_t chargingCurrent();             // command 0x14
  uint16_t chargingVoltage();             // command 0x15
  uint16_t batteryStatus();               // command 0x16
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
  uint16_t cycleCount();                  // command 0x17
  uint16_t designCapacity();              // command 0x18
  uint16_t designVoltage();               // command 0x19
  char* specificationInfo();              // command 0x1a
  uint16_t manufactureDate();             // command 0x1b
  uint16_t manufactureDay();
  uint16_t manufactureMonth();
  uint16_t manufactureYear();
  uint16_t serialNumber();                // command 0x1c
  char* manufacturerName();               // command 0x20
  char* deviceName();                     // command 0x21
  char* deviceChemistry();                // command 0x22
  virtual char* manufacturerData();       // command 0x23, virtual function may be overidden to be more specific for a certain type IC
  uint16_t optionalMFGfunction4();        // command 0x3c
  uint16_t optionalMFGfunction3();        // command 0x3d
  uint16_t optionalMFGfunction2();        // command 0x3e
  uint16_t optionalMFGfunction1();        // command 0x3f
  uint8_t address();

struct Info{
  uint8_t reg;
  union {
    boolfunction p_boolfunction;
    uint16function p_uint16function;
    int16function p_int16function;
    floatfunction p_floatfunction;
    charfunction p_charfunction;
  };
  uint8_t monitor_group;
  String name;
  Info(uint8_t r, boolfunction pbf, uint8_t g, String n) : reg(r), p_boolfunction(pbf), monitor_group(g), name(n) {}
  Info(uint8_t r, uint16function pu16, uint8_t g, String n) : reg(r), p_uint16function(pu16), monitor_group(g), name(n) {}
  Info(uint8_t r, int16function pi16, uint8_t g, String n) : reg(r), p_int16function(pi16), monitor_group(g), name(n) {}
  Info(uint8_t r, floatfunction pf, uint8_t g, String n) : reg(r), p_floatfunction(pf), monitor_group(g), name(n) {}
  Info(uint8_t r, charfunction pc, uint8_t g, String n) : reg(r), p_charfunction(pc), monitor_group(g), name(n) {}
  Info(uint8_t r, uint8_t g, String n) : reg(r), monitor_group(g), name(n) {}  
};
  std::vector<Info> info;

  int16_t readRegister(uint8_t reg);
  void writeRegister(uint8_t reg, uint16_t data);
  void readBlock(uint8_t reg, uint8_t* data, uint8_t len);
  private:
  uint8_t batteryAddress;
};

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
