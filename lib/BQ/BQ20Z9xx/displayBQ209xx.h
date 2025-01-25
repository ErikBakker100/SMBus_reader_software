#pragma once

#include "../../display/display.h"
#include "BQ20Z9xx.h"

class DisplayBQ20Z9xx : public bq20z9xx, public Display {

public:
    DisplayBQ20Z9xx() {};
    void setup(uint8_t);
    void loop();

    //static void displayoptionalMFGfunctions();

    static void displaymanufacturerAccessType();       // command 0x00 0x0001
    static void displaymanufacturerAccessFirmware();   // command 0x00
    static void displaymanufacturerAccessHardware();   // command 0x00
    static void displaymanufacturerAccessStatus();     // command 0x00
    static void displaymanufacturerAccessChemistryID(); // command 0x00 0x0008
    static void displaymanufacturerAccessShutdown();   // command 0x0010
    static void displaymanufacturerAccessSleep();      // command 0x0011
    static void displaymanufacturerAccessSeal();       // command 0x00 0x0020
    static void displaymanufacturerAccessPermanentFailClear(uint16_t key_a = PFCLEARA, uint16_t key_b = PFCLEARB); 
    static void displaymanufacturerAccessUnseal(uint16_t key_a = UNSEALA, uint16_t key_b = UNSEALB);
    static void displaymanufacturerAccessFullAccess(uint16_t key_a = FULLACCESSA, uint16_t key_b = FULLACCESSB);
    static void displaymanufacturerData();             // command 0x23
    static void displayfetControl();
    static void displaystateOfHealth();                // command 0x4f
    static void displaysafetyAlert();                  // command 0x50
    static void displaysafetyStatus();                 // command 0x51
    static void displaypfAlert();
    static void displaypfStatus();
    static void displayoperationStatus();              // command 0x54
    static void displayunsealKey();                    // command 0x60

    bool displaySealstatus();                   // true if sealed, otherwise false.
    bool testkey(uint16_t);  // Tests a 16 bit key part. Returns true if I2C code is ok.
    
    std::vector<Info<DisplayBQ20Z9xx>> info; // Store structs

    // Call a specific function by name
    void displayByName(const String&);
    // Call functions dynamically
    void displayByClassifier(uint8_t);
    void displayCommandNames();
};







