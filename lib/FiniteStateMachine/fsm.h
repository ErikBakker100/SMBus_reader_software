#pragma once
#include <Arduino.h>
#include "../display/display.h"
#include "../CmdParser/CmdBuffer.hpp"
#include "../CmdParser/CmdParser.hpp"

class CommandState;

class Command{
public:
    Command();
    virtual void handleInput(CmdBuffer<64>);
    virtual void update();
    Display* display = {nullptr};
private:
    CommandState* state_ {nullptr};
protected:
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

class categoryState : public CommandState {
public:
    virtual void enter(Command &);
//    virtual CommandState* handleInput(Command&, uint8_t);
};

class commandnameState : public CommandState {
public:
    virtual void enter(Command&);
//    virtual CommandState* handleInput(Command&, uint8_t);
};

class unsealState : public CommandState {
public:
    virtual void enter(Command&);
    virtual CommandState* handleInput(Command&, uint8_t);
    virtual void update();
private:
    bool scanning {false};
    uint32_t key {0x1000};
    Command* com;
};

class sealState : public CommandState {
public:
    virtual void enter(Command&);
//    virtual CommandState* handleInput(Command&, uint8_t);
};
class clearpfState : public CommandState {
public:
    virtual void enter(Command&);
    virtual CommandState* handleInput(Command&, uint8_t);
    virtual void update();
};

class fullaccessState : public CommandState {
public:
    virtual void enter(Command&);
    virtual CommandState* handleInput(Command&, uint8_t);
    virtual void update();
};

