#pragma once
#include <Arduino.h>
#include "../ansi/ansi.h"
#include "../SBS/ArduinoSMBus.h"
#include "../i2cscanner/i2cscanner.h"

class CommandState;

class Command{
public:
    Command();
    virtual void handleInput(uint8_t);
    virtual void update();
private:
    CommandState* state_;
};

class CommandState {
public:
    virtual ~CommandState() {};
    virtual void enter(Command&);
    virtual CommandState* handleInput(Command&, uint8_t);
    virtual void update();
protected:
};

class menuState : public CommandState {
public:
    virtual void enter(Command&);
//    virtual CommandState* handleInput(Command&, uint8_t);
};

class scanState : public CommandState {
public:
    virtual void enter(Command &);
//    virtual CommandState* handleInput(Command&, uint8_t);
};

class standardState : public CommandState {
public:
    virtual void enter(Command &);
//    virtual CommandState* handleInput(Command&, uint8_t);
};

class extendedState : public CommandState {
public:
    virtual void enter(Command&);
//    virtual CommandState* handleInput(Command&, uint8_t);
};
