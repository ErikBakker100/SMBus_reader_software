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
    
    std::vector<Info<DisplayBQ20Z9xx>> info; // Store structs

    // Call a specific function by name
    void displayCommandNames();
};







