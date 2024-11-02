/**
 * @file main.cpp
 * @author Christopher Lee (clee@unitedconsulting.com)
 * @brief Example arduino code to read battery data from an SMBus battery and print to serial output.
 * @version 1.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Arduino.h>
#include "../lib/SBS/ArduinoSMBus.h"
#include "../lib/i2cscanner/i2cscanner.h"
#include "../lib/ansi/ansi.h"
#include "../lib/FiniteStateMachine/fsm.h"
#include "../lib/CmdParser/CmdBuffer.hpp"
#include "../lib/CmdParser/CmdParser.hpp"

#define TAB1 3
#define TAB2 40
#define TAB3 65

static ArduinoSMBus battery;
static ANSI ansi(&Serial);
static uint8_t batteryaddress {0};
Command command;
CmdBuffer<64> cmdBuffer;
CmdParser cmdParser;

uint8_t findbatteryaddress();
void displaymenu();

void setup() {
  Serial.begin(115200);
  command.handleInput(1); // display menu
  displaymenu();
  cmdBuffer.getStringFromBuffer()
}

void loop() {
  if(cmdBuffer.readFromSerial(&ansi)) {
    ansi.print("received: ");
    ansi.println(cmdBuffer.getStringFromBuffer());
    cmdBuffer.clear();
  }
  (batteryaddress == 0) batteryaddress = findbatteryaddress();
  if (ansi.available()) {
    uint8_t toets = ansi.read();
    switch (toets) {
      case 0x31: // 1 is entered
        displaymenu();  
        Display_standard(battery);
        break;
    }
  }
}

uint8_t findbatteryaddress() {
  uint16_t x, y; // x and y position
  uint8_t address = i2cscan();
  if (address > 0) {
    battery.setBatteryAddress(batteryaddress);
    ansi.print("Battery find at: ");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print("0x");
    ansi.print(address < 0x10 ? "0": "" );
    ansi.println(address, HEX);
  } else ansi.print("No Battery found !");
  return address;
}

void displaymenu() {
  ansi.clearScreen();
  ansi.print("1 = display standard, ");
  ansi.println();
}