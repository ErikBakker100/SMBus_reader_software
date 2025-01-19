#include "BQ40Z6xx.h"
#include <cstdlib>

bq40z6xx::bq40z6xx(uint8_t address) : smbuscommands(address) {
}

/**
 * @brief implementation specific.
 * Content determined by the Smart Battery's manufacturer.
 * • SBS:ManufacturerAccess(0x00)
 * @return uint16_t
 */
void bq40z6xx::manufacturerAccessType() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSTYPE);
    itoa(readRegister(MANUFACTURERACCESS), text, 16);
}

/**
 * @brief implementation specific.
 * Content determined by the Smart Battery's manufacturer. The format is most-significant byte (MSB) = Decimal integer, and the
 * least-significant byte (LSB) = sub-decimal integer, e.g.: 0x0120 = version 01.20.
 * • SBS:ManufacturerAccess(0x00)
 * @return fills the smbus::text array
 */
 void bq40z6xx::manufacturerAccessFirmware() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSFIRMWARE);
  manufacturerData();
}

/**
 * @brief implementation specific.
 * Content determined by the Smart Battery's manufacturer. Returns the hardware version stored in a single byte of reserved data flash.
 * • SBS:ManufacturerAccess(0x00)
 * @return fills the smbus::text array
 */
void bq40z6xx::manufacturerAccessHardware() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSHARDWARE);
  manufacturerData();
}

/**
 * @brief implementation specific.
 * @return fills the smbus::text array
 */
void bq40z6xx::manufacturerAccessChemistryID() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSCHEMISTRY);
  manufacturerData();
}

/**
 * @brief Shutdown To reduce power consumption, the device can be sent to SHUTDOWN mode before shipping. After
 * sending this command, OperationStatus()[SDM] sets to 1, an internal counter will start and the CHG and
 * DSG FETs will be turned off when the counter reaches Ship FET Off Time. When the counter reaches
 * Ship Delay time, the device will enter SHUTDOWN mode if no charger is detected.
 * If the device is sealed, this feature requires the command to be sent twice in a row within 4 seconds (for safety purposes).
 * To wake up the device, a voltage > Charger Present Threshold must apply to the VACP pin. The device
 * will power up and a full reset is applied.
 * @return none
 */
void bq40z6xx::manufacturerAccessShutdown(){
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSSHUTDOWN);
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSSHUTDOWN);
}

/**
 * @brief Sleep
 * @return none
 */
void bq40z6xx::manufacturerAccessSleep(){ // command 0x0011
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSSLEEP);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Brings the IC in a Sealed condition.
 * Content determined by the Smart Battery's manufacturer. For the bq20z90/bq20z95 used to limit access to the extended SBS functions and data flash space, sets.
 * This command is only available when the bq20z90/bq20z95 is in Unsealed or Full Access mode.
 * • SBS:ManufacturerAccess(0x00)
 * @return fills the smbus:text array
 */
void bq40z6xx::manufacturerAccessSeal() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSSEAL);
}

void bq40z6xx::manufacturerSecurityKeys() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSSECURITYKEYS);
  manufacturerData();
}

/**
 * @brief Returns indications of pending safety issues.
 * This function fills a safetyalert union indicating pending safety issues, such as running safety timers, or fail 
 * counters that are nonzero but have not reached the required time or value to trigger a SafetyStatus failure.
 * • SBS:Safetyalert(0x50)
 * @return fills the smbus::text array
 */
void bq40z6xx::safetyAlert() {
  readRegister(SAFETYALERT);
  manufacturerData();
}

/**
 * @brief Fills a safetystaus union with the status of the 1st level safety features.
 * This command is not supported by all batteries.
 * • SBS:Safetystatus(0x51)
 * @return fills the smbus::text array
 */
void bq40z6xx::safetyStatus() {
  readRegister(SAFETYSTATUS);
  manufacturerData();
}

/**
 * @brief Fills a pfalert union indicating pending safety issues.
 * This read-word function returns indications of pending safety issues, such as running safety timers that
 * have not reached the required time to trigger a PFAlert failure
 * • SBS:PFalert(0x52)
 * @return fills the smbus::text array 
 */
void bq40z6xx::pfAlert() {
  readRegister(PFALERT);
  manufacturerData();
}


/**
 * @brief Fills a pfstatus with data from the permanent failure status register.
 * Returns the source of the bq20z90/bq20z95 permanent-failure condition. The permanent failure status register indicates the source of the bq20z90/bq20z95 permanent-failure condition.
 * This command is not supported by all batteries.
 * • SBS:PFstatus(0x53)
 * @return fills the smbus::text array
 */
void bq40z6xx::pfStatus() {
  readRegister(PFSTATUS);
  manufacturerData();
}

/**
 * @brief fills an operationstatus union with the current Operation Status from the battery.
 * This read-word function returns the current operation status of the bq20z90/bq20z95.
 * This command is not supported by all batteries.
 * • SBS:Operationstatus(0x54)
 * @return fills the smbus::text array 
 */
void bq40z6xx::operationStatus() {
  readRegister(OPERATIONSTATUS);
  manufacturerData();
}

/**
 * @brief Clear bq permanent failure.
 * Related Variables:
 * • SBS:M
 * @param uint16_t, uint16_t 
 * @return void
 */
void bq40z6xx::manufacturerAccessPermanentFailClear(uint16_t a, uint16_t b) {
  writeRegister(MANUFACTURERACCESS, a);
  writeRegister(MANUFACTURERACCESS, b);
}

/**
 * @brief implementation specific. Used to get into Sealed mode or Full Access
 * Content determined by the Smart Battery's manufacturer. TI default unseal codes are 0x0414 0x3672 and the default full access codes are ffff ffff.
 * Unsealing is a 2 step command performed by writing the 1st word of the UnSealKey followed by the second word of the UnSealKey.
 * Changing from Unsealed to Full Access is performed by writing the 1st word of the FullAccessKey followed by the second word of the FullAccessKey.
 * This command is only available when the bq20z90/bq20z95 is in Sealed mode.
 * • SBS:ManufacturerAccess(0x00)
 * @param uint16_t, uint16_t 
 * @return void
 */
void bq40z6xx::manufacturerAccessUnseal(uint16_t Key_a, uint16_t Key_b) {
  writeRegister(MANUFACTURERACCESS, Key_a);
  writeRegister(MANUFACTURERACCESS, Key_b);
}

/**
 * @brief implementation specific. Used to get into Sealed mode or Full Access
 * Instructs the bq20z90/bq20z95 to enable full access to all SBS functions and data flash space and set the
 * [FAS] flag. This 2 step command needs to be written to ManufacturerAccess in the following order: 1st word of the FullAccessKey first followed by the 2nd word of the FullAccessKey.
 * • SBS:ManufacturerAccess(0x00)
 * @param uint16_t, uint16_t 
 * @return void
 */
void bq40z6xx::manufacturerAccessFullAccess(uint16_t Key_a, uint16_t Key_b) {
  writeRegister(MANUFACTURERACCESS, Key_a);
  writeRegister(MANUFACTURERACCESS, Key_b);
}

