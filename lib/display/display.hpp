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
void displayBatteryAddress(bq20z9xxcommands*);
void display_sbscommands(bq20z9xxcommands*);
void display_bq20z9xx(bq20z9xxcommands*);
void displaySealstatus(bq20z9xxcommands*);
void displayBatteryStatus(bq20z9xxcommands*);
void printBits(uint8_t);
void printBits(uint16_t);

void displaymanufacturerAccess(bq20z9xxcommands*);
void displayremainingCapacityAlarm(bq20z9xxcommands*);
void displayremainingTimeAlarm(bq20z9xxcommands*);
void displaybatteryMode(bq20z9xxcommands*);
void displayatRate(bq20z9xxcommands*);
void displayatRateTimeToFull(bq20z9xxcommands*);
void displayatRateTimeToEmpty(bq20z9xxcommands*);
void displayatRateOK(bq20z9xxcommands*);
void displaytemperature(bq20z9xxcommands*);
void displayvoltage(bq20z9xxcommands*);
void displaycurrent(bq20z9xxcommands*);
void displayaverageCurrent(bq20z9xxcommands*);
void displaymaxError(bq20z9xxcommands*);
void displayrelativeStateOfCharge(bq20z9xxcommands*);
void displayabsoluteStateOfCharge(bq20z9xxcommands*);
void displayremainingCapacity(bq20z9xxcommands*);
void displayfullCapacity(bq20z9xxcommands*);
void displayrunTimeToEmpty(bq20z9xxcommands*);
void displayavgTimeToEmpty(bq20z9xxcommands*);
void displayavgTimeToFull(bq20z9xxcommands*);
void displaychargingCurrent(bq20z9xxcommands*);
void displaychargingVoltage(bq20z9xxcommands*);
void displaybatteryStatus(bq20z9xxcommands*);
void displaydesignCapacity(bq20z9xxcommands*);
void displaycycleCount(bq20z9xxcommands*);
void displaydesignVoltage(bq20z9xxcommands*);
void displayspecificationInfo(bq20z9xxcommands*);
void displaymanufactureDate(bq20z9xxcommands*);
void displayserialNumber(bq20z9xxcommands*);
void displaymanufacturerName(bq20z9xxcommands*);
void displaydeviceName(bq20z9xxcommands*);
void displaydeviceChemistry(bq20z9xxcommands*);
void displayoptionalMFGfunctions(bq20z9xxcommands*);

