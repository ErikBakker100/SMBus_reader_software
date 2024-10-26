/**
 * @file main.cpp
 * @author Christopher Lee (clee@unitedconsulting.com)
 * @brief Example arduino code to read battery data from an SMBus battery and print to serial output.
 * @version 1.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Arduino.h>
#include "../lib/SBS/ArduinoSMBus.h"
#include "../lib/i2cscanner/i2cscanner.h"
#include "../lib/ansi/ansi.h"

#define BACKGROUND ansi.black
#define TAB1 40
#define TAB2 65

ANSI ansi(&Serial);
ArduinoSMBus battery;

void setup() {
  Serial.begin(115200);
}

void loop() {
  delay(1000*5);
  ansi.clearScreen();
  delay(200);
  ansi.background(BACKGROUND);
  uint16_t x, y; // x and y position

  uint8_t batteryaddress = i2cscan();
  if (batteryaddress > 0) {
    battery.setBatteryAddress(batteryaddress);
    ansi.print("Battery address set to:");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print("0x");
    ansi.print(batteryaddress < 0x10 ? "0": "" );
    ansi.println(batteryaddress, HEX);

    ansi.print("Device Type (0x00 -> 0x0001):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print("bq20z");
    ansi.print(battery.manufacturerAccessType(), HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Firmware version (0x00 -> 0x0002):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    uint16_t version = battery.manufacturerAccessFirmware();
    ansi.print((uint8_t)version >> 8 , HEX);
    ansi.print(".");
    ansi.print((uint8_t)version & 0x00ff);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Hardware version (0x00 -> 0x0003):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    version = battery.manufacturerAccessHardware();
    ansi.print((uint8_t)version & 0x00ff, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Man. Battery Status (0x00 -> 0x0006):");
    ansi.readCursorPosition(x, y);
    ManufacturerBatStatus status = battery.manufacturerAccessBatStatus();
    ansi.gotoXY(TAB1, y);
    ansi.print(status.raw, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.I2CError.note);
    ansi.gotoXY(3, y+1);
    ansi.print("Status: ");
    ansi.gotoXY(TAB1, y+1);
    ansi.println(status.failure + " " + status.permfailure);
    ansi.gotoXY(3, y+2);
    ansi.print("Charge FET:");
    ansi.gotoXY(TAB1, y+2);
    ansi.println(status.chg_fet ? "On" : "Off");
    ansi.gotoXY(3, y+3);
    ansi.print("Discharge FET:");
    ansi.gotoXY(TAB1, y+3);
    ansi.println(status.dsg_fet ? "On" : "Off");

    BatteryMode mode = battery.batteryMode(); // We need to read this first to determine output ranges further on

    ansi.print("Remaining Capacity Alarm (0x01):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.remainingCapacityAlarm());
    ansi.print(mode.capacity_mode ? " 10mWh" : " mAh");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Remaining Time Alarm (0x02):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.remainingTimeAlarm());
    ansi.print(" minutes");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Battery Modes (0x03):"); 
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(mode.raw, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    ansi.print("  Internal Charge Controller:");
    ansi.gotoXY(TAB1, y+1);
    ansi.println(mode.internal_charge_controller ? "Supported" : "Not Supported");
    ansi.print("  Primary Battery Support:");
    ansi.gotoXY(TAB1, y+2);
    ansi.println(mode.primary_battery_support ? "Supported" : "Not Supported");
    ansi.print("  Condition Flag:");
    ansi.gotoXY(TAB1, y+3);
    ansi.println(mode.condition_flag ? "Cycle Requested" : "Battery OK");
    ansi.print("  Internal Charge Controller:");
    ansi.gotoXY(TAB1, y+4);
    ansi.println(mode.charge_controller_enabled ? "Enabled" : "Disabled");
    ansi.print("  Primary Battery:");
    ansi.gotoXY(TAB1, y+5);
    ansi.println(mode.primary_battery ? "Operating in primary role" : "Operating in secondary role");
    ansi.print("  Alarm Mode:");
    ansi.gotoXY(TAB1, y+6);
    ansi.println(mode.alarm_mode ? "Disabled AlarmWarning broadcast" : "Enabled AlarmWarning broadcasts");
    ansi.print("  Charger Mode:");
    ansi.gotoXY(TAB1, y+7);
    ansi.println(mode.charger_mode ? "Disabled broadcasts" : "Enabled broadcasts");
    ansi.print("  Capacity Mode:");
    ansi.gotoXY(TAB1, y+8);
    ansi.println(mode.capacity_mode ? "In 10mW or 10mWh" : "In mA or mAh");

    ansi.print("At Rate (0x04):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.atRate());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("At Rate Time To Full (0x05):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.atRateTimeToFull());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("At Rate Time To Empty (0x06):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.atRateTimeToEmpty());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("At Rate OK (0x07):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.atRateOK() ? "true" : "false");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Temperature (0x08):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.temperature(), 1);
    ansi.print(" Kelvin");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Temperature:");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.temperatureC(), 1);
    ansi.print(" Celsius");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Temperature:");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.temperatureF());
    ansi.print(" Fahrenheit");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    
    ansi.print("Voltage (0x09):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    float voltage = (float)battery.voltage()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Current (0x0a):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.current());
    ansi.print(" mA");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Average Current (0x0b):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.averageCurrent());
    ansi.print(" mA");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Max Error (0x0c):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.maxError());
    ansi.print("%");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Relative State Of Charge (0x0d):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.relativeStateOfCharge());
    ansi.print("% of Full Capacity");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
  
    ansi.print("Absolute State Of Charge (0x0e):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.absoluteStateOfCharge());
    ansi.print("% of Full Capacity");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Remaining Capacity (0x0f):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.remainingCapacity());
    ansi.print(mode.capacity_mode ? " 10mW" : " mA");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Full Capacity (0x10):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.fullCapacity());
    ansi.print(mode.capacity_mode ? " 10mWh" : " mAh");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Run Time To Empty (0x11):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.runTimeToEmpty());
    ansi.print(" minutes");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Average Time To Empty (0x12):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.avgTimeToEmpty());
    ansi.print(" minutes");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Average Time To Full (0x13):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.avgTimeToFull());
    ansi.print(" minutes");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Desired Charging Current (0x14):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.chargingCurrent());
    ansi.print(" mA");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Desired Charging Voltage (0x15):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.chargingVoltage());
    ansi.print(" mV");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Battery Status (0x16):");
    ansi.readCursorPosition(x, y);
    BatteryStatus stat = battery.batteryStatus();
    ansi.gotoXY(TAB1, y);
    ansi.print(stat.raw, HEX);
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    ansi.print("  Over Charged Alarm:");
    ansi.gotoXY(TAB1, y+1);
    ansi.println(stat.over_charged_alarm ? "Battery fully charged" : "Cleared");
    ansi.print("  Terminate Charge Alarm:");
    ansi.gotoXY(TAB1, y+2);
    ansi.println(stat.term_charge_alarm ? "Suspend charging" : "No charging, alarm cleared");
    ansi.print("  Over Temperature Alarm:");
    ansi.gotoXY(TAB1, y+3);
    ansi.println(stat.over_temp_alarm ? "Above limit" : "Within acceptable range");
    ansi.print("  Terminate Discharge Alarm:");
    ansi.gotoXY(TAB1, y+4);
    ansi.println(stat.term_discharge_alarm ? "Capacity depleted" : "Discharge not detected");
    ansi.print("  Remaining Capacity Alarm:");
    ansi.gotoXY(TAB1, y+5);
    ansi.println(stat.rem_capacity_alarm ? "True" : "False or 0");
    ansi.print("  Remaining Time Alarm:");
    ansi.gotoXY(TAB1, y+6);
    ansi.println(stat.rem_time_alarm ? "True" : "False or 0");
    ansi.print("  Initialized:");
    ansi.gotoXY(TAB1, y+7);
    ansi.println(stat.initialized ? "First calibrated" : "Calibration or configuration lost");
    ansi.print("  Discharging:");
    ansi.gotoXY(TAB1, y+8);
    ansi.println(stat.discharging ? "True" : "False");
    ansi.print("  Fully Charged:");
    ansi.gotoXY(TAB1, y+9);
    ansi.println(stat.fully_charged ? "True" : "False");
    ansi.print("  Fully Discharged:");
    ansi.gotoXY(TAB1, y+10);
    ansi.println(stat.fully_discharged ? "True" : "False");

    ansi.print("Cycle Count (0x17):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.cycleCount());
    ansi.print(" times");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Design Capacity (0x18):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.designCapacity());
    ansi.print(mode.capacity_mode ? " 10mWh" : " mAh");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Design Voltage (0x19):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    voltage = (float)battery.designVoltage()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("Protocol (0x1a):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.specificationInfo());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    
    ansi.print("Manufacture Date (0x1b): ");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.manufactureDate());
    ansi.print(" ");
    ansi.print(battery.manufactureDay());
    ansi.print("-");
    ansi.print(battery.manufactureMonth());
    ansi.print("-");
    ansi.print(battery.manufactureYear());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    
    ansi.print("Serial Number (0x1c):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.serialNumber());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    
    ansi.print("Manufacturer Name (0x20):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.manufacturerName());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    
    ansi.print("Device Name (0x21):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.deviceName());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    
    ansi.print("Device Chemistry (0x22):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.deviceChemistry());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    
    ansi.print("Manufacturer Data (0x23):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    ansi.print(battery.manufacturerData());
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    
  // Following functions are not part of the smart battery specification version 1.1
  // First Unseal to access extended SBS commands

    ansi.print("voltageCellOne (0x3f):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    voltage = (float)battery.voltageCellOne()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    
    ansi.print("voltageCellTwo (0x3e):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    voltage = (float)battery.voltageCellTwo()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);

    ansi.print("voltageCellThree (0x3d):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    voltage = (float)battery.voltageCellThree()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    
    ansi.print("voltageCellFour (0x3c):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    voltage = (float)battery.voltageCellFour()/1000;
    ansi.print(voltage);
    ansi.print(" V");
    ansi.gotoXY(TAB2, y);
    ansi.println(battery.I2CError.note);
    
    ansi.print("FET's status (0x46):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    FETcontrol fets = battery.FETControl();
    if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
    else { 
      ansi.print(fets.raw, HEX);
      ansi.gotoXY(TAB2, y);
      ansi.println(battery.I2CError.note);
      ansi.print("  Charge FET:");
      ansi.gotoXY(TAB1, y+1);
      ansi.println(fets.chg?"On":"Off");
      ansi.print("  DisCharge FET:");
      ansi.gotoXY(TAB1, y+2);
      ansi.println(fets.dsg?"On":"Off");
    }

    ansi.print("State Of Health (0x4f):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    uint16_t data = battery.stateOfHealth();
    if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
    else { 
      ansi.print(data + "%");
      ansi.gotoXY(TAB2, y);
      ansi.println(battery.I2CError.note);
    }

    ansi.print("Safety Alert (0x50):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    data = battery.Safetyalert().raw;
    if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
    else { 
      ansi.print(data, HEX);
      ansi.gotoXY(TAB2, y);
      ansi.println(battery.I2CError.note);
    }

    ansi.print("Safety Status (0x51):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    data = battery.Safetystatus().raw;
    if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
    else { 
      ansi.print(data, HEX);
      ansi.gotoXY(TAB2, y);
      ansi.println(battery.I2CError.note);
    }

    ansi.print("PF Alert (0x52):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    data = battery.PFalert().raw;
    if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
    else { 
      ansi.print(data, HEX);
      ansi.gotoXY(TAB2, y);
      ansi.println(battery.I2CError.note);
    }

    ansi.print("PF Status (0x53):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB1, y);
    data = battery.PFstatus().raw;
    if (battery.I2CError.nr) ansi.println("Not valid, is device unsealed ?");
    else { 
      ansi.print(data, HEX);
      ansi.gotoXY(TAB2, y);
      ansi.println(battery.I2CError.note);
    }
  } else {
    ansi.print("No Battery found, retrying !");
  }
}