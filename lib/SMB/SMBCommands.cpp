#include "SMBCommands.h"

smbuscommands::smbuscommands(uint8_t address) {
    batteryAddress = address;
  // list of the different SMB commands, including function pointers to these funtions. This to be able to call them via user input
    info.emplace_back([this]() {manufacturerAccess();}, 0x00, DEVICEINFO, "ManufacturerAccess()");
    info.emplace_back([this]() {remainingCapacityAlarm();},0x01, DEVICEINFO, "remainingCapacityAlarm()");
    info.emplace_back([this]() {remainingTimeAlarm();}, 0x02, DEVICEINFO, "remainingTimeAlarm()");
    info.emplace_back([this]() {batteryMode();}, 0x03, STATUSBITS, "batteryMode()");
    info.emplace_back([this]() {atRate();}, 0x04, ATRATES, "atRate()");
    info.emplace_back([this]() {atRateTimeToFull();}, 0x05, ATRATES, "atRateTimeToFull()");
    info.emplace_back([this]() {atRateTimeToEmpty();}, 0x06, ATRATES, "atRateTimeToEmpty()");
    info.emplace_back([this]() {atRateOK();}, 0x07, ATRATES, "atRateOK()");
    info.emplace_back([this]() {temperature();}, 0x08, USAGEINFO, "temperature()");
    info.emplace_back([this]() {voltage();}, 0x09, USAGEINFO, "voltage()");
    info.emplace_back([this]() {current();}, 0x0a, USAGEINFO, "current()");
    info.emplace_back([this]() {averageCurrent();}, 0x0b, USAGEINFO, "averageCurrent()");
    info.emplace_back([this]() {maxError();}, 0x0c, USAGEINFO, "maxError()");
    info.emplace_back([this]() {relativeStateOfCharge();}, 0x0d, COMPUTEDINFO, "relativeStateOfCharge()");
    info.emplace_back([this]() {absoluteStateOfCharge();}, 0x0e, COMPUTEDINFO, "absoluteStateOfCharge()");
    info.emplace_back([this]() {remainingCapacity();}, 0x0f, USAGEINFO, "remainingCapacity()");
    info.emplace_back([this]() {fullCapacity();}, 0x10, USAGEINFO, "fullCapacity()");
    info.emplace_back([this]() {runTimeToEmpty();}, 0x11, COMPUTEDINFO, "runTimeToEmpty()");
    info.emplace_back([this]() {avgTimeToEmpty();}, 0x12, COMPUTEDINFO, "avgTimeToEmpty()");
    info.emplace_back([this]() {avgTimeToFull();}, 0x13, COMPUTEDINFO, "avgTimeToFull()");
    info.emplace_back([this]() {chargingCurrent();}, 0x14, USAGEINFO, "chargingCurrent()");
    info.emplace_back([this]() {chargingVoltage();}, 0x15, USAGEINFO, "chargingVoltage()");
    info.emplace_back([this]() {batteryStatus();}, 0x16, STATUSBITS, "batteryStatus()");
    info.emplace_back([this]() {cycleCount();}, 0x17, USAGEINFO, "cycleCount()");
    info.emplace_back([this]() {designCapacity();}, 0x18, DEVICEINFO, "designCapacity()");
    info.emplace_back([this]() {designVoltage();}, 0x19, DEVICEINFO, "designVoltage()");
    info.emplace_back([this]() {specificationInfo();}, 0x1a, DEVICEINFO, "specificationInfo()");
    info.emplace_back([this]() {manufactureDate();}, 0x1b, DEVICEINFO, "manufactureDate()");
    info.emplace_back([this]() {serialNumber();}, 0x1c, DEVICEINFO, "serialNumber()");
    info.emplace_back([this]() {manufacturerName();}, 0x20, DEVICEINFO, "manufacturerName()");
    info.emplace_back([this]() {deviceName();}, 0x21, DEVICEINFO, "deviceName()");
    info.emplace_back([this]() {deviceChemistry();}, 0x22, DEVICEINFO, "deviceChemistry()");
    info.emplace_back([this]() {optionalMFGfunction4();}, 0x3c, DEVICEINFO, "optionalMFGfunction4()");
    info.emplace_back([this]() {optionalMFGfunction3();}, 0x3d, DEVICEINFO, "optionalMFGfunction3()");
    info.emplace_back([this]() {optionalMFGfunction2();}, 0x3e, DEVICEINFO, "optionalMFGfunction2()");
    info.emplace_back([this]() {optionalMFGfunction1();}, 0x3f, DEVICEINFO, "optionalMFGfunction1()");
}

int16_t smbuscommands::readRegister(uint8_t reg) {
  return smbus::readRegister(reg, batteryAddress);
}

void smbuscommands::writeRegister(uint8_t reg, uint16_t data) {
  smbus::writeRegister(reg, data, batteryAddress);
}

void smbuscommands::readBlock(uint8_t reg, uint8_t* data, uint8_t len) {
  smbus::readBlock(reg, data, len, batteryAddress);
}

/**
 * @brief 
 * @details This function is optional and its meaning is implementation specific.  It may be used by a battery
 * manufacturer or silicon supplier to return specific version information, internal calibration information, or some other manufacturer specific function.  There is no implied or 
 * required use for this function and therefore it may be used for multiple purposes.  The only requirement is the data protocol listed below: read word or write word.
 * @return uint16_t 
 */
uint16_t smbuscommands::manufacturerAccess() {
  return readRegister(MANUFACTURERACCESS);
}

/**
 * @brief Get the battery's remaining capacity alarm.
 * @details Sets or gets the Low Capacity alarm threshold value.  Whenever the RemainingCapacity() falls below the
 * Low Capacity value, the Smart Battery sends AlarmWarning() messages to the SMBus Host with the REMAINING_CAPACITY_ALARM bit set.  A Low Capacity value of 0 disables this alarm.
 * (If the ALARM_MODE bit is set in BatteryMode() then the AlarmWarning() message is disabled for a set period of time.
 * @return uint16_t 
 */
uint16_t smbuscommands::remainingCapacityAlarm() {
  return readRegister(REMAININGCAPACITYALARM);
}

/**
 * @brief Get the battery's remaining time alarm.
 * @details Sets or gets the Remaining Time alarm value. Whenever the AverageTimeToEmpty() falls below the Remaining Time value, the Smart Battery sends
 * AlarmWarning() messages to the SMBus Host with the REMAINING_TIME_ALARM bit set.  A Remaining Time value of 0 effectively disables this alarm. (If the ALARM_MODE bit is set in 
 * BatteryMode() then the AlarmWarning() message is disabled for a set period of time.
 * @return uint16_t 
 */
uint16_t smbuscommands::remainingTimeAlarm() {
  return readRegister(REMAININGTIMEALARM);
}

/**
 * @brief Get the battery's mode.
 * 
 * @details This function selects the various battery operational modes and reports the battery’s capabilities, modes, and flags minor conditions requiring attention.
 * @return uint16_t
 */
uint16_t smbuscommands::batteryMode() {
  batterymode.raw = readRegister(BATTERYMODE);
  return batterymode.raw;
}

/**
 * @brief First half of a two-function call-set used to set the AtRate value used in calculations made by the AtRateTimeToFull(), AtRateTimeToEmpty(), and AtRateOK() functions.
 * @details The AtRate() function is the first half of a two-function call-set used to set the AtRate value used in calculations made by the AtRateTimeToFull(),
 * AtRateTimeToEmpty(), and AtRateOK() functions.  The AtRate value may be expressed in either current (mA) or power (10mW) depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.
 * @return int16_t 
 */
int16_t smbuscommands::atRate() {
  return readRegister(ATRATE);
}

/**
 * @brief Returns the predicted remaining time to fully charge the battery at the previously written AtRate value in mA.
 * Returns minutes. 65,535 indicates the battery is not being charged. If the CAPACITY_MODE bit is set, then AtRateTimeToFull() may return 65535 to indicate over-range and return 
 * an error code indicating overflow.  Alternately, this function may return a remaining time to full based on a 10 mW value in AtRate().
 * @return uint16_t
 */
uint16_t smbuscommands::atRateTimeToFull() {
  return readRegister(ATRATETIMETOFULL);
}

/**
 * @brief Returns the predicted remaining operating time if the battery is discharged at the previously written AtRate value. (Result will depend on the setting of CAPACITY_MODE bit.)
 * Returns minutes. 65,535 indicates the battery is not being discharged.
 * @return uint16_t
 */
uint16_t smbuscommands::atRateTimeToEmpty() {
  return readRegister(ATRATETIEMTOEMPTY);
}

/**
 * @brief Part of a two-function call-set used by power management systems to determine if the battery can safely supply enough energy for an additional load. 
 * Returns a Boolean value that indicates whether or not the battery can deliver the previously written AtRate value of additional energy for 10 seconds (Boolean). If 
 * the AtRate value is zero or positive, the AtRateOK() function will ALWAYS return true. Result may depend on the setting of CAPACITY_MODE bit.
 * @return bool
 */
bool smbuscommands::atRateOK() {
  return readRegister(ATRATEOK);
}

/**
 * @brief Get the battery's temperature.
 * Returns the cell-pack's internal temperature (°K). The actual operational temperature range will be defined at a pack level 
 * by a particular manufacturer. Typically it will be in the range of -20°C to +75°C.
 * @return uint16_t 
 */
uint16_t smbuscommands::temperature() {
  return readRegister(TEMPERATURE);
}

/**
 * @brief Get the battery's temperature in Celsius.
 * Returns the battery temperature in 0.1 degrees Celsius.
 * @return float 
 */
float smbuscommands::temperatureC() {
  return temperature()/10 - 273.15;
}

/**
 * @brief Get the battery's temperature in Fahrenheit.
 * Returns the battery temperature in 0.1 degrees Fahrenheit.
 * @return float 
 */
float smbuscommands::temperatureF() {
  return temperature()/10 - 459.67;
}

/**
 * @brief Get the battery's voltage.
 * Returns the sum of all cell voltages, in mV.
 * @return uint16_t 
 */
uint16_t smbuscommands::voltage() {
  return readRegister(VOLTAGE);
}

/**
 * @brief Get the battery's current.
 * Returns the current being supplied (or accepted) through the battery's terminals (mA).
 * @return uint16_t 
 */
int16_t smbuscommands::current() {
  return readRegister(CURRENT);
}

/**
 * @brief Get the battery's average current.
 * Returns a one-minute rolling average based on the current being supplied (or accepted) through the battery's
 * terminals (mA).  The AverageCurrent() function is expected to return meaningful values during the battery's first minute of operation.
 * @return uint16_t 
 */
int16_t smbuscommands::averageCurrent() {
  return readRegister(AVERAGECURRENT);
}

/**
 * @brief Get the battery's state of charge error.
 * Returns the expected margin of error (%) in the state of charge calculation.
 * @return uint16_t 
 */
uint16_t smbuscommands::maxError() {
  return readRegister(MAXERROR);
}

/**
 * @brief Get the battery's current relative charge.
 * Returns the predicted remaining battery capacity expressed as a percentage of FullChargeCapacity() (%).
 * @return uint16_t 
 */
uint16_t smbuscommands::relativeStateOfCharge() {
  uint16_t data = readRegister(RELATIVESTATEOFCHARGE);
  data &= 0x00ff;
  return data;
}

/**
 * @brief Get the battery's absolute charge.
 * Returns the predicted remaining battery capacity expressed as a percentage of DesignCapacity() (%).  Note
 * that AbsoluteStateOfCharge() can return values greater than 100%.
 * @return uint16_t 
 */
uint16_t smbuscommands::absoluteStateOfCharge() {
  uint16_t data = readRegister(ABSOLUTESTATEOFCHARGE);
  data &= 0x00ff;
  return data;
}

/**
 * @brief Get the battery's capacity.
 * Returns the predicted remaining battery capacity.  The RemainingCapacity() capacity value is expressed in
 * either current (mAh at a C/5 discharge rate) or power (10mWh at a P/5 discharge rate) depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.
 * @return uint16_t 
 */
uint16_t smbuscommands::remainingCapacity() {
  return readRegister(REMAININGCAPACITY);
}

/**
 * @brief Get the battery's full capacity.
 * Returns the predicted pack capacity when it is fully charged.  The FullChargeCapacity() value is expressed
 * in either current (mAh at a C/5 discharge rate) or power (10mWh at a P/5 discharge rate) depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.
 * @return uint16_t 
 */
uint16_t smbuscommands::fullCapacity() {
  return readRegister(FULLCAPACITY);
}

/**
 * @brief Get the battery's time to empty.
 * Returns the predicted remaining battery life at the present rate of discharge (minutes).  The RunTimeToEmpty() 
 * value is calculated based on either current or power depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.
 * @return uint16_t 
 */
uint16_t smbuscommands::runTimeToEmpty() {
  return readRegister(RUNTIMETOEMPTY);
}

/**
 * @brief Get the battery's average time to empty.
 * Returns a one-minute rolling average of the predicted remaining battery life (minutes).  The AverageTimeToEmpty() 
 * value is calculated based on either current or power depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.
 * @return uint16_t 
 */
uint16_t smbuscommands::avgTimeToEmpty() {
  return readRegister(AVGTIMETOEMPTY);
}

/**
 * @brief Get the battery's time to full.
 * Returns a one minute rolling average of the predicted remaining time until the Smart Battery reaches full charge (minutes).
 * @return uint16_t 
 */
uint16_t smbuscommands::avgTimeToFull() {
  return readRegister(AVGTIMETOFULL);
}

/**
 * @brief Get the battery's design charging current.
 * Sends the desired charging rate to the Smart Battery Charger (mA). This represents the maximum current which may be
 * provided by the Smart Battery Charger to permit the Smart Battery to reach a Fully Charged state.
 * @return uint16_t 
 */
uint16_t smbuscommands::chargingCurrent() {
  return readRegister(CHARGINGCURRENT);
}

/**
 * @brief Get the battery's design charging voltage.
 * Sends the desired charging voltage to the Smart Battery Charger (mV). This represents the maximum voltage which may
 * be provided by the Smart Battery Charger to permit the Smart Battery to reach a Fully Charged state.
 * @return uint16_t 
 */
uint16_t smbuscommands::chargingVoltage() {
  return readRegister(CHARGINGVOLTAGE);
}

/**
 * @brief Get the battery's status.
 * 
 * This function reads the BatteryStatus register and returns a struct with its value.
 * The BatteryStatus register indicates various alarm conditions and states of the battery.
 * These include over charge, termination charge, over temperature, termination discharge,
 * remaining capacity, remaining time, initialization, discharging, fully charged, and fully discharged states.
 * Returns the Smart Battery's status word  which contains Alarm and Status bit flags.  Some of the BatteryStatus() 
 * flags (REMAINING_CAPACITY_ALARM and REMAINING_TIME_ALARM) are calculated based on either current or power depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.
 * @return uint16_t, individual flags can be accessed via 'batterystatus' struct containing the status of each bit in the BatteryStatus register.
 */
uint16_t smbuscommands::batteryStatus() {
  batterystatus.raw = readRegister(BATTERYSTATUS);
  return batterystatus.raw;
}

/**
 * @brief  Get the battery's cycle count.
 * Returns the number of discharge cycles the battery has experienced.
 * A cycle is defined as an amount of discharge equal to the battery's design capacity.
 * @return uint16_t 
 */
uint16_t smbuscommands::cycleCount() {
  return readRegister(CYCLECOUNT);
}

/**
 * @brief Get the battery's design capacity.
 * Returns the theoretical capacity of a new pack.  The DesignCapacity() value is expressed in either current (mAh at 
 * a C/5 discharge rate) or power (10mWh at a P/5 discharge rate) depending on the setting of the BatteryMode()'s CAPACITY_MODE bit.
 * @return uint16_t 
 */
uint16_t smbuscommands::designCapacity() {
  return readRegister(DESIGNCAPACITY);
}

/**
 * @brief Get the battery's design voltage.
 * Returns the theoretical voltage of a new pack (mV).
 * @return uint16_t 
 */
uint16_t smbuscommands::designVoltage() {
  return readRegister(DESIGNVOLTAGE);
}

/**
 * @brief  Get the battery's supported smart battery specification.
 * Returns the version number of the Smart Battery specification the battery pack supports, as well as voltage and current and capacity scaling information
 * in a packed unsigned integer. Power scaling is the product of the voltage scaling times the current scaling. The latter 2 are ot supported.  
 * @return const char* 
 */
  char* smbuscommands::specificationInfo() {
  uint16_t data = readRegister(SPECIFICATIONINFO);
  data = (data >> 4) & 0x000f;
  static char* info;
  switch (data) {
    case 1:
      info = new char [sizeof("1.0")+1] {'1', '.', '0', '\0'};
      break;
    case 2:
      info = new char [sizeof("1.1")+1] {'1', '.', '1', '\0'};
      break;
    case 3:
      info = new char [sizeof("1.1 with optional PEC")+1] {'1', '.', '1', ' ', 'w', 'i', 't', 'h', ' ', 'o', 'p', 't', 'i', 'o', 'n', 'a', 'l', ' ', 'P', 'E', 'C', '\0'};
  }
  return info;
}

/**
 * @brief  Get the battery's manufacture date.
 * This function returns the date the cell pack was manufactured in a packed integer.  The date is packed in the
 * following fashion: (year-1980) * 512 + month * 32 + day.
 * @return uint16_t 
 */
uint16_t smbuscommands::manufactureDate() {
  return readRegister(MANUFACTURERDATE);
}

/**
 * @brief Get the manufacture day from the manufacture date.
 * @return uint16_t
 */
uint16_t smbuscommands::manufactureDay() {
  return (manufactureDate() & 0xf);
}
/**
 * @brief Get the manufacture month from the manufacture date.
 * @return uint16_t
 */
uint16_t smbuscommands::manufactureMonth() {
  return ((manufactureDate() >> 5 ) & 0xf);
}
/**
 * @brief Get the manufacture year from the manufacture date.
 * @return uint16_t 
 */
uint16_t smbuscommands::manufactureYear() {
  uint16_t date = manufactureDate();
  return ((date >> 9) & 0x7f) + 1980;
}

/**
 * @brief Get the Serial Number from the battery.
 * This function is used to return a serial number.  This number when combined with the ManufacturerName(), the DeviceName(), 
 * and the ManufactureDate() will uniquely identify the battery (unsigned int).
 * @return uint16_t 
 */
uint16_t smbuscommands::serialNumber() {
  return readRegister(SERIALNUMBER);
}

/**
 * @brief Get the Manufacturer Name from the battery.
 * This function returns a character array containing the battery's manufacturer's name.  For example,
 * "MyBattCo" would identify the Smart Battery's manufacturer as MyBattCo.
 * @return const char* 
 */
  char* smbuscommands::manufacturerName() {
  static char data[BLOCKLENGTH]; // 20 characters plus null terminator
  readBlock(MANUFACTURERNAME, reinterpret_cast<uint8_t*>(data), BLOCKLENGTH-2);
  data[BLOCKLENGTH-1] = '\0'; // Null-terminate the C-string
  return data;
}

/**
 * @brief Get the Device Name from the battery.
 * This function returns a character string that contains the battery's name.  For example, a DeviceName() of "MBC101" 
 * would indicate that the battery is a model MBC101.
 * @return const char* 
 */
  char* smbuscommands::deviceName() {
  static char data[BLOCKLENGTH];
  readBlock(DEVICENAME, reinterpret_cast<uint8_t*>(data), 7);
  data[BLOCKLENGTH-1] = '\0'; // Null-terminate the C-string
  return data;
}

/**
 * @brief Get the Device Chemistry from the battery.
 * This function returns a character string that contains the battery's chemistry.  For example, if the
 * DeviceChemistry() function returns "NiMH," the battery pack would contain nickel metal hydride cells.
 * @return const char* 
 */
  char* smbuscommands::deviceChemistry() {
  static char data[BLOCKLENGTH];
  readBlock(DEVICECHEMISTRY, reinterpret_cast<uint8_t*>(data), 4);
  data[BLOCKLENGTH-1] = '\0';
  return data;
}

/**
 * @brief Get the manufacturer data.
 * Implementation dependant, can be overridden.
 * @return char*
 */
char* smbuscommands::manufacturerData() {
  static char data[BLOCKLENGTH];
  readBlock(MANUFACTURERDATA, reinterpret_cast<uint8_t*>(data), 15);
  data[BLOCKLENGTH-1] = '\0';
  return data;
}

/**
 * @brief Get the battery's cell 4 voltage.
 * Returns the nominal voltage of cell 4, in mV.
 * @return uint16_t
 */
uint16_t smbuscommands::optionalMFGfunction4() {
  return readRegister(OPTIONALMFGFUNCTION4);
}

/**
 * @brief Get the battery's cell 3 voltage.
 * Returns the nominal voltage of cell 3, in mV.
 * @return uint16_t
 */
uint16_t smbuscommands::optionalMFGfunction3() {
  return readRegister(OPTIONALMFGFUNCTION3);
}

/**
 * @brief Get the battery's cell 2 voltage.
 * Returns the nominal voltage of cell 2, in mV.
 * @return uint16_t
 */
uint16_t smbuscommands::optionalMFGfunction2() {
  return readRegister(OPTIONALMFGFUNCTION2);
}

/**
 * @brief Get the battery's cell 1 voltage.
 * Returns the nominal voltage of cell 1, in mV.
 * @return uint16_t
 */
uint16_t smbuscommands::optionalMFGfunction1() {
  return readRegister(OPTIONALMFGFUNCTION1);
}

uint8_t smbuscommands::address() {
  return batteryAddress;
};