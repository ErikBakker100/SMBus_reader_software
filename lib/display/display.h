#pragma once

#define TAB1 5
#define TAB2 40
#define TAB3 70

#include <Arduino.h>
#include "../ansi/ansi.h"
#include "../i2cscanner/i2cscanner.h"
#include "CommandClassifiers.h"
#include "../SMB/SMBCommands.h"
#include <variant>
#include <vector>

extern ANSI ansi;

template <typename T>
using pdc = std::variant<             // pdc = pointer to display command
    void (T::*)(),                    // Member function with signature `void()`
    void (T::*)(uint16_t, uint16_t)  // Member function with signature `bool(uint16_t, uint16_t)
>;

template <typename T>
struct Info {
  pdc<T> dc;                          // dc = display command
  uint8_t monitor_group;
  String name;                        // name of the battery function the display function calls 
  // Constructor to initialize the struct
  Info(pdc<T> f, uint8_t g, String n) : dc(f), monitor_group(g), name(n) {};
};

class Display : protected smbuscommands{

public:
    Display(uint8_t);
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
    virtual void displayoptionalMFGfunctions(); // not within the SMBUS standard v1.1, so may be overridden

    void displayBatteryAddress();

protected:
    void printBits(uint8_t);
    void printBits(uint16_t);
    void printBits(uint32_t);
};







