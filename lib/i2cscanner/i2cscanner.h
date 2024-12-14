#pragma once

#include <Arduino.h>
#include <Wire.h>

uint8_t i2cscan();
uint8_t i2cscan(uint8_t, uint8_t);

static String I2Ccode[6] {
    "ok",
    "data too long",
    "NACK on tx address",
    "NACK on tx data",
    "other",
    "timeout"
};

