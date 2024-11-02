#pragma once

#define TAB1 3
#define TAB2 40
#define TAB3 65

#include <Arduino.h>
#include "../ansi/ansi.h"
#include "../SBS/ArduinoSMBus.h"

static ANSI ansi(&Serial);

void displayMainmenu();
void displaySmallmenu();
void displayBatteryNr(uint8_t address);

void Display_standard(ArduinoSMBus& battery);
void Display_bq2020z9xx(ArduinoSMBus& battery);
