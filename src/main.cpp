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
#include "../lib/FiniteStateMachine/fsm.h"
#include "../lib/CmdParser/CmdBuffer.hpp"
#include "../lib/CmdParser/CmdParser.hpp"

#define TAB1 3
#define TAB2 40
#define TAB3 65

//static ArduinoSMBus battery;
static uint8_t batteryaddress {0};
Command command;
CmdBuffer<64> cmdBuffer;
CmdParser cmdParser;

//uint8_t findbatteryaddress();
// void displaymenu();

void setup() {
  Serial.begin(115200);
  cmdBuffer.setEcho(true);
  command.update();
}

void loop() {
//  command.update();
  if (cmdBuffer.readFromSerial(&Serial)) {
    if (cmdParser.parseCmd(&cmdBuffer) != CMDPARSER_ERROR) {
      String com = cmdParser.getCommand();
      uint8_t i = com.toInt();
      command.handleInput(i);
      cmdBuffer.clear();
    } else Serial.println("Parser error!");
  } else Serial.println("timeout");
//  (batteryaddress == 0) batteryaddress = findbatteryaddress();
}

/*
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

*/