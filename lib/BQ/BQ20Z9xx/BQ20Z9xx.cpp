#include "BQ20Z9xx.h"
#include <cstdlib>

bq20z9xx::bq20z9xx() {
}

int16_t bq20z9xx::readRegister(uint8_t reg) {
  return readRegister(reg);
}

void bq20z9xx::writeRegister(uint8_t reg, uint16_t data) {
  writeRegister(reg, data);
}

void bq20z9xx::readBlock(uint8_t reg) {
  readBlock(reg);
}

/**
 * @brief implementation specific. For TI bq20z90/bq20z95 Returns the last digits of the IC part number.
 * Content determined by the Smart Battery's manufacturer.
 * • SBS:ManufacturerAccess(0x00)
 * @param none
 * @return fills the smbus::text array
 */
void bq20z9xx::manufacturerAccessType() {
  writeRegister(MANUFACTURERACCESS, MANUFACTURERACCESSTYPE);
  itoa(readRegister(MANUFACTURERACCESS), smbus::text, 16);
  text[2] = 0;
}
