/**
 * @file main.cpp
 * @author 
 * @brief Arduino code to read battery data from an SMBus battery and print to serial output.
 * @version 1.1
 * @date 12-2024
 *
 * @copyright
 *
 */

#include <Arduino.h>
#include "../lib/FiniteStateMachine/fsm.h"
#include "../lib/CmdParser/CmdBuffer.hpp"
#include "../lib/CmdParser/CmdParser.hpp"

Command command;
CmdBuffer<64> cmdBuffer;

void setup() {
  Serial.begin(115200);
  cmdBuffer.setEcho(true);
  cmdBuffer.readFromString("1");
  command.update();
  command.handleInput(cmdBuffer);
  cmdBuffer.clear();
}

void loop() {
  if (cmdBuffer.readFromSerial(&Serial)) {
      command.handleInput(cmdBuffer);
      cmdBuffer.clear();
  } else Serial.println("Parser error!");
}
