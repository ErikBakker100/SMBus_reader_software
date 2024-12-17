#include "BQ20Z9xx.h"

bq20z9xx::bq20z9xx(uint8_t address) : smbuscommands(address) {
  info.push_back({static_cast<psmbcommand<smbuscommands>>(0x00, &bq20z9xx::manufacturerAccessType, DEVICEINFO, "manufacturerAccessType()")});
//  bq20z9xx_info.push_back({0x00, &smbuscommands::manufacturerAccessType, DEVICEINFO, "manufacturerAccessType()"});

/*
  command DeviceType {"DeviceType()", 0x00, 0x0001, DEVICEINFO};
  command FirmwareVersion {"FirmwareVersion()", 0x00, 0x0002, DEVICEINFO};
  command HardwareVersion {"HardwareVersion()", 0x00, 0x0003, DEVICEINFO};
  command ManufacturerStatus {"ManufacturerStatus()", 0x00, 0x0006, DEVICEINFO};
  command ChemistryID {"ChemistryID()", 0x00, 0x0008, DEVICEINFO};
  command Shutdown {"Shutdown()", 0x00, 0x0010, SET}; // Instructs the bq20z90/bq20z95 to verify and enter shutdown mode.
  command Sleep {"Sleep()", 0x00, 0x0011, SET}; // Instructs the bq20z90/bq20z95 to verify and enter sleep mode if no other command is sent after the Sleep command.
  command SealDevice {"SealDevice()", 0x00, 0x0020, SET}; 
  command PermanentFailClear {"PermanentFailClear(PFKey)", 0x00, 0x0000, SET}; 
  command UnsealDevice {"UnsealDevice()", 0x00, 0x0000, SET}; Instructs the bq20z90/bq20z95 to enable access to the SBS functions and data flash space and clear the [SS] flag.
  This 2 step command needs to be written to ManufacturerAccess in the following order: 1st word of the UnSealKey first followed by the 2nd word of the UnSealKey.
  If the command fails 4 seconds must pass before the command can be reissued.
  command FullAccessDevice {"FullAccessDevice()", 0x00, 0x0000, SET}; 
  command FETControl {"FETControl()", 0x46, 0x0000, SET};
    command StateOfHealth {"StateOfHealth()", 0x4f, 0x0000, DEVICEINFO};
  command SafetyAlert {"SafetyAlert()", 0x50, 0x0000, DEVICEINFO};
    command PFAlert {"PFAlert()", 0x52, 0x0000, DEVICEINFO};  
  command PFStatus {"PFStatus()", 0x53, 0x0000, DEVICEINFO};
  command OperationStatus {"OperationStatus()", 0x54, 0x0000, DEVICEINFO};
  command UnSealKey {"UnSealKey()", 0x60, 0x0000, SET};
*/
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the last digits of the IC part number.
 * Content determined by the Smart Battery's manufacturer.
 * • SBS:ManufacturerAccess(0x00)
 * @return uint16_t
 */
uint16_t bq20z9xx::manufacturerAccessType() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSTYPE);
  return readRegister(MANUFACTURERACCESS);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the Firmware version.
 * Content determined by the Smart Battery's manufacturer. The format is most-significant byte (MSB) = Decimal integer, and the
 * least-significant byte (LSB) = sub-decimal integer, e.g.: 0x0120 = version 01.20.
 * • SBS:ManufacturerAccess(0x00)
 * @return uint16_t
 */
uint16_t bq20z9xx::manufacturerAccessFirmware() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSFIRMWARE);
  return readRegister(MANUFACTURERACCESS);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the Hardware version.
 * Content determined by the Smart Battery's manufacturer. Returns the hardware version stored in a single byte of reserved data flash.
 * • SBS:ManufacturerAccess(0x00)
 * @return uint16_t
 */
uint16_t bq20z9xx::manufacturerAccessHardware() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSHARDWARE);
  return readRegister(MANUFACTURERACCESS);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the Battery Status.
 * Content determined by the Smart Battery's manufacturer. fills a manufacturerstatus union.
 * This function is available while the bq20z90/bq20z95 is in normal operation. This 16-bit word reports the battery status.
 * • SBS:ManufacturerAccess(0x00)
 * @return uint16_t
 */
uint16_t bq20z9xx::manufacturerStatus() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSTATUS);
  manufacturerstatus.raw = readRegister(MANUFACTURERACCESS);
  return manufacturerstatus.raw;
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Brings the IC in a Sealed condition.
 * Content determined by the Smart Battery's manufacturer. For the bq20z90/bq20z95 used to limit access to the extended SBS functions and data flash space, sets.
 * This command is only available when the bq20z90/bq20z95 is in Unsealed or Full Access mode.
 * • SBS:ManufacturerAccess(0x00)
 * @return 0
 */
uint16_t bq20z9xx::manufacturerAccessSeal() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSSEAL);
  return 0;
}

/**
 * @brief Clear bq20z90/bq20z95 permanent failure.
 * The bq20z90/bq20z95 permanent failure can be cleared by sending 
 * two ManufacturerAccess commands in sequence: the first word of the 
 * PFKey followed by the second word of the PFKey. After sending 
 * these two commands in sequence, PFStatus flags are cleared. 
 * Refer to Permanent Fail Clear (PFKey) Manufacturer access for further details.
 * This 2 step command needs to be written to ManufacturerAccess in following order: 1st word of the PFKey
 * first followed by the 2nd word of the PFKey. If the command fails 4 seconds must pass before the command can be reissued. It instructs the bq20z90/bq20z95 to clear
 * the PFStatus, clear the [PF] flag, clear the Fuse Flag, reset the SAFE pin, and unlock the data flash for writes.
 * Related Variables:
 * • SBS:ManufacturerAccess(0x00)
 * @param void
 * @return 0
 */
uint16_t bq20z9xx::manufacturerAccessPermanentFailClear(uint16_t a, uint16_t b) {
  writeRegister(MANUFACTURERACCESS, a);
  writeRegister(MANUFACTURERACCESS, b);
  return 0;
}

/**
 * @brief implementation specific. Used to get into Sealed mode or Full Access
 * Content determined by the Smart Battery's manufacturer. TI default unseal codes are 0x0414 0x3672 and the default full access codes are ffff ffff.
 * Unsealing is a 2 step command performed by writing the 1st word of the UnSealKey followed by the second word of the UnSealKey.
 * Changing from Unsealed to Full Access is performed by writing the 1st word of the FullAccessKey followed by the second word of the FullAccessKey.
 * This command is only available when the bq20z90/bq20z95 is in Sealed mode.
 * • SBS:ManufacturerAccess(0x00)
 * @return 0
 */
uint16_t bq20z9xx::manufacturerAccessUnseal(uint16_t UnSealKey_a, uint16_t UnSealKey_b) {
  writeRegister(MANUFACTURERACCESS, UnSealKey_a);
  writeRegister(MANUFACTURERACCESS, UnSealKey_b);
  return 0;
}

/**
 * @brief implementation specific. Used to get into Sealed mode or Full Access
 * Instructs the bq20z90/bq20z95 to enable full access to all SBS functions and data flash space and set the
 * [FAS] flag. This 2 step command needs to be written to ManufacturerAccess in the following order: 1st word of the FullAccessKey first followed by the 2nd word of the FullAccessKey.
 * • SBS:ManufacturerAccess(0x00)
 * @return 0
 */
uint16_t bq20z9xx::manufacturerAccessFullAccess(uint16_t Key_a, uint16_t Key_b) {
  writeRegister(MANUFACTURERACCESS, Key_a);
  writeRegister(MANUFACTURERACCESS, Key_b);
  return 0;
}

/**
 * @brief Get the data from the bq20z9xxx and fill a manufacturerdata union.
 * • SBS:manufacturerData(0x23)
 * @return char* 
 */
char* bq20z9xx::manufacturerData() {
  readBlock(MANUFACTURERDATA, reinterpret_cast<uint8_t*>(manufacturerdata.raw), 15);
  manufacturerdata.raw[15] = '\0'; // Null-terminate the C-string
  return manufacturerdata.raw;
}

/**
 * @brief Get the current FET Status from the battery.
 * fills a fetcontrol union, This read- or write-word function allows direct control of the FETs for test purposes.
 * This command is not supported by all batteries.
 * • SBS:FETControl(0x46)
 * @return void 
 */
uint16_t bq20z9xx::fetControl() {
  fetcontrol.raw = readRegister(FETCONTROL);
  return fetcontrol.raw; 
}

/**
 * @brief Get the State of Health from the battery.
 * Returns the estimated health of the battery, as a percentage of design capacity. This read word function returns the state of health of the battery in %.
 * This command is not supported by all batteries.
 * • SBS:stateOfHealth(0x4f)
 * @return uint16_t 
 */
uint16_t bq20z9xx::stateOfHealth() {
  return readRegister(STATEOFHEALTH);
}

/**
 * @brief Returns indications of pending safety issues.
 * This read-word function fills a safetyalert union indicating pending safety issues, such as running safety timers, or fail 
 * counters that are nonzero but have not reached the required time or value to trigger a SafetyStatus failure.
 * This command is not supported by all batteries.
 * • SBS:Safetyalert(0x50)
 * @return uint16_t 
 */
uint16_t bq20z9xx::safetyAlert() {
  safetyalert.raw = readRegister(SAFETYALERT);
  return safetyalert.raw;
}

/**
 * @brief Fills a safetystaus union with the status of the 1st level safety features.
 * This command is not supported by all batteries.
 * • SBS:Safetystatus(0x51)
 * @return uint16_t
 */
uint16_t bq20z9xx::safetyStatus() {
  safetystatus.raw = readRegister(SAFETYSTATUS);
  return safetystatus.raw;
}

/**
 * @brief Fills a pfalert union indicating pending safety issues.
 * This read-word function returns indications of pending safety issues, such as running safety timers that
 * have not reached the required time to trigger a PFAlert failure
 * • SBS:PFalert(0x52)
 * @return uint16_t 
 */
uint16_t bq20z9xx::pfAlert() {
  pfalert.raw = readRegister(PFALERT);
  return pfalert.raw;
}

/**
 * @brief Fills a pfstatus with data from the permanent failure status register.
 * Returns the source of the bq20z90/bq20z95 permanent-failure condition. The permanent failure status register indicates the source of the bq20z90/bq20z95 permanent-failure condition.
 * This command is not supported by all batteries.
 * • SBS:PFstatus(0x53)
 * @return uint16_t
 */
uint16_t bq20z9xx::pfStatus() {
  pfstatus.raw = readRegister(PFSTATUS);
  return pfstatus.raw;
}

/**
 * @brief fills an operationstatus union with the current Operation Status from the battery.
 * This read-word function returns the current operation status of the bq20z90/bq20z95.
 * This command is not supported by all batteries.
 * • SBS:Operationstatus(0x54)
 * @return uint16_t 
 */
uint16_t bq20z9xx::operationStatus() {
  operationstatus.raw = readRegister(OPERATIONSTATUS);
  return operationstatus.raw;
}

/**
 * @brief Read Unseal Key.
 * The bq20z90/bq20z95 Unseal Key can be read when in full access mode 
 * The order of the bytes, when entered in ManufacturerAccess, is the 
 * reverse of what is written to or read from the part. For example, 
 * if the 1st and 2nd word of the UnSealKey block read returns 0x1234 and 
 * 0x5678, then in ManufacturerAccess, 0x3412 and 0x7856 should be entered 
 * to unseal the part.
 * This read- or write-block command allows the user to change the Unseal key for the Sealed-to-Unsealed security-state transition.
 * This function is only available when the bq20z90/bq20z95 is in the Full-Access mode
 * Related Variables:
 * • SBS:UnsealKEy(0x60)
 * @param void
 * @return uint32_t 
 */
uint32_t bq20z9xx::unsealKey() {
  uint8_t data[4]{0};
  readBlock(UNSEALKEY, data, 4);
  uint32_t key{0};
  key &= data[0] << 16;
  key &= data[1] << 24;
  key &= data[2];
  key &= data[3] << 8;
  return key;
}

