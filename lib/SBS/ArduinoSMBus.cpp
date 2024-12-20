/**
 * @file ArduinoSMBus.cpp
 * @author Christopher Lee (clee@unitedconsulting.com), modified extensively
 * @brief Function definitions for the ArduinoSMBus class.
 * @version 2.0
 * @date 12-2024
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
 * Content determined by the Smart Battery's manufacturer. TI default unseal codes are 0x0414 0x3672 and the default full access codes are ffff ffff.
 * Unsealing is a 2 step command performed by writing the 1st word of the UnSealKey followed by the second word of the UnSealKey.
 * Changing from Unsealed to Full Access is performed by writing the 1st word of the FullAccessKey followed by the second word of the FullAccessKey.
 * This command is only available when the bq20z90/bq20z95 is in Sealed mode.
 * @return void
 */
void ArduinoSMBus::manufacturerAccessUnseal(uint16_t UnSealKey_a, uint16_t UnSealKey_b) {
  writeRegister(BQ20Z9xx_COMMAND.UnsealDevice.reg, UnSealKey_a);
  writeRegister(BQ20Z9xx_COMMAND.UnsealDevice.reg, UnSealKey_b);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the IC part number.
 * Content determined by the Smart Battery's manufacturer.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::manufacturerAccessType() {
  writeRegister(BQ20Z9xx_COMMAND.DeviceType.reg, BQ20Z9xx_COMMAND.DeviceType.system_data);
  return readRegister(BQ20Z9xx_COMMAND.DeviceType.reg);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the Firmware version.
 * Content determined by the Smart Battery's manufacturer. The format is most-significant byte (MSB) = Decimal integer, and the
 * least-significant byte (LSB) = sub-decimal integer, e.g.: 0x0120 = version 01.20.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::manufacturerAccessFirmware() {
  writeRegister(BQ20Z9xx_COMMAND.FirmwareVersion.reg, BQ20Z9xx_COMMAND.FirmwareVersion.system_data);
  return readRegister(BQ20Z9xx_COMMAND.FirmwareVersion.reg);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the Hardware version.
 * Content determined by the Smart Battery's manufacturer. Returns the hardware version stored in a single byte of reserved data flash.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::manufacturerAccessHardware() {
  writeRegister(BQ20Z9xx_COMMAND.HardwareVersion.reg, BQ20Z9xx_COMMAND.HardwareVersion.system_data);
  return readRegister(BQ20Z9xx_COMMAND.HardwareVersion.reg);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the Battery Status.
 * Content determined by the Smart Battery's manufacturer. 
 * @return void
 */
void ArduinoSMBus::manufacturerStatus() {
  writeRegister(BQ20Z9xx_COMMAND.ManufacturerStatus.reg, BQ20Z9xx_COMMAND.ManufacturerStatus.system_data);
  BQ20Z9xx_COMMAND.manufacturerstatus.raw = readRegister(BQ20Z9xx_COMMAND.ManufacturerStatus.reg); // we only are interested in the upper 8 bits if ((status.raw & 0x0f) == 0) status.failure = "Wake Up.";
  return;
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Brings the IC in a Sealed condition.
 * Content determined by the Smart Battery's manufacturer. For the bq20z90/bq20z95 used to limit access to the extended SBS functions and data flash space, sets.
 * This command is only available when the bq20z90/bq20z95 is in Unsealed or Full Access mode.
 * @return void
 */
void ArduinoSMBus::manufacturerAccessSeal() {
  writeRegister(BQ20Z9xx_COMMAND.SealDevice.reg, BQ20Z9xx_COMMAND.SealDevice.system_data);
}

/**
 * @brief Get the battery's remaining capacity alarm.
 * Returns the battery's remaining capacity alarm threshold value, in mAh.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::remainingCapacityAlarm() {
  return readRegister(SBS_COMMAND.RemainingCapacityAlarm.reg);
}

/**
 * @brief Get the battery's remaining time alarm.
 * Returns the battery's remaining time alarm threshold value, in minutes.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::remainingTimeAlarm() {
  return readRegister(SBS_COMMAND.RemainingTimeAlarm.reg);
}

/**
 * @brief Get the battery's mode.
 * 
 * This method reads the battery's mode register, which contains various settings and status bits.
 * It sets batterymode fields based on the bits in the mode.
 * 
 * @return void
 */
void ArduinoSMBus::batteryMode() {
  SBS_COMMAND.batterymode.raw = readRegister(SBS_COMMAND.BatteryMode.reg); /**> Read the raw data battery mode from the device. */;
  return;
}

/**
 * @brief First half of a two-function call-set used to set the AtRate value used in calculations made by the AtRateTimeToFull(), AtRateTimeToEmpty(), and AtRateOK() functions.
 * The AtRate value is positive for charge, negative for discharge and zero for neither (default), may be expressed in either current (mA) or power (10mW) depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.
 * @return int16_t 
 */
int16_t ArduinoSMBus::atRate() {
  return readRegister(SBS_COMMAND.AtRate.reg);
}

/**
 * @brief Returns the predicted remaining time to fully charge the battery at the previously written AtRate value in mA.
 * Returns minutes. 65,535 indicates the battery is not being charged. If the CAPACITY_MODE bit is set, then AtRateTimeToFull() may return 65535 to indicate over-range and return an error code indicating overflow.  Alternately, this function may return a remaining time to full based on a 10 mW value in AtRate().
 * @return uint16_t
 */
uint16_t ArduinoSMBus::atRateTimeToFull() {
  return readRegister(SBS_COMMAND.AtRateTimeToFull.reg);
}

/**
 * @brief Returns the predicted remaining operating time if the battery is discharged at the previously written AtRate value. (Result will depend on the setting of CAPACITY_MODE bit.)
 * Returns minutes. 65,535 indicates the battery is not being discharged.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::atRateTimeToEmpty() {
  return readRegister(SBS_COMMAND.AtRateTimeToEmpty.reg);
}

/**
 * @brief Part of a two-function call-set used by power management systems to determine if the battery can safely supply enough energy for an additional load. 
 * Returns a Boolean value that indicates whether or not the battery can deliver the previously written AtRate value of additional energy for 10 seconds (Boolean).  If the AtRate value is zero or positive, the AtRateOK() function will ALWAYS return true.  Result may depend on the setting of CAPACITY_MODE bit.
 * @return bool
 */
bool ArduinoSMBus::atRateOK() {
  return readRegister(SBS_COMMAND.AtRate.reg);
}

/**
 * @brief Get the battery's temperature.
 * Returns the battery temperature in Kelvin.
 * @return float 
 */
float ArduinoSMBus::temperature() {
  return readRegister(SBS_COMMAND.Temperature.reg)/10;
}

/**
 * @brief Get the battery's temperature in Celsius.
 * Returns the battery temperature in 0.1 degrees Celsius.
 * @return float 
 */
float ArduinoSMBus::temperatureC() {
  float temperatureKelvin = readRegister(SBS_COMMAND.Temperature.reg)/10;
  return temperatureKelvin - 273.15;
}

/**
 * @brief Get the battery's temperature in Fahrenheit.
 * Returns the battery temperature in 0.1 degrees Fahrenheit.
 * @return float 
 */
float ArduinoSMBus::temperatureF() {
  float temperatureKelvin = readRegister(SBS_COMMAND.Temperature.reg)/10;
  return temperatureKelvin - 459.67;
}

/**
 * @brief Get the battery's voltage.
 * Returns the sum of all cell voltages, in mV.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::voltage() {
  return readRegister(SBS_COMMAND.Voltage.reg);
}

/**
 * @brief Get the battery's current.
 * Returns the battery measured current (from the coulomb counter) in mA.
 * @return uint16_t 
 */
int16_t ArduinoSMBus::current() {
  return readRegister(SBS_COMMAND.Current.reg);
}

/**
 * @brief Get the battery's average current.
 * Returns the average current in a 1-minute rolling average, in mA.
 * @return uint16_t 
 */
int16_t ArduinoSMBus::averageCurrent() {
  return readRegister(SBS_COMMAND.AverageCurrent.reg);
}

/**
 * @brief Get the battery's state of charge error.
 * Returns the battery's margin of error when estimating SOC, in percent
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::maxError() {
  return readRegister(SBS_COMMAND.MaxError.reg);
}

/**
 * @brief Get the battery's current relative charge.
 * Returns the predicted remaining battery capacity as a percentage of fullChargeCapacity()
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::relativeStateOfCharge() {
  uint16_t data = readRegister(SBS_COMMAND.RelativeStateOfCharge.reg);
  data &= 0x00ff;
  return data;
}

/**
 * @brief Get the battery's absolute charge.
 * Returns the predicted remaining battery capacity as a percentage of designCapacity()
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::absoluteStateOfCharge() {
  uint16_t data = readRegister(SBS_COMMAND.AbsoluteStateOfCharge.reg);
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
  return readRegister(SBS_COMMAND.RemainingCapacity.reg);
}

/**
 * @brief Get the battery's full capacity.
 * Returns the predicted battery capacity when fully charged, in mAh.
 * For some batteries, this may be in 10s of mWh, if the BatteryMode() register (0x03) is set that way
 * See protocol documentation for details.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::fullCapacity() {
  return readRegister(SBS_COMMAND.FullChargeCapacity.reg);
}

/**
 * @brief Get the battery's time to empty.
 * Returns the predicted time to empty, in minutes, based on current instantaneous discharge rate.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::runTimeToEmpty() {
  return readRegister(SBS_COMMAND.RunTimeToEmpty.reg);
}

/**
 * @brief Get the battery's average time to empty.
 * Returns the predicted time to empty, in minutes, based on 1-minute rolling average discharge rate.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::avgTimeToEmpty() {
  return readRegister(SBS_COMMAND.AverageTimeToEmpty.reg);
}

/**
 * @brief Get the battery's time to full.
 * Returns the predicted time to full charge, in minutes, based on 1-minute rolling average charge rate.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::avgTimeToFull() {
  return readRegister(SBS_COMMAND.AverageTimeToFull.reg);
}

/**
 * @brief Get the battery's design charging current.
 * Returns the desired design charging current of the battery, in mA.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::chargingCurrent() {
  return readRegister(SBS_COMMAND.ChargingCurrent.reg);
}

/**
 * @brief Get the battery's design charging voltage.
 * Returns the desired design charging voltage of the battery, in mV.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::chargingVoltage() {
  return readRegister(SBS_COMMAND.ChargingVoltage.reg);
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
void ArduinoSMBus::batteryStatus() {
  SBS_COMMAND.batterystatus.raw = readRegister(SBS_COMMAND.BatteryStatus.reg);
  return;
}

/**
 * @brief  Get the battery's cycle count.
 * Returns the number of discharge cycles the battery has experienced.
 * A cycle is defined as an amount of discharge equal to the battery's design capacity.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::cycleCount() {
  return readRegister(SBS_COMMAND.CycleCount.reg);
}

/**
 * @brief Get the battery's design capacity.
 * Returns the theoretical maximum capacity of the battery, in mAh.
 * For some batteries, this may be in 10 mWh, if the BatteryMode() register (0x03) is set to CAPM 1.
 * See TI protocol documentation for details.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::designCapacity() {
  return readRegister(SBS_COMMAND.DesignCapacity.reg);
}

/**
 * @brief Get the battery's design voltage.
 * Returns the nominal voltage of the battery, in mV.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::designVoltage() {
  return readRegister(SBS_COMMAND.DesignVoltage.reg);
}

/**
 * @brief  Get the battery's supported smart battery specification.
 * Returns the Version and revision : 
 * Day + Month*32 + (Year–1980)*512
 * @return String 
 */
String ArduinoSMBus::specificationInfo() {
  uint16_t data = readRegister(SBS_COMMAND.SpecificationInfo.reg);
  data &= 0x00ff;
  String info = String((data & 0xf0) >> 4);
  info += ".";
  info += String(data & 0x0f);
  return info;
}

/**
 * @brief  Get the battery's manufacture date.
 * Returns the date the battery was manufactured, in the following format: 
 * Day + Month*32 + (Year–1980)*512
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::manufactureDate() {
  return readRegister(SBS_COMMAND.ManufactureDate.reg);
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
  return readRegister(SBS_COMMAND.SerialNumber.reg);
}

/**
 * @brief Get the Manufacturer Name from the battery.
 * 
 * @return const char* 
 */
const char* ArduinoSMBus::manufacturerName() {
  static char data[BLOCKLENGTH]; // 20 characters plus null terminator
  readBlock(SBS_COMMAND.ManufacturerName.reg, reinterpret_cast<uint8_t*>(data), BLOCKLENGTH-2);
  data[BLOCKLENGTH-1] = '\0'; // Null-terminate the C-string
  return data;
}

/**
 * @brief Get the Device Name from the battery.
 * 
 * @return const char* 
 */
const char* ArduinoSMBus::deviceName() {
  static char data[BLOCKLENGTH];
  readBlock(SBS_COMMAND.DeviceName.reg, reinterpret_cast<uint8_t*>(data), 7);
  data[BLOCKLENGTH-1] = '\0'; // Null-terminate the C-string
  return data;
}

/**
 * @brief Get the Device Chemistry from the battery.
 * 
 * @return const char* 
 */
const char* ArduinoSMBus::deviceChemistry() {
  static char data[BLOCKLENGTH];
  readBlock(SBS_COMMAND.DeviceChemistry.reg, reinterpret_cast<uint8_t*>(data), 4);
  data[BLOCKLENGTH-1] = '\0';
  return data;
}

/**
 * @brief Get the battery's cell 4 voltage.
 * Returns the nominal voltage of cell 4, in mV.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::voltageCellFour() {
  return readRegister(SBS_COMMAND.OptionalMfgFunction4.reg);
}

/**
 * @brief Get the battery's cell 3 voltage.
 * Returns the nominal voltage of cell 3, in mV.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::voltageCellThree() {
  return readRegister(SBS_COMMAND.OptionalMfgFunction3.reg);
}

/**
 * @brief Get the battery's cell 2 voltage.
 * Returns the nominal voltage of cell 2, in mV.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::voltageCellTwo() {
  return readRegister(SBS_COMMAND.OptionalMfgFunction2.reg);
}

/**
 * @brief Get the battery's cell 1 voltage.
 * Returns the nominal voltage of cell 1, in mV.
 * @return uint16_t
 */
uint16_t ArduinoSMBus::voltageCellOne() {
  return readRegister(SBS_COMMAND.OptionalMfgFunction1.reg);
}

/**
 * @brief Get the manufacturer data.
 * Returns a struct with the manufacturer data.
 * @return void 
 */
void ArduinoSMBus::manufacturerData() {
  readBlock(BQ20Z9xx_COMMAND.ManufacturerData.reg, reinterpret_cast<uint8_t*>(BQ20Z9xx_COMMAND.manufacturerdata.raw), 15);
  BQ20Z9xx_COMMAND.manufacturerdata.raw[15] = '\0';   
  return;
}

/**
 * @brief Get the current FET Status from the battery.
 * fills a fetcontrol union 
 * This command is not supported by all batteries.
 * @return void 
 */
void ArduinoSMBus::FETControl() {
  BQ20Z9xx_COMMAND.fetcontrol.raw = readRegister(BQ20Z9xx_COMMAND.FETControl.reg);
  return; 
}

/**
 * @brief Get the State of Health from the battery.
 * Returns the estimated health of the battery, as a percentage of design capacity
 * This command is not supported by all batteries.
 * @return uint16_t 
 */
uint16_t ArduinoSMBus::stateOfHealth() {
  return readRegister(BQ20Z9xx_COMMAND.StateOfHealth.reg);
}

/**
 * @brief Returns indications of pending safety issues.
 * This read-word function fills a safetyalert union indicating pending safety issues, such as running safety timers, or fail 
 * counters that are nonzero but have not reached the required time or value to trigger a SafetyStatus failure.
 * This command is not supported by all batteries.
 * @return void 
 */
void ArduinoSMBus::Safetyalert() {
  BQ20Z9xx_COMMAND.safetyalert.raw = readRegister(BQ20Z9xx_COMMAND.SafetyAlert.reg);
  return;
}

/**
 * @brief Fills a safetystaus union with the status of the 1st level safety features.
 * This command is not supported by all batteries.
 * @return void
 */
void ArduinoSMBus::Safetystatus() {
  BQ20Z9xx_COMMAND.safetystatus.raw = readRegister(BQ20Z9xx_COMMAND.SafetyStatus.reg);
  return;
}

/**
 * @brief Fills a pfalert union indicating pending safety issues.
 * This read-word function returns indications of pending safety issues, such as running safety timers that
 * have not reached the required time to trigger a PFAlert failure
 * @return void 
 */
void ArduinoSMBus::PFalert() {
  BQ20Z9xx_COMMAND.pfalert.raw = readRegister(BQ20Z9xx_COMMAND.PFAlert.reg);
  return;
}

/**
 * @brief Fills a pfstatus with data from the permanent failure status register.
 * Returns the source of the bq20z90/bq20z95 permanent-failure condition.
 * This command is not supported by all batteries.
 * @return void
 */
void ArduinoSMBus::PFstatus() {
  BQ20Z9xx_COMMAND.pfstatus.raw = readRegister(BQ20Z9xx_COMMAND.PFStatus.reg);
  return;
}

/**
 * @brief fills an operationstatus union with the current Operation Status from the battery.
 *  
 * This command is not supported by all batteries.
 * @return void 
 */
void ArduinoSMBus::Operationstatus() {
  BQ20Z9xx_COMMAND.operationstatus.raw = readRegister(BQ20Z9xx_COMMAND.OperationStatus.reg);
  return;
}

/**
 * @brief Read Unseal Key.
 * The bq20z90/bq20z95 Unseal Key can be read when in full access mode 
 * The order of the bytes, when entered in ManufacturerAccess, is the 
 * reverse of what is written to or read from the part. For example, 
 * if the 1st and 2nd word of the UnSealKey block read returns 0x1234 and 
 * 0x5678, then in ManufacturerAccess, 0x3412 and 0x7856 should be entered 
 * to unseal the part.
 * Related Variables:
 * • SBS:UnsealKEy(0x60)
 * @param void
 * @return uint32_t 
 */
uint32_t ArduinoSMBus::unsealKey() {
  uint8_t data[4]{0};
  readBlock(BQ20Z9xx_COMMAND.UnSealKey.reg, data, 4);
  uint32_t key{0};
  key &= data[0] << 16;
  key &= data[1] << 24;
  key &= data[2];
  key &= data[3] << 8;
  return key;
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
void ArduinoSMBus::ClearPermanentFailure(uint16_t a, uint16_t b) {
  writeRegister(BQ20Z9xx_COMMAND.PermanentFailClear.reg, a);
  writeRegister(BQ20Z9xx_COMMAND.PermanentFailClear.reg, b);
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
void ArduinoSMBus::writeRegister(uint8_t reg, uint16_t data) {
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
void ArduinoSMBus::readBlock(uint8_t reg, uint8_t* data, uint8_t length) {
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
