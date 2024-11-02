#include "fsm.h"

static ANSI ansi(&Serial);

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
    ansi.println("Menu");
}

//class I2c Scan = 2
/*CommandState* scanState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void scanState::enter(Command& command) {
    ansi.println("Scan");
}

//class standard = 3
/*CommandState* standardState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void standardState::enter(Command& command) {
    ansi.println("Standard");
}

// class extended = 4
/*CommandState* extendedState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void extendedState::enter(Command& command) {
    ansi.println("Extended");
}
