#pragma once

#include <Arduino.h>
#include <Wire.h>

#ifdef ESP8266
#define SDA D2
#define SCL D1
#endif

#ifdef STM32
#define SDA PA1
#define SCL PA2
#endif

#ifdef ESP32
#define SDA 21
#define SCL 22
#endif

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
