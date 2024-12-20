#pragma once

#define TAB1 5
#define TAB2 40
#define TAB3 70

#include <Arduino.h>
#include "../ansi/ansi.h"
#include "../BQ/BQ20Z9xx.h"
#include "../i2cscanner/i2cscanner.h"

static ANSI ansi(&Serial);

class Display {

    void displaymanufacturerAccessType();       // command 0x00 0x0001
    void displaymanufacturerAccessFirmware();   // command 0x00
    void displaymanufacturerAccessHardware();   // command 0x00
    void displaymanufacturerAccessStatus();     // command 0x00
    void displaymanufacturerAccessChemistryID(); // command 0x00 0x0008
    void displaymanufacturerAccessShutdown();   // command 0x0010
    void displaymanufacturerAccessSleep();      // command 0x0011
    void displaymanufacturerAccessPermanentFailClear(uint16_t key_a, uint16_t key_b); 
    void displaymanufacturerAccessUnseal(uint16_t key_a, uint16_t key_b);
    void displaymanufacturerAccessFullAccess(uint16_t key_a, uint16_t key_b);
    void displaymanufacturerData();             // command 0x23
    void displayfetControl();
    void displaystateOfHealth();                // command 0x4f
    void displaysafetyAlert();                  // command 0x50
    void displaysafetyStatus();                 // command 0x51
    void displaypfAlert();
    void displaypfStatus();
    void displayoperationStatus();              // command 0x54
    void displayunsealKey();                    // command 0x60

    private:
    void printBits(uint8_t);
    void printBits(uint16_t);

};

void displayMainmenu();
void displaySmallmenu();
void displayBatteryAddress(bq20z9xx*);

void display_sbscommands(bq20z9xx*);
void display_bq20z9xx(bq20z9xx*);

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


void displaySealstatus(bq20z9xx*); // command 0x54


