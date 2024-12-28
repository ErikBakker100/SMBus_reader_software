#include "fsm.h"
#include "../i2cscanner/i2cscanner.h"
#include "../display/display.h"
#include "../display/menus.h"

CmdParser cmd;
ANSI ansi(&Serial);

// class Command
Command::Command() {
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
    if (input == 2) return new scanState;
    if (command.display == nullptr) {
        Serial.println("please select '2' (Search address) first");
    } else {
        if (input == 3) return new categoryState;
        else if (input == 4) return new commandnameState;
        else if (input == 5) return new unsealState;
        else if (input == 6) return new sealState;
        else if (input == 7) return new clearpfState;
        else if (input == 8) return new fullaccessState;
    }
    return new menuState;
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
        first = (uint8_t)cmd.toLong(1);
        second = (uint8_t)cmd.toLong(2);
        if (first <= second) address =i2cscan(first, second);
    } else address = i2cscan();
    if (address > 0) {
        command.display = new Display(address);
        command.display->displayBatteryAddress();
    }
}

//class standard = 3
/*CommandState* standardState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void categoryState::enter(Command& command) {
    displaySmallmenu();
    if(cmd.getParamCount() == 2) {
        String param = cmd.getCmdParam(1);
        command.display->displayByClassifier(param.toInt());
    } else Serial.println("please specify category. Select '3 x' (x is category number)");
}

// class extended = 4
/*CommandState* extendedState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void commandnameState::enter(Command& command) {
    displaySmallmenu();
    if(cmd.getParamCount() == 2) {
        String param = cmd.getCmdParam(1);
        (param == "?")? command.display->displayCommandNames():command.display->displayByName(param);
        
    } else Serial.println("Please specify command name. Select '3 x' (x is name, or use ?)");
}

// class unseal = 5
CommandState* unsealState::handleInput (Command& command, uint8_t input) {
    return new menuState;
}

void unsealState::update () {
    com->display->testkey(key);
    key++;
    if (key == 0xffff) scanning = false;
}

void unsealState::enter(Command& command) {
    displaySmallmenu();
    com = &command;
    if(cmd.getParamCount() == 2) {
        String param = cmd.getCmdParam(1);
        if (param == "?") scanning = true;
    }
    else if(cmd.getParamCount() == 3) {
        uint32_t first, second;
        first = cmd.toLong(1);
        second = cmd.toLong(2);
        command.display->displaymanufacturerAccessUnseal(first, second);
    } else command.display->displaymanufacturerAccessUnseal(); // using default values
}

// class seal = 6
/*CommandState* sealState::handleInput (Command& command, uint8_t input) {
    return nullptr;
}*/

void sealState::enter(Command& command) {
    displaySmallmenu();
    command.display->displaymanufacturerAccessSeal();
//        displaySealstatus(battery);
}

// class clear pf = 7
CommandState* clearpfState::handleInput (Command& command, uint8_t input) {
    return new menuState;
}

void clearpfState::enter(Command& command) {
    displaySmallmenu();
    if(cmd.getParamCount() == 3) {
        uint32_t first, second;
        first = cmd.toLong(1);
        second = cmd.toLong(2);
        command.display->displaymanufacturerAccessPermanentFailClear(first, second);
    } else command.display->displaymanufacturerAccessPermanentFailClear(); // using default values
}

void clearpfState::update () {
//    command.display->findkey();
}


// class full access = 8
CommandState* fullaccessState::handleInput (Command& command, uint8_t input) {
    return new menuState;
}

void fullaccessState::enter(Command& command) {
    displaySmallmenu();
    if(cmd.getParamCount() == 3) {
        uint32_t first, second;
        first = cmd.toLong(1);
        second = cmd.toLong(2);
        command.display->displaymanufacturerAccessFullAccess(first, second);
    } else command.display->displaymanufacturerAccessFullAccess(); // using default values
}

void fullaccessState::update () {
//    command.display->findkey();
}