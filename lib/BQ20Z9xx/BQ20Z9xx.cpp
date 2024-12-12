#include "BQ20Z9xx.h"
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
