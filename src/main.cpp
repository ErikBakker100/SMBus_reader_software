/**
 * @file main.cpp
 * @author Christopher Lee (clee@unitedconsulting.com)
 * @brief Arduino code to read battery data from an SMBus battery and print to serial output.
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

ANSI ansi(&Serial);
Command command;
CmdBuffer<64> cmdBuffer;

void setup() {
  Serial.begin(115200);
  cmdBuffer.setEcho(true);
  command.update();
}

void loop() {
  if (cmdBuffer.readFromSerial(&Serial)) {
      command.handleInput(cmdBuffer);
      cmdBuffer.clear();
  } else Serial.println("Parser error!");
}
