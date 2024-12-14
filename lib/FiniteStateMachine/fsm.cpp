#include "fsm.h"
#include "../i2cscanner/i2cscanner.h"
#include "../display/display.hpp"

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
    if (command.battery == nullptr) {
        Serial.println("please select '2' (Search address) first");
    } else {
        if (input == 3) return new standardState;
        else if (input == 4) return new extendedState;
        else if (input == 5) return new unsealState;
        else if (input == 6) return new sealState;
        else if (input == 7) return new clearpfState;
        else if (input == 8) return new specifycommandState;
    }
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
    uint8_t address = 0;
    if(cmd.getParamCount() == 3) {
        uint8_t first, second;
        String param = cmd.getCmdParam(1);
        first = param.toInt();
        param = cmd.getCmdParam(2);
        second = param.toInt();
        if (first <= second) address =i2cscan(first, second);
    } else address = i2cscan();
    if (address > 0) command.battery = new bq20z9xxcommands(address);
    displayBatteryAddress(command.battery);
}

//class standard = 3
/*CommandState* standardState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void standardState::enter(Command& command) {
    displaySmallmenu();
    display_sbscommands(command.battery);
}

// class extended = 4
/*CommandState* extendedState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void extendedState::enter(Command& command) {
    displaySmallmenu();
//    display_bq20z9xx(battery);
}

// class unseal = 5
/*CommandState* unsealState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void unsealState::enter(Command& command) {
    displaySmallmenu();
        command.battery->manufacturerAccessUnseal(UNSEALA, UNSEALB);
//        displaySealstatus(battery);

}

// class seal = 6
/*CommandState* sealState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void sealState::enter(Command& command) {
    displaySmallmenu();
    command.battery->manufacturerAccessSeal();
//        displaySealstatus(battery);
}

// class clear pf = 7
/*CommandState* clearpfState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void clearpfState::enter(Command& command) {
    displaySmallmenu();
    command.battery->manufacturerAccessPermanentFailClear(PFCLEARA, PFCLEARB);
    Serial.println("done!");
}

// class specify command = 8
/*CommandState* specifycommand::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void specifycommandState::enter(Command& command) {
    displaySmallmenu();
    if(cmd.getParamCount() == 2) {
        String reg = cmd.getCmdParam(1);
        if (reg = "BatteryStatus") {
            displayBatteryStatus(command.battery);
        }
    } else Serial.println("Command not found");
    Serial.println("done!");
}


