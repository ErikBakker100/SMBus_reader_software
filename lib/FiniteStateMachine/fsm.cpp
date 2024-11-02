#include "fsm.h"

static uint8_t batteryaddress {0};

// class Command

Command::Command() {
    state_ = new menuState;
}

void Command::handleInput (uint8_t input) {
    Serial.print(input, DEC);
    CommandState* state = state_->handleInput(*this, input);
    if (state != nullptr) {
        delete state_;
        state_ = state;
        state_->enter(*this);
    }
}

void Command::update () {
    if(state_) state_->update();
    else {
        state_ = new menuState;
        state_->enter(*this);
    }
}

void CommandState::enter(Command& command) {
}

CommandState* CommandState::handleInput (Command& command, uint8_t input) {
    if (input == 1) return new menuState;
    else if (input == 2) return new scanState;
    else if (input == 3) return new standardState;
    else if (input == 4) return new extendedState;
    return nullptr;
}

void CommandState::update () {
}

//class menu = 1
/*CommandState* menuState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void menuState::enter(Command& command) {
    displayMainmenu();
}

//class I2c Scan = 2
/*CommandState* scanState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void scanState::enter(Command& command) {
    displaySmallmenu();
    batteryaddress = i2cscan();
    displayBatteryNr(batteryaddress);
    battery.setBatteryAddress(batteryaddress);
}

//class standard = 3
/*CommandState* standardState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void standardState::enter(Command& command) {
    displaySmallmenu();
    Display_standard(battery);
}

// class extended = 4
/*CommandState* extendedState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void extendedState::enter(Command& command) {
    displaySmallmenu();
    Display_bq2020z9xx(battery);
}
