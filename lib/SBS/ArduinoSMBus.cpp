/**
 * @file ArduinoSMBus.cpp
 * @author Christopher Lee (clee@unitedconsulting.com)
 * @brief Function definitions for the ArduinoSMBus class.
 * @version 1.1
 * @date 2024-03-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "ArduinoSMBus.h"
#define BACKGROUND ansi.black
#define TAB1 40
#define TAB2 65

ANSI ansi(&Serial);

/**
 * @brief Construct a new ArduinoSMBus:: ArduinoSMBus object.
 * 
 * @param batteryAddress 
 */
ArduinoSMBus::ArduinoSMBus() {
  Wire.begin();
  Wire.setClock(130000);                  /**< Roughly 100kHz */
}

/**
 * @brief Set the battery's I2C address.
 * Can be used to change the address after the object is created
 * @param batteryAddress 
 */
void ArduinoSMBus::setBatteryAddress(uint8_t batteryAddress) {
  _batteryAddress = batteryAddress;
}

/**
 * @brief implementation specific. Used to get into Sealed mode or Full Access
 * Content determined by the Smart Battery's manufacturer. TI default unseal codes are 0414 3672 and the default full access codes are ffff ffff.
 * Unsealing is a 2 step command performed by writing the 1st word of the UnSealKey followed by the second word of the UnSealKey.
 * Changing from Unsealed to Full Access is performed by writing the 1st word of the FullAccessKey followed by the second word of the FullAccessKey.
 * This command is only available when the bq20z90/bq20z95 is in Sealed mode.
 * @return void
 */
void ArduinoSMBus::manufacturerAccessUnseal(uint16_t UnSealKey_a, uint16_t UnSealKey_b) {
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the IC part number.
 * Content determined by the Smart Battery's manufacturer.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::manufacturerAccessType(uint16_t code) {
  writeRegister(MANUFACTURER_ACCESS, code);
  return readRegister(MANUFACTURER_ACCESS);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the Firmware version.
 * Content determined by the Smart Battery's manufacturer. The format is most-significant byte (MSB) = Decimal integer, and the
 * least-significant byte (LSB) = sub-decimal integer, e.g.: 0x0120 = version 01.20.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::manufacturerAccessFirmware(uint16_t code) {
  writeRegister(MANUFACTURER_ACCESS, code);
  return readRegister(MANUFACTURER_ACCESS);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the Hardware version.
 * Content determined by the Smart Battery's manufacturer. Returns the hardware version stored in a single byte of reserved data flash.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::manufacturerAccessHardware(uint16_t code) {
  writeRegister(MANUFACTURER_ACCESS, code);
  return readRegister(MANUFACTURER_ACCESS);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the Battery Status.
 * Content determined by the Smart Battery's manufacturer. 
 * @return ManufacturerBatteryStatus
 */
ManufacturerBatStatus ArduinoSMBus::manufacturerAccessBatStatus(uint16_t code) {
  ManufacturerBatStatus status{0};
  writeRegister(MANUFACTURER_ACCESS, code);
  uint16_t data = readRegister(MANUFACTURER_ACCESS);
  status.raw = data;

  if ((data & 0x000f) == 0) status.failure = "Wake Up.";
  else if ((data & 0x000f) == 1) status.failure = "Normal Discharge";
  else if ((data & 0x000f) == 3) status.failure = "Pre-Charge";
  else if ((data & 0x000f) == 5) status.failure = "Charge";
  else if ((data & 0x000f) == 7) status.failure = "Charge Termination";
  else if ((data & 0x000f) == 8) status.failure = "Fault Charge Terminate";
  else if ((data & 0x000f) == 9) status.failure = "Permanent Failure, ";
  else if ((data & 0x000f) == 10) status.failure = "Overcurrent";
  else if ((data & 0x000f) == 11) status.failure = "Overtemperature";
  else if ((data & 0x000f) == 12) status.failure = "Battery Failure";
  else if ((data & 0x000f) == 13) status.failure = "Sleep";
  else if ((data & 0x000f) == 15) status.failure = "Battery Removed";
  status.permfailure = "";
  if (data >> 14 == 0) {
    status.chg_fet = true;
    status.dsg_fet = true; }
  else if (data >> 14 == 1) {
    status.chg_fet = false;
    status.dsg_fet = true; }
  else if (data >> 14 == 2) {
    status.chg_fet = false;
    status.dsg_fet = false; }
  else {
    status.chg_fet = true;
    status.dsg_fet = false; }
  if ((data & 0x000f) == 9) { // Permanent Failure
    if (((data >> 12) & 0xc) == 0) status.permfailure = "Fuse is blown";
    else if (((data >> 12) & 0xc) == 1) status.permfailure = "Cell imbalance failure";
    else if (((data >> 12) & 0xc) == 2) status.permfailure = "Safety voltage failure";
    else status.permfailure = "FET failure";
  }
  return status;
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Brings the IC in a Sealed condition.
 * Content determined by the Smart Battery's manufacturer. For the bq20z90/bq20z95 used to limit access to the extended SBS functions and data flash space, sets.
 * This command is only available when the bq20z90/bq20z95 is in Unsealed or Full Access mode.
 * @return void
 */
void ArduinoSMBus::manufacturerAccessSealDevice(uint16_t code) {
  writeRegister(MANUFACTURER_ACCESS, code);
}

/**
 * @brief Get the battery's remaining capacity alarm.
 * Returns the battery's remaining capacity alarm threshold value, in mAh.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::remainingCapacityAlarm() {
  return readRegister(REMAINING_CAPACITY_ALARM);
}

/**
 * @brief Get the battery's remaining time alarm.
 * Returns the battery's remaining time alarm threshold value, in minutes.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::remainingTimeAlarm() {
  return readRegister(REMAINING_TIME_ALARM);
}

/**
 * @brief Get the battery's mode.
 * 
 * This method reads the battery's mode register, which contains various settings and status bits.
 * It then creates a BatteryMode struct and sets its fields based on the bits in the mode.
 * 
 * @return BatteryMode A struct containing the following fields:
 * - internal_charge_controller: bit 0 of the mode register
 * - primary_battery_support: bit 1 of the mode register
 * - condition_flag: bit 7 of the mode register
 * - charge_controller_enabled: bit 8 of the mode register
 * - primary_battery: bit 9 of the mode register
 * - alarm_mode: bit 13 of the mode register
 * - charger_mode: bit 14 of the mode register
 * - capacity_mode: bit 15 of the mode register
 */
BatteryMode ArduinoSMBus::batteryMode() {
  // Read the battery mode from the device...
  uint16_t mode = readRegister(BATTERY_MODE);
  // Create a BatteryMode struct and set its fields based on the mode
  BatteryMode batteryMode;
  batteryMode.internal_charge_controller = mode & 0x0001;
  batteryMode.primary_battery_support = (mode >> 1) & 0x0001;
  batteryMode.condition_flag = (mode >> 7) & 0x0001;
  batteryMode.charge_controller_enabled = (mode >> 8) & 0x0001;
  batteryMode.primary_battery = (mode >> 9) & 0x0001;
  batteryMode.alarm_mode = (mode >> 13) & 0x0001;
  batteryMode.charger_mode = (mode >> 14) & 0x0001;
  batteryMode.capacity_mode = (mode >> 15) & 0x0001;
  // Return the struct
  return batteryMode;
}

/**
 * @brief First half of a two-function call-set used to set the AtRate value used in calculations made by the AtRateTimeToFull(), AtRateTimeToEmpty(), and AtRateOK() functions.
 * The AtRate value is positive for charge, negative for discharge and zero for neither (default), may be expressed in either current (mA) or power (10mW) depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.
 * @return int16_t 
 */
int16_t ArduinoSMBus::atRate() {
  return readRegister(ATRATE);
}

/**
 * @brief Returns the predicted remaining time to fully charge the battery at the previously written AtRate value in mA.
 * Returns minutes. 65,535 indicates the battery is not being charged. If the CAPACITY_MODE bit is set, then AtRateTimeToFull() may return 65535 to indicate over-range and return an error code indicating overflow.  Alternately, this function may return a remaining time to full based on a 10 mW value in AtRate().
 * @return uint16_t
 */
uint16_t ArduinoSMBus::atRateTimeToFull() {
  return readRegister(ATRATETIMETOFULL);
}

/**
 * @brief Returns the predicted remaining operating time if the battery is discharged at the previously written AtRate value. (Result will depend on the setting of CAPACITY_MODE bit.)
 * Returns minutes. 65,535 indicates the battery is not being discharged.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::atRateTimeToEmpty() {
  return readRegister(ATRATETIMETOEMPTY);
}

/**
 * @brief Part of a two-function call-set used by power management systems to determine if the battery can safely supply enough energy for an additional load. 
 * Returns a Boolean value that indicates whether or not the battery can deliver the previously written AtRate value of additional energy for 10 seconds (Boolean).  If the AtRate value is zero or positive, the AtRateOK() function will ALWAYS return true.  Result may depend on the setting of CAPACITY_MODE bit.
 * @return bool
 */
bool ArduinoSMBus::atRateOK() {
  return readRegister(ATRATEOK);
}

/**
 * @brief Get the battery's temperature.
 * Returns the battery temperature in Kelvin.
 * @return float 
 */
float ArduinoSMBus::temperature() {
  return readRegister(TEMPERATURE)/10;
}

/**
 * @brief Get the battery's temperature in Celsius.
 * Returns the battery temperature in 0.1 degrees Celsius.
 * @return float 
 */
float ArduinoSMBus::temperatureC() {
  float temperatureKelvin = readRegister(TEMPERATURE)/10;
  return temperatureKelvin - 273.15;
  
//  uint16_t temperatureCelsius = temperatureKelvin - 2731; // Convert from Kelvin to Celsius
//  return temperatureCelsius;
}

/**
 * @brief Get the battery's temperature in Fahrenheit.
 * Returns the battery temperature in 0.1 degrees Fahrenheit.
 * @return float 
 */
float ArduinoSMBus::temperatureF() {
  float temperatureKelvin = readRegister(TEMPERATURE)/10;
  ;
  return temperatureKelvin - 459.67;

//  uint16_t temperatureFahrenheit = (temperatureKelvin * 18 - 45967) / 10; // Convert from Kelvin to Fahrenheit
//  return temperatureFahrenheit;
}

/**
 * @brief Get the battery's voltage.
 * Returns the sum of all cell voltages, in mV.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::voltage() {
  return readRegister(VOLTAGE);
}

/**
 * @brief Get the battery's current.
 * Returns the battery measured current (from the coulomb counter) in mA.
 * @return uint16_t 
 */
int16_t ArduinoSMBus::current() {
  return readRegister(CURRENT);
}

/**
 * @brief Get the battery's average current.
 * Returns the average current in a 1-minute rolling average, in mA.
 * @return uint16_t 
 */
int16_t ArduinoSMBus::averageCurrent() {
  return readRegister(AVERAGE_CURRENT);
}

/**
 * @brief Get the battery's state of charge error.
 * Returns the battery's margin of error when estimating SOC, in percent
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::maxError() {
  return readRegister(MAX_ERROR);
}

/**
 * @brief Get the battery's current relative charge.
 * Returns the predicted remaining battery capacity as a percentage of fullChargeCapacity()
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::relativeStateOfCharge() {
  uint16_t data = readRegister(REL_STATE_OF_CHARGE);
  data &= 0x00ff;
  return data;
}

/**
 * @brief Get the battery's absolute charge.
 * Returns the predicted remaining battery capacity as a percentage of designCapacity()
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::absoluteStateOfCharge() {
  uint16_t data = readRegister(ABS_STATE_OF_CHARGE);
  data &= 0x00ff;
  return data;
}

/**
 * @brief Get the battery's capacity.
 * Returns the predicted battery capacity when fully charged, in mAh.
 * For some batteries, this may be in 10s of mWh, if the BatteryMode() register (0x03) is set that way
 * See protocol documentation for details.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::remainingCapacity() {
  return readRegister(REM_CAPACITY);
}

/**
 * @brief Get the battery's full capacity.
 * Returns the predicted battery capacity when fully charged, in mAh.
 * For some batteries, this may be in 10s of mWh, if the BatteryMode() register (0x03) is set that way
 * See protocol documentation for details.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::fullCapacity() {
  return readRegister(FULL_CAPACITY);
}

/**
 * @brief Get the battery's time to empty.
 * Returns the predicted time to empty, in minutes, based on current instantaneous discharge rate.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::runTimeToEmpty() {
  return readRegister(RUN_TIME_TO_EMPTY);
}

/**
 * @brief Get the battery's average time to empty.
 * Returns the predicted time to empty, in minutes, based on 1-minute rolling average discharge rate.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::avgTimeToEmpty() {
  return readRegister(AVG_TIME_TO_EMPTY);
}

/**
 * @brief Get the battery's time to full.
 * Returns the predicted time to full charge, in minutes, based on 1-minute rolling average charge rate.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::avgTimeToFull() {
  return readRegister(AVG_TIME_TO_FULL);
}

/**
 * @brief Get the battery's design charging current.
 * Returns the desired design charging current of the battery, in mA.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::chargingCurrent() {
  return readRegister(CHARGING_CURRENT);
}

/**
 * @brief Get the battery's design charging voltage.
 * Returns the desired design charging voltage of the battery, in mV.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::chargingVoltage() {
  return readRegister(CHARGING_VOLTAGE);
}

/**
 * @brief Get the battery's status.
 * 
 * This function reads the BatteryStatus register and returns a struct with its value.
 * The BatteryStatus register indicates various alarm conditions and states of the battery.
 * These include over charge, termination charge, over temperature, termination discharge,
 * remaining capacity, remaining time, initialization, discharging, fully charged, and fully discharged states.
 * 
 * @return BatteryStatus A struct containing the status of each bit in the BatteryStatus register.
 */
BatteryStatus ArduinoSMBus::batteryStatus() {
  uint16_t data = readRegister(BATTERY_STATUS);
  BatteryStatus status;

  status.over_charged_alarm = data & (1 << 15);
  status.term_charge_alarm = data & (1 << 14);
  status.over_temp_alarm = data & (1 << 12);
  status.term_discharge_alarm = data & (1 << 11);
  status.rem_capacity_alarm = data & (1 << 9);
  status.rem_time_alarm = data & (1 << 8);
  status.initialized = data & (1 << 7);
  status.discharging = data & (1 << 6);
  status.fully_charged = data & (1 << 5);
  status.fully_discharged = data & (1 << 4);
  return status;
}

/**
 * @brief  Get the battery's cycle count.
 * Returns the number of discharge cycles the battery has experienced.
 * A cycle is defined as an amount of discharge equal to the battery's design capacity.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::cycleCount() {
  return readRegister(CYCLE_COUNT);
}

/**
 * @brief Get the battery's design capacity.
 * Returns the theoretical maximum capacity of the battery, in mAh.
 * For some batteries, this may be in 10 mWh, if the BatteryMode() register (0x03) is set to CAPM 1.
 * See TI protocol documentation for details.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::designCapacity() {
  return readRegister(DESIGN_CAPACITY);
}

/**
 * @brief Get the battery's design voltage.
 * Returns the nominal voltage of the battery, in mV.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::designVoltage() {
  return readRegister(DESIGN_VOLTAGE);
}

/**
 * @brief  Get the battery's supported smart battery specification.
 * Returns the Version and revision : 
 * Day + Month*32 + (Year–1980)*512
 * @return String 
 */
String ArduinoSMBus::specificationInfo() {
  uint16_t data = readRegister(SPECIFICATIONINFO);
  data &= 0x00ff;
  String info{"Version: "};
  info += String(data, HEX);
//  if (data & 0x08) info += "Version: 1.0.";
//  if (data & 0x02) info += "Version: 1.1.";
//  if (data & 0x03) info += "Version: 1.1 with optional PEC support.";
//  if (data & 0x10) info += "Version: 1.0 and 1.1.";
  return info;
}

/**
 * @brief  Get the battery's manufacture date.
 * Returns the date the battery was manufactured, in the following format: 
 * Day + Month*32 + (Year–1980)*512
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::manufactureDate() {
  return readRegister(MANUFACTURE_DATE);
}

/**
 * @brief Get the manufacture day from the manufacture date.
 * @return uint8_t
 */
uint8_t ArduinoSMBus::manufactureDay() {
  uint16_t date = manufactureDate();
  uint8_t day = (date & 0xF);
  return day;
}
/**
 * @brief Get the manufacture month from the manufacture date.
 * @return uint8_t
 */
uint8_t ArduinoSMBus::manufactureMonth() {
  uint8_t date = manufactureDate();
  uint8_t month = ((date >> 5 ) & 0xF);
  return month;
}
/**
 * @brief Get the manufacture year from the manufacture date.
 * @return int 
 */
int ArduinoSMBus::manufactureYear() {
  uint16_t date = manufactureDate();
  int year = ((date >> 9) & 0x7F) + 1980;
  return year;
}

/**
 * @brief Get the Serial Number from the battery.
 * 
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::serialNumber() {
  return readRegister(SERIAL_NUMBER);
}

/**
 * @brief Get the Manufacturer Name from the battery.
 * 
 * @return const char* 
 */
const char* ArduinoSMBus::manufacturerName() {
  static char manufacturerName[BLOCKLENGTH]; // 20 characters plus null terminator
  readBlock(MANUFACTURER_NAME, reinterpret_cast<uint8_t*>(manufacturerName), BLOCKLENGTH-2);
  manufacturerName[BLOCKLENGTH-1] = '\0'; // Null-terminate the C-string
  return manufacturerName;
}

/**
 * @brief Get the Device Name from the battery.
 * 
 * @return const char* 
 */
const char* ArduinoSMBus::deviceName() {
  static char deviceName[BLOCKLENGTH]; // Assuming the device name is up to 20 characters long
  readBlock(DEVICE_NAME, reinterpret_cast<uint8_t*>(deviceName), BLOCKLENGTH-2);
  deviceName[BLOCKLENGTH-1] = '\0'; // Null-terminate the C-string
  return deviceName;
}

/**
 * @brief Get the Device Chemistry from the battery.
 * 
 * @return const char* 
 */
const char* ArduinoSMBus::deviceChemistry() {
  static char deviceChemistry[BLOCKLENGTH];
  readBlock(DEVICE_CHEMISTRY, reinterpret_cast<uint8_t*>(deviceChemistry), BLOCKLENGTH-2);
  deviceChemistry[BLOCKLENGTH-1] = '\0';
  return deviceChemistry;
}

/**
 * @brief Get the manufacturer data.
 * Returns the string the manufacturer has programmed.
 * @return string 
 */
const char* ArduinoSMBus::manufacturerData() {
  static char data[BLOCKLENGTH];
  readBlock(MANUFACTURERDATA, reinterpret_cast<uint8_t*>(data), BLOCKLENGTH-2);
  data[BLOCKLENGTH-1] = '\0'; // Null-terminate the C-string
  return data;
}

/**
 * @brief Get the battery's cell 4 voltage.
 * Returns the nominal voltage of cell 4, in mV.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::voltageCellFour() {
  return readRegister(VOLTAGECELLFOUR);
}

/**
 * @brief Get the battery's cell 3 voltage.
 * Returns the nominal voltage of cell 3, in mV.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::voltageCellThree() {
  return readRegister(VOLTAGECELLTHREE);
}

/**
 * @brief Get the battery's cell 2 voltage.
 * Returns the nominal voltage of cell 2, in mV.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::voltageCellTwo() {
  return readRegister(VOLTAGECELLTWO);
}

/**
 * @brief Get the battery's cell 1 voltage.
 * Returns the nominal voltage of cell 1, in mV.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::voltageCellOne() {
  return readRegister(VOLTAGECELLONE);
}

/**
 * @brief Get the current FET Status from the battery.
 * Returns the current operation status struct 
 * This command is not supported by all batteries.
 * @return FETcontrol 
 */
FETcontrol ArduinoSMBus::FETControl() {
  FETcontrol fets;
  uint16_t data = readRegister(FETCONTROL);
  fets.raw = static_cast<uint8_t>(data & 0x00FF);
  fets.od = data & (1 << 4);
  fets.zvchg = data & (1 << 3);
  fets.chg = data & (1 << 2);
  fets.dsg = data & (1 << 1);
  return fets; 
}

/**
 * @brief Get the State of Health from the battery.
 * Returns the estimated health of the battery, as a percentage of design capacity
 * This command is not supported by all batteries.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::stateOfHealth() {
  return readRegister(STATE_OF_HEALTH);
}

/**
 * @brief Returns indications of pending safety issues.
 * This read-word function returns indications of pending safety issues, such as running safety timers, or fail 
 * counters that are nonzero but have not reached the required time or value to trigger a SafetyStatus failure.
 * This command is not supported by all batteries.
 * @return SafetyAlert 
 */
SafetyAlert ArduinoSMBus::Safetyalert() {
  SafetyAlert status{0};
  uint16_t data = readRegister(SAFETYALERT);
  status.raw = data;
  status.scd = data & (1 << 0);
  status.scc = data & (1 << 1);
  status.aocd = data & (1 << 2);
  status.wdf = data & (1 << 3);
  status.hwdg = data & (1 << 4);
  status.pf = data & (1 << 5);
  status.cov = data & (1 << 6);
  status.cuv = data & (1 << 7);
  status.pov = data & (1 << 8);
  status.puv = data & (1 << 9);
  status.occ2 = data & (1 << 10);
  status.ocd2 = data & (1 << 11);
  status.occ = data & (1 << 12);
  status.ocd = data & (1 << 13);
  status.otc = data & (1 << 14);
  status.otd = data & (1 << 15);
  return status;
}

/**
 * @brief Returns the status of the 1st level safety features.
 * This command is not supported by all batteries.
 * @return SafetyStatus 
 */
SafetyStatus ArduinoSMBus::Safetystatus() {
  SafetyStatus status{0};
  uint16_t data = readRegister(SAFETYSTATUS);
  status.raw = data;
  status.scd = data & (1 << 0);
  status.scc = data & (1 << 1);
  status.aocd = data & (1 << 2);
  status.wdf = data & (1 << 3);
  status.hwdg = data & (1 << 4);
  status.pf = data & (1 << 5);
  status.cov = data & (1 << 6);
  status.cuv = data & (1 << 7);
  status.pov = data & (1 << 8);
  status.puv = data & (1 << 9);
  status.occ2 = data & (1 << 10);
  status.ocd2 = data & (1 << 11);
  status.occ = data & (1 << 12);
  status.ocd = data & (1 << 13);
  status.otc = data & (1 << 14);
  status.otd = data & (1 << 15);
  return status;
}

/**
 * @brief Returns indications of pending safety issues.
 * This read-word function returns indications of pending safety issues, such as running safety timers that
 * have not reached the required time to trigger a PFAlert failure
 * @return PFAlert 
 */
PFAlert ArduinoSMBus::PFalert() {
  PFAlert status{0};
  uint16_t data = readRegister(PFALERT);
  status.raw = data;
  status.pfin = data & (1 << 0);
  status.sov = data & (1 << 1);
  status.sotc = data & (1 << 2);
  status.sotd = data & (1 << 3);
  status.cim = data & (1 << 4);
  status.cfetf = data & (1 << 5);
  status.dfetf = data & (1 << 6);
  status.dff = data & (1 << 7);
  status.ace_c = data & (1 << 8);
  status.afe_p = data & (1 << 9);
  status.socc = data & (1 << 10);
  status.socd = data & (1 << 11);
  status.sopt = data & (1 << 12);
  status.fbf = data & (1 << 15);
  return status;
}

/**
 * @brief Get the data from the permanent failure status register.
 * Returns the source of the bq20z90/bq20z95 permanent-failure condition.
 * This command is not supported by all batteries.
 * @return PFStatus
 */
PFStatus ArduinoSMBus::PFstatus() {
  PFStatus status{0};
  uint16_t data = readRegister(PFSTATUS);
  status.raw = data;
  status.pfin = data & (1 << 0);
  status.sov = data & (1 << 1);
  status.sotc = data & (1 << 2);
  status.sotd = data & (1 << 3);
  status.cim = data & (1 << 4);
  status.cfetf = data & (1 << 5);
  status.dfetf = data & (1 << 6);
  status.dff = data & (1 << 7);
  status.afe_c = data & (1 << 8);
  status.afe_p = data & (1 << 9);
  status.socc = data & (1 << 10);
  status.socd = data & (1 << 11);
  status.sopt = data & (1 << 12);
  status.fbf = data & (1 << 15);
  return status;
}

/**
 * @brief Get the current Operation Status from the battery.
 * Returns the current operation status struct 
 * This command is not supported by all batteries.
 * @return uint16_t 
 */
OperationStatus ArduinoSMBus::Operationstatus() {
  OperationStatus status{0};
  uint16_t data = readRegister(OPERATIONSTATUS);
  status.raw = data;
  status.qen = data & (1 <<0 ); 
  status.vok = data & (1 << 1);
  status.r_dis = data & (1 << 2);
  status.xdsgi = data & (1 << 4);
  status.xdsg = data & (1 << 5);
  status.dsg = data & (1 << 6);
  status.wake = data & (1 << 7);
  status.ldmd = data & (1 << 10);
  status.csv = data & (1 << 12);
  status.ss = data & (1 << 13);
  status.fas = data & (1 << 14);
  status.pres = data & (1 << 15);
  return status;
}

/**
 * @brief Clear bq20z90/bq20z95 permanent failure.
 * The bq20z90/bq20z95 permanent failure can be cleared by sending 
 * two ManufacturerAccess commands in sequence: the first word of the 
 * PFKey followed by the second word of the PFKey. After sending 
 * these two commands in sequence, PFStatus flags are cleared. 
 * Refer to Permanent Fail Clear (PFKey) Manufacturer access for further details.
 * Related Variables:
 * • SBS:ManufacturerAccess(0x00)
 * • SBS:PFStatus(0x53)
 * @param void
 * @return void 
 */
void ClearingPermanentFailure() {

}

/**
 * @brief Read a register from the battery.
 * Reads a standard 16-bit register from the battery.
 * @param reg 
 * @return uint16_t 
 */
int16_t ArduinoSMBus::readRegister(uint8_t reg) {
  Wire.beginTransmission(_batteryAddress);
  Wire.write(reg);
  I2Ccode(Wire.endTransmission());
  
  delay(10);
  
  uint8_t datalength = 2;
  Wire.requestFrom(_batteryAddress, datalength); // Read 2 bytes
  
  if(Wire.available()) {
    return Wire.read() | (Wire.read() << 8);
  } else {
    return 0;
  }
/* 
  Wire.beginTransmission(_batteryAddress);
  Wire.write(BATTERY_STATUS);
  I2Ccode(Wire.endTransmission());
  delay(10);
  Wire.requestFrom(_batteryAddress, datalength); // Read 2 bytes

  if(Wire.available()) {
    uint16_t status = Wire.read() | (Wire.read() << 8);
    BatErrorCode(status &= 0x0007);
  }*/
}

/**
 * @brief Write word to a register.
 * @param reg
 * @param data 
 * @return void 
 */
void ArduinoSMBus::writeRegister(uint8_t reg, uint16_t data) {
  Wire.beginTransmission(_batteryAddress);
  Wire.write(reg);
  Wire.write(lowByte(data));
  Wire.write(highByte(data));
  I2Ccode(Wire.endTransmission());
}

/**
 * @brief Reads a block of data from the battery.
 * Length of block is specified by the length parameter.
 * @param reg 
 * @param data 
 * @param length 
 */
void ArduinoSMBus::readBlock(uint8_t reg, uint8_t* data, uint8_t length) {
  Wire.beginTransmission(_batteryAddress);
  Wire.write(reg);

  I2Ccode(Wire.endTransmission(false));

  delay(10); // Add a small delay to give the device time to prepare the data
  uint8_t datalength = length + 1;
  uint8_t count = Wire.requestFrom(_batteryAddress, datalength); // Request one extra byte for the length

  if (Wire.available()) {
    count = Wire.read(); // The first byte is the length of the block, it returns the numberr of bytes received.
  }

  for (uint8_t i = 0; i < count && i < length; i++) {
    if (Wire.available()) {
      data[i] = Wire.read();
    }
  }
  data[count] = '\0'; //terminate the string
  
  uint16_t status = readRegister(BATTERY_STATUS);
  BatErrorCode(status &= 0x0007);
}

/**
 * @brief Check if the battery status is OK.
 * Check for any alarm conditions in the battery status. These include over charge, 
 * termination charge, over temperature, termination discharge alarms. If any of these alarms are set, the battery is not OK.
 * 
 * @return String.
 */
void ArduinoSMBus::BatErrorCode(uint8_t code) {
  code &= 0x07;
  if (code != BatError.nr) {
    BatError.nr = code;
    switch (code) {
      case 0:
        BatError.note = "ok";                 /**< The Smart Battery processed the function code without detecting any errors. */
        break;
      case 1:
        BatError.note = "busy";               /**< The Smart Battery is unable to process the function code at this time. */
        break;
      case 2:
        BatError.note = "reserved";           /**< The Smart Battery detected an attempt to read orwrite to a function code
                                                   reserved by this version of the specification. The Smart Battery detected an
                                                   attempt to access an unsupported optional manufacturer function code. */
        break;
      case 3:
        BatError.note = "usupported";         /**< The Smart Battery does not support this function code which is defined in version 1.1 of the specification. */
        break;
      case 4:
        BatError.note = "accessDenied";       /**< The Smart Battery detected an attempt to write to a read only function code. */
        break;
      case 5:
        BatError.note = "over-, under-flow";  /**< The Smart Battery detected a data overflow or under flow. */
        break;
      case 6:
        BatError.note = "badSize";            /**< The Smart Battery detected an attempt to write to a function code with an incorrect size data block. */
        break;
      case 7: 
        BatError.note = "unknownerror";       /**< The Smart Battery detected an unidentifiable error. */
    }
  }
}

void ArduinoSMBus::I2Ccode(uint8_t code) {
  code &= 0x05;
  if (I2CError.nr != code) {
    I2CError.nr = code;
    switch (code) {
      case 0:
        I2CError.note = "ok";
        break;
      case 1:
        I2CError.note = "data too long";
        break;
      case 2:
        I2CError.note = "NACK on tx address";
        break;
      case 3:
        I2CError.note = "NACK on tx data";
        break;
      case 4:
        I2CError.note = "other";
      case 5:
        I2CError.note = "timeout";
    }
  }
}

void Display_standard(ArduinoSMBus& battery) {
    uint16_t x, y; // x and y position

    ansi.print("Device Type (0x00 -> 0x0001):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print("bq20z");
    ansi.print(battery.manufacturerAccessType(), HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Firmware version (0x00 -> 0x0002):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    uint16_t version = battery.manufacturerAccessFirmware();
    ansi.print((uint8_t)version >> 8 , HEX);
    ansi.print(".");
    ansi.print((uint8_t)version & 0x00ff);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Hardware version (0x00 -> 0x0003):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    version = battery.manufacturerAccessHardware();
    ansi.print((uint8_t)version & 0x00ff, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Man. Battery Status (0x00 -> 0x0006):");
    ansi.readCursorPosition(x, y);
    ManufacturerBatStatus status = battery.manufacturerAccessBatStatus();
    ansi.gotoXY(TAB1, y);
    ansi.print(status.raw, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.I2CError.note);
    ansi.gotoXY(3, y+1);
    ansi.print("Status: ");
    ansi.gotoXY(TAB1, y+1);
    ansi.println(status.failure + " " + status.permfailure);
    ansi.gotoXY(3, y+2);
    ansi.print("Charge FET:");
    ansi.gotoXY(TAB1, y+2);
    ansi.println(status.chg_fet ? "On" : "Off");
    ansi.gotoXY(3, y+3);
    ansi.print("Discharge FET:");
    ansi.gotoXY(TAB1, y+3);
    ansi.println(status.dsg_fet ? "On" : "Off");

    BatteryMode mode = battery.batteryMode(); // We need to read this first to determine output ranges further on

    ansi.print("Remaining Capacity Alarm (0x01):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.remainingCapacityAlarm());
    ansi.print(mode.capacity_mode ? " 10mWh" : " mAh");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Remaining Time Alarm (0x02):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.remainingTimeAlarm());
    ansi.print(" minutes");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Battery Modes (0x03):"); 
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(mode.raw, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    ansi.print("  Internal Charge Controller:");
    ansi.gotoXY(TAB1, y+1);
    ansi.println(mode.internal_charge_controller ? "Supported" : "Not Supported");
    ansi.print("  Primary Battery Support:");
    ansi.gotoXY(TAB1, y+2);
    ansi.println(mode.primary_battery_support ? "Supported" : "Not Supported");
    ansi.print("  Condition Flag:");
    ansi.gotoXY(TAB1, y+3);
    ansi.println(mode.condition_flag ? "Cycle Requested" : "Battery OK");
    ansi.print("  Internal Charge Controller:");
    ansi.gotoXY(TAB1, y+4);
    ansi.println(mode.charge_controller_enabled ? "Enabled" : "Disabled");
    ansi.print("  Primary Battery:");
    ansi.gotoXY(TAB1, y+5);
    ansi.println(mode.primary_battery ? "Operating in primary role" : "Operating in secondary role");
    ansi.print("  Alarm Mode:");
    ansi.gotoXY(TAB1, y+6);
    ansi.println(mode.alarm_mode ? "Disabled AlarmWarning broadcast" : "Enabled AlarmWarning broadcasts");
    ansi.print("  Charger Mode:");
    ansi.gotoXY(TAB1, y+7);
    ansi.println(mode.charger_mode ? "Disabled broadcasts" : "Enabled broadcasts");
    ansi.print("  Capacity Mode:");
    ansi.gotoXY(TAB1, y+8);
    ansi.println(mode.capacity_mode ? "In 10mW or 10mWh" : "In mA or mAh");

    ansi.print("At Rate (0x04):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.atRate());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("At Rate Time To Full (0x05):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.atRateTimeToFull());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("At Rate Time To Empty (0x06):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.atRateTimeToEmpty());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("At Rate OK (0x07):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.atRateOK() ? "true" : "false");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Temperature (0x08):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.temperature(), 1);
    ansi.print(" Kelvin");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Temperature:");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.temperatureC(), 1);
    ansi.print(" Celsius");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Temperature:");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.temperatureF());
    ansi.print(" Fahrenheit");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Voltage (0x09):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    float voltage = (float)battery.voltage()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Current (0x0a):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.current());
    ansi.print(" mA");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Average Current (0x0b):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.averageCurrent());
    ansi.print(" mA");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Max Error (0x0c):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.maxError());
    ansi.print("%");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Relative State Of Charge (0x0d):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.relativeStateOfCharge());
    ansi.print("% of Full Capacity");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Absolute State Of Charge (0x0e):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.absoluteStateOfCharge());
    ansi.print("% of Full Capacity");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Remaining Capacity (0x0f):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.remainingCapacity());
    ansi.print(mode.capacity_mode ? " 10mW" : " mA");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Full Capacity (0x10):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.fullCapacity());
    ansi.print(mode.capacity_mode ? " 10mWh" : " mAh");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Run Time To Empty (0x11):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.runTimeToEmpty());
    ansi.print(" minutes");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Average Time To Empty (0x12):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.avgTimeToEmpty());
    ansi.print(" minutes");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Average Time To Full (0x13):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.avgTimeToFull());
    ansi.print(" minutes");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Desired Charging Current (0x14):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.chargingCurrent());
    ansi.print(" mA");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Desired Charging Voltage (0x15):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.chargingVoltage());
    ansi.print(" mV");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Battery Status (0x16):");
    ansi.readCursorPosition(x, y);
    BatteryStatus stat = battery.batteryStatus();
    ansi.gotoXY(TAB1, y);
    ansi.print(stat.raw, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    ansi.print("  Over Charged Alarm:");
    ansi.gotoXY(TAB1, y+1);
    ansi.println(stat.over_charged_alarm ? "Battery fully charged" : "Cleared");
    ansi.print("  Terminate Charge Alarm:");
    ansi.gotoXY(TAB1, y+2);
    ansi.println(stat.term_charge_alarm ? "Suspend charging" : "No charging, alarm cleared");
    ansi.print("  Over Temperature Alarm:");
    ansi.gotoXY(TAB1, y+3);
    ansi.println(stat.over_temp_alarm ? "Above limit" : "Within acceptable range");
    ansi.print("  Terminate Discharge Alarm:");
    ansi.gotoXY(TAB1, y+4);
    ansi.println(stat.term_discharge_alarm ? "Capacity depleted" : "Discharge not detected");
    ansi.print("  Remaining Capacity Alarm:");
    ansi.gotoXY(TAB1, y+5);
    ansi.println(stat.rem_capacity_alarm ? "True" : "False or 0");
    ansi.print("  Remaining Time Alarm:");
    ansi.gotoXY(TAB1, y+6);
    ansi.println(stat.rem_time_alarm ? "True" : "False or 0");
    ansi.print("  Initialized:");
    ansi.gotoXY(TAB1, y+7);
    ansi.println(stat.initialized ? "First calibrated" : "Calibration or configuration lost");
    ansi.print("  Discharging:");
    ansi.gotoXY(TAB1, y+8);
    ansi.println(stat.discharging ? "True" : "False");
    ansi.print("  Fully Charged:");
    ansi.gotoXY(TAB1, y+9);
    ansi.println(stat.fully_charged ? "True" : "False");
    ansi.print("  Fully Discharged:");
    ansi.gotoXY(TAB1, y+10);
    ansi.println(stat.fully_discharged ? "True" : "False");

    ansi.print("Cycle Count (0x17):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.cycleCount());
    ansi.print(" times");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Design Capacity (0x18):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.designCapacity());
    ansi.print(mode.capacity_mode ? " 10mWh" : " mAh");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Design Voltage (0x19):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    voltage = (float)battery.designVoltage()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Protocol (0x1a):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.specificationInfo());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Manufacture Date (0x1b): ");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.manufactureDate());
    ansi.print(" ");
    ansi.print(battery.manufactureDay());
    ansi.print("-");
    ansi.print(battery.manufactureMonth());
    ansi.print("-");
    ansi.print(battery.manufactureYear());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Serial Number (0x1c):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.serialNumber());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Manufacturer Name (0x20):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.manufacturerName());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Device Name (0x21):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.deviceName());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Device Chemistry (0x22):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.deviceChemistry());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Manufacturer Data (0x23):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.manufacturerData());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    // Following functions are not part of the smart battery specification version 1.1

    ansi.print("voltageCellOne (0x3f):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    voltage = (float)battery.voltageCellOne()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("voltageCellTwo (0x3e):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    voltage = (float)battery.voltageCellTwo()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("voltageCellThree (0x3d):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    voltage = (float)battery.voltageCellThree()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("voltageCellFour (0x3c):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    voltage = (float)battery.voltageCellFour()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
}

void Display_bq2020z9xx(ArduinoSMBus& battery) {
  uint16_t x, y; // x and y position

  ansi.print("FET's status (0x46):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB1, y);
  FETcontrol fets = battery.FETControl();
  if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
  else { 
    ansi.print(fets.raw, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    ansi.print("  Charge FET:");
    ansi.gotoXY(TAB1, y+1);
    ansi.println(fets.chg?"On":"Off");
    ansi.print("  DisCharge FET:");
    ansi.gotoXY(TAB1, y+2);
    ansi.println(fets.dsg?"On":"Off");
  }

  ansi.print("State Of Health (0x4f):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB1, y);
  uint16_t data = battery.stateOfHealth();
  if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
  else { 
    ansi.print(data + "%");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
  }

  ansi.print("Safety Alert (0x50):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB1, y);
  data = battery.Safetyalert().raw;
  if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
  else { 
    ansi.print(data, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
  }

  ansi.print("Safety Status (0x51):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB1, y);
  data = battery.Safetystatus().raw;
  if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
  else { 
    ansi.print(data, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
  }

  ansi.print("PF Alert (0x52):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB1, y);
  data = battery.PFalert().raw;
  if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
  else { 
    ansi.print(data, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
  }

  ansi.print("PF Status (0x53):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB1, y);
  data = battery.PFstatus().raw;
  if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
  else { 
    ansi.print(data, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
  }
}
