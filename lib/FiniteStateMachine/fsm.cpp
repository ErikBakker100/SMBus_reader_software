#include "fsm.h"

static uint8_t batteryaddress {0};
CmdParser cmd;

// class Command

Command::Command() {
    state_ = new menuState;
}

void Command::handleInput (CmdBuffer<64> buffer) {
    if (cmd.parseCmd(&buffer) == CMDPARSER_ERROR) return;
    String com = cmd.getCommand();
    uint8_t i = com.toInt();
    CommandState* state = state_->handleInput(*this, i);
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
    else if (input == 5) return new unsealState;
    else if (input == 6) return new sealState;
    else if (input == 7) return new clearpfState;

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
    if(cmd.getParamCount() == 3) {
        uint8_t first, second;
        String param = cmd.getCmdParam(1);
        first = param.toInt();
        param = cmd.getCmdParam(2);
        second = param.toInt();
        if (first <= second) batteryaddress = i2cscan(first, second);
    } else batteryaddress = i2cscan();
    battery.setBatteryAddress(batteryaddress);
    displayBatteryNr(batteryaddress);
}

//class standard = 3
/*CommandState* standardState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void standardState::enter(Command& command) {
    displaySmallmenu();
    if (batteryaddress > 0) display_standard(battery);
    else Serial.println("\n\tPlease Search for address of the Battery first.");
}

// class extended = 4
/*CommandState* extendedState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void extendedState::enter(Command& command) {
    displaySmallmenu();
    if (batteryaddress > 0) display_bq2020z9xx(battery);
    else Serial.println("\n\tPlease Search for address of the Battery first.");
}

// class unseal = 5
/*CommandState* unsealState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void unsealState::enter(Command& command) {
    displaySmallmenu();
    if (batteryaddress > 0) {
        battery.manufacturerAccessUnseal(UNSEALA, UNSEALB);
        displaySealstatus(battery);
    }
    else Serial.println("\n\tPlease Search for address of the Battery first.");

}

// class seal = 6
/*CommandState* sealState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void sealState::enter(Command& command) {
    displaySmallmenu();
    if (batteryaddress > 0) {
        battery.manufacturerAccessSeal();
        displaySealstatus(battery);
    }
    else Serial.println("\n\tPlease Search for address of the Battery first.");
}

// class clear pf = 7
/*CommandState* clearpfState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void clearpfState::enter(Command& command) {
    displaySmallmenu();
    if (batteryaddress > 0) battery.ClearPermanentFailure(PFCLEARA, PFCLEARB);
    else Serial.println("\n\tPlease Search for address of the Battery first.");
    Serial.println("done!");
}
