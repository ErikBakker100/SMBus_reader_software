#pragma once

#include "../../display/display.h"

class DisplayBQ20Z9xx : public Display {

public:
    DisplayBQ20Z9xx(uint8_t);
    void displayoptionalMFGfunctions();

    void displaymanufacturerAccessType();       // command 0x00 0x0001
    void displaymanufacturerAccessFirmware();   // command 0x00
    void displaymanufacturerAccessHardware();   // command 0x00
    void displaymanufacturerAccessStatus();     // command 0x00
    void displaymanufacturerAccessChemistryID(); // command 0x00 0x0008
    void displaymanufacturerAccessShutdown();   // command 0x0010
    void displaymanufacturerAccessSleep();      // command 0x0011
    void displaymanufacturerAccessSeal();       // command 0x00 0x0020
    void displaymanufacturerAccessPermanentFailClear(uint16_t key_a = PFCLEARA, uint16_t key_b = PFCLEARB); 
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
    void displayunsealKey();                    // command 0x60

    bool displaySealstatus();                   // true if sealed, otherwise false.
    bool testkey(uint16_t);  // Tests a 16 bit key part. Returns true if I2C code is ok.
    
    std::vector<Info<DisplayBQ20Z9xx>> info; // Store structs

    // Call a specific function by name
    void displayByName(const String&);
    // Call functions dynamically
    void displayByClassifier(uint8_t);
    void displayCommandNames();
};







