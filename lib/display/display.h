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
using pdc = std::variant<            // pdc = pointer to display command
    void (T::*)(),                   // Member function with signature `void()`
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

class Display : public smbuscommands{

public:
    Display(uint8_t);
    static void displaymanufacturerAccess();
    static void displayremainingCapacityAlarm();
    static void displayremainingTimeAlarm();
    static void displaybatteryMode();

    static void displayatRate();
    static void displayatRateTimeToFull();
    static void displayatRateTimeToEmpty();
    static void displayatRateOK();
    static void displaytemperature();
    static void displayvoltage();
    static void displaycurrent();
    static void displayaverageCurrent();
    static void displaymaxError();
    static void displayrelativeStateOfCharge();
    static void displayabsoluteStateOfCharge();
    static void displayremainingCapacity();
    static void displayfullCapacity();
    static void displayrunTimeToEmpty();
    static void displayavgTimeToEmpty();
    static void displayavgTimeToFull();
    static void displaychargingCurrent();
    static void displaychargingVoltage();
    static void displaybatteryStatus();
    static void displaydesignCapacity();
    static void displaycycleCount();
    static void displaydesignVoltage();
    static void displayspecificationInfo();
    static void displaymanufactureDate();
    static void displayserialNumber();
    static void displaymanufacturerName();
    static void displaydeviceName();
    static void displaydeviceChemistry();
    static void displayoptionalMFGfunctions(); // not within the SMBUS standard v1.1, so may be overridden

    void displayBatteryAddress();

protected:
    void printBits(uint8_t);
    void printBits(uint16_t);
    void printBits(uint32_t);
};
