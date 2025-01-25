#pragma once
#include <Arduino.h>
#include "../display/display.h"
#include "../CmdParser/CmdBuffer.hpp"
#include "../CmdParser/CmdParser.hpp"

// Abstracte basisclass, do not modify
class Config {
public:
    virtual void setup(uint8_t) = 0;
    virtual void loop() = 0;
    virtual ~Config() {};
private:

};

// create a class for each configuration, based on the platformio.ini [bqchip] build_flags = setting
#if defined (BQ20Z9XX)
#include "../BQ/BQ20Z9xx/displayBQ209xx.h"
class DisplayBQ20Z9xx : public Config, bq20z9xx {
public:
    void setup(uint8_t) override;
    void loop() override;
};
#endif

#if defined (BQ40Z6XX)
class BQ40Z9xx : public Config {
public:
    void setup() override;
    void loop() override;
};
#endif
// end of configuration part, do not modify below
class CommandState;

class Command{
public:
    Command();
    virtual void handleInput(CmdBuffer<64>);
    virtual void update();
    Config* display = nullptr;
private:
    CommandState* state_ = nullptr;
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
    void enter(Command&) override;
//    CommandState* handleInput(Command&, uint8_t);
};

class scanState : public CommandState {
public:
    void enter(Command &)  override;
//    CommandState* handleInput(Command&, uint8_t);
};

class categoryState : public CommandState {
public:
    void enter(Command &) override;
//    CommandState* handleInput(Command&, uint8_t);
};

class commandnameState : public CommandState {
public:
    void enter(Command&) override;
//    CommandState* handleInput(Command&, uint8_t);
};

class unsealState : public CommandState {
public:
    void enter(Command&) override;
    CommandState* handleInput(Command&, uint8_t) override;
    void update() override;
private:
    bool scanning {false};
    uint32_t key {0x1000};
    Command* com;
};

class sealState : public CommandState {
public:
    void enter(Command&) override;
//    virtual CommandState* handleInput(Command&, uint8_t);
};
class clearpfState : public CommandState {
public:
    void enter(Command&) override;
    CommandState* handleInput(Command&, uint8_t) override;
    void update() override;
};

class fullaccessState : public CommandState {
public:
    void enter(Command&) override;
    CommandState* handleInput(Command&, uint8_t) override;
    void update() override;
};

