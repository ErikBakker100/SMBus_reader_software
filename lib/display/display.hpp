#pragma once

#define TAB1 5
#define TAB2 40
#define TAB3 70

#include <Arduino.h>
#include "../ansi/ansi.h"
#include "../SBS/ArduinoSMBus.h"
#include "../i2cscanner/i2cscanner.h"

static ANSI ansi(&Serial);

void displayMainmenu();
void displaySmallmenu();
void displayBatteryNr(uint8_t address);
void display_sbscommands(ArduinoSMBus& battery);
void display_bq20z9xx(ArduinoSMBus& battery);
void displaySealstatus(ArduinoSMBus& battery);
void displayBatteryStatus(ArduinoSMBus& battery);
void printBits(uint8_t);
void printBits(uint16_t);


