#pragma once

#define TAB1 5
#define TAB2 40
#define TAB3 70

#include <Arduino.h>
#include "../ansi/ansi.h"
#include "../BQ/BQ20Z9xx.h"
#include "../i2cscanner/i2cscanner.h"

static ANSI ansi(&Serial);

void displayMainmenu();
void displaySmallmenu();
void displayBatteryAddress(bq20z9xxcommands*);
void display_sbscommands(bq20z9xxcommands*);
void display_bq20z9xx(bq20z9xxcommands*);
void displaySealstatus(bq20z9xxcommands*);
void displayBatteryStatus(bq20z9xxcommands*);
void printBits(uint8_t);
void printBits(uint16_t);

