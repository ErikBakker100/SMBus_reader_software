#pragma once

#define TAB1 5
#define TAB2 40
#define TAB3 65

#include <Arduino.h>
#include "../ansi/ansi.h"
#include "../SBS/ArduinoSMBus.h"

static ANSI ansi(&Serial);

void displayMainmenu();
void displaySmallmenu();
void displayBatteryNr(uint8_t address);
void display_standard(ArduinoSMBus& battery);
void display_bq2020z9xx(ArduinoSMBus& battery);
void displaySealstatus(ArduinoSMBus& battery);
