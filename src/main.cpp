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


class Config {
public:
    virtual void setup() = 0;   // Pure virtuele functie voor setup
    virtual void loop() = 0;    // Pure virtuele functie voor loop
    virtual ~Config() {}        // Virtuele destructor
};

// Configuratie A
#if defined(BQ20Z9XX)
class BQ20Z9xx : public Config {
public:
    void setup() override {
        Serial.println("Setup voor Configuratie A");
    }
    void loop() override {
        Serial.println("Loop van Configuratie A actief");
        delay(1000);
    }
};
#endif

// Configuratie B
#if defined(BQ40Z6XX)
class BQ40Z6xx : public Config {
public:
    void setup() override {
        Serial.println("Setup voor Configuratie B");
    }
    void loop() override {
        Serial.println("Loop van Configuratie B actief");
        delay(1000);
    }
};
#endif

// Selecteer de actieve configuratie
Config* activeConfig = nullptr;

void setup() {
  Serial.begin(115200);
  cmdBuffer.setEcho(true);
  command.update();

#if defined(BQ20Z9XX)
    activeConfig = new BQ20Z9xx();
#elif defined(BQ40Z6XX)
    activeConfig = new BQ40Z6xx();
#else
    #error "Geen configuratie geselecteerd. Definieer CONFIG_A, CONFIG_B of CONFIG_C."
#endif

    if (activeConfig) {
        activeConfig->setup();
    }
}

void loop() {
  if (cmdBuffer.readFromSerial(&Serial, 1)) {
      command.handleInput(cmdBuffer);
      cmdBuffer.clear();
  }
  command.update();
  if (activeConfig) {
    activeConfig->loop();
  }
}
