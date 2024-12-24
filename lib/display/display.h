#pragma once

#define TAB1 5
#define TAB2 40
#define TAB3 70

#include <Arduino.h>
#include "../ansi/ansi.h"
#include "../BQ/BQ20Z9xx.h"
#include "../i2cscanner/i2cscanner.h"
#include "CommandClassifiers.h"
#include <variant>
#include <vector>

extern ANSI ansi;

template <typename T>
using pdc = std::variant<             // pdc = pointer to display command
    void (T::*)(),                    // Member function with signature `void()`
    void (T::*)(uint16_t, uint16_t)  // Member function with signature `void(uint16_t, uint16_t)
>;

template <typename T>
struct Info {
  pdc<T> dc;                          // dc = display command
  uint8_t monitor_group;
  String name;                        // name of the battery function the display function calls 
  // Constructor to initialize the struct
  Info(pdc<T> f, uint8_t g, String n) : dc(f), monitor_group(g), name(n) {};
};

class Display : private bq20z9xx {

public:
    Display(uint8_t);
    void displaymanufacturerAccess();
    void displayremainingCapacityAlarm();
    void displayremainingTimeAlarm();
    void displaybatteryMode();
    void displayatRate();
    void displayatRateTimeToFull();
    void displayatRateTimeToEmpty();
    void displayatRateOK();
    void displaytemperature();
    void displayvoltage();
    void displaycurrent();
    void displayaverageCurrent();
    void displaymaxError();
    void displayrelativeStateOfCharge();
    void displayabsoluteStateOfCharge();
    void displayremainingCapacity();
    void displayfullCapacity();
    void displayrunTimeToEmpty();
    void displayavgTimeToEmpty();
    void displayavgTimeToFull();
    void displaychargingCurrent();
    void displaychargingVoltage();
    void displaybatteryStatus();
    void displaydesignCapacity();
    void displaycycleCount();
    void displaydesignVoltage();
    void displayspecificationInfo();
    void displaymanufactureDate();
    void displayserialNumber();
    void displaymanufacturerName();
    void displaydeviceName();
    void displaydeviceChemistry();
    void displayoptionalMFGfunctions();

    void displaymanufacturerAccessType();       // command 0x00 0x0001
    void displaymanufacturerAccessFirmware();   // command 0x00
    void displaymanufacturerAccessHardware();   // command 0x00
    void displaymanufacturerAccessStatus();     // command 0x00
    void displaymanufacturerAccessChemistryID(); // command 0x00 0x0008
    void displaymanufacturerAccessShutdown();   // command 0x0010
    void displaymanufacturerAccessSleep();      // command 0x0011
    void displaymanufacturerAccessSeal();       // command 0x00 0x0020
    void displaymanufacturerAccessPermanentFailClear(uint16_t key_a = PFCLEARA, uint16_t key_b = PFCLEARA); 
    void displaymanufacturerAccessUnseal(uint16_t key_a = UNSEALA, uint16_t key_b = UNSEALB);
    void displaymanufacturerAccessFullAccess(uint16_t key_a = FULLACCESSA, uint16_t key_b = FULLACCESSB);
    void displaymanufacturerData();             // command 0x23
    void displayfetControl();
    void displaystateOfHealth();                // command 0x4f
    void displaysafetyAlert();                  // command 0x50
    void displaysafetyStatus();                 // command 0x51
    void displaypfAlert();
    void displaypfStatus();
    void displayoperationStatus();              // command 0x54
    void displaySealstatus(); // command 0x54
    void displayunsealKey();                    // command 0x60
    void displayBatteryAddress();

    std::vector<Info<Display>> info; // Store structs

    // Call a specific function by name
    void displayByName(const String&);
    // Call functions dynamically
    void displayByClassifier(uint8_t);
    void displayCommandNames();

private:
    void printBits(uint8_t);
    void printBits(uint16_t);
};







