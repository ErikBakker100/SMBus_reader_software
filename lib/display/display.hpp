#pragma once

#define TAB1 5
#define TAB2 40
#define TAB3 70

#include <Arduino.h>
#include "../ansi/ansi.h"
#include "../BQ/BQ20Z9xx.h"
#include "../i2cscanner/i2cscanner.h"

static ANSI ansi(&Serial);

void displayMainmenu();
void displaySmallmenu();
void displayBatteryAddress(bq20z9xx*);
void display_sbscommands(bq20z9xx*);
void display_bq20z9xx(bq20z9xx*);
void displaySealstatus(bq20z9xx*);
void displayBatteryStatus(bq20z9xx*);
void printBits(uint8_t);
void printBits(uint16_t);

void displaymanufacturerAccess(bq20z9xx*);
void displayremainingCapacityAlarm(bq20z9xx*);
void displayremainingTimeAlarm(bq20z9xx*);
void displaybatteryMode(bq20z9xx*);
void displayatRate(bq20z9xx*);
void displayatRateTimeToFull(bq20z9xx*);
void displayatRateTimeToEmpty(bq20z9xx*);
void displayatRateOK(bq20z9xx*);
void displaytemperature(bq20z9xx*);
void displayvoltage(bq20z9xx*);
void displaycurrent(bq20z9xx*);
void displayaverageCurrent(bq20z9xx*);
void displaymaxError(bq20z9xx*);
void displayrelativeStateOfCharge(bq20z9xx*);
void displayabsoluteStateOfCharge(bq20z9xx*);
void displayremainingCapacity(bq20z9xx*);
void displayfullCapacity(bq20z9xx*);
void displayrunTimeToEmpty(bq20z9xx*);
void displayavgTimeToEmpty(bq20z9xx*);
void displayavgTimeToFull(bq20z9xx*);
void displaychargingCurrent(bq20z9xx*);
void displaychargingVoltage(bq20z9xx*);
void displaybatteryStatus(bq20z9xx*);
void displaydesignCapacity(bq20z9xx*);
void displaycycleCount(bq20z9xx*);
void displaydesignVoltage(bq20z9xx*);
void displayspecificationInfo(bq20z9xx*);
void displaymanufactureDate(bq20z9xx*);
void displayserialNumber(bq20z9xx*);
void displaymanufacturerName(bq20z9xx*);
void displaydeviceName(bq20z9xx*);
void displaydeviceChemistry(bq20z9xx*);
void displayoptionalMFGfunctions(bq20z9xx*);

