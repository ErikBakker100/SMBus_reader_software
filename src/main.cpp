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

ArduinoSMBus battery;

void setup() {
  Serial.begin(115200);
}

void loop() {
  delay(1000*5);
  battery.setBatteryAddress(i2cscan());
  Serial.print("Manufacturer Access (0x00):\t\t0x");
  Serial.print(battery.manufacturerAccess(), HEX);
  Serial.println("\t\t\t" + battery.ErrorCode());

  BatteryMode mode = battery.batteryMode(); // We need to read this first to determine output ranges further on

  Serial.print("Remaining Capacity Alarm (0x01):\t");
  Serial.print(battery.remainingCapacityAlarm());
  Serial.print(mode.capacity_mode ? " 10mWh" : " mAh");
  Serial.println("\t\t\t" + battery.ErrorCode());

  Serial.print("Remaining Time Alarm (0x02):\t\t");
  Serial.print(battery.remainingTimeAlarm());
  Serial.println(" minutes\t\t" + battery.ErrorCode());

  Serial.print("Battery Modes (0x03):"); 
  Serial.println("\t\t\t\t\t\t" + battery.ErrorCode());
  Serial.print("  Internal Charge Controller:\t\t");
  Serial.println(mode.internal_charge_controller ? "Supported" : "Not Supported");
  Serial.print("  Primary Battery Support:\t\t");
  Serial.println(mode.primary_battery_support ? "Supported" : "Not Supported");
  Serial.print("  Condition Flag:\t\t\t");
  Serial.println(mode.condition_flag ? "Cycle Requested" : "Battery OK");
  Serial.print("  Internal Charge Controller:\t\t");
  Serial.println(mode.charge_controller_enabled ? "Enabled" : "Disabled");
  Serial.print("  Primary Battery:\t\t\t");
  Serial.println(mode.primary_battery ? "Operating in primary role" : "Operating in secondary role");
  Serial.print("  Alarm Mode:\t\t\t\t");
  Serial.println(mode.alarm_mode ? "Disabled AlarmWarning broadcast" : "Enabled AlarmWarning broadcasts");
  Serial.print("  Charger Mode:\t\t\t\t");
  Serial.println(mode.charger_mode ? "Disabled broadcasts of ChargingVoltage and Current" : "Enabled broadcasts of ChargingVoltage and Current");
  Serial.print("  Capacity Mode:\t\t\t");
  Serial.println(mode.capacity_mode ? "In 10mW or 10mWh" : "In mA or mAh");

  Serial.print("At Rate (0x04):\t\t\t\t");
  Serial.print(battery.atRate());
  Serial.println("\t\t\t" + battery.ErrorCode());

  Serial.print("At Rate Time To Full (0x05):\t\t");
  Serial.print(battery.atRateTimeToFull());
  Serial.println("\t\t\t" + battery.ErrorCode());

  Serial.print("At Rate Time To Empty (0x06):\t\t");
  Serial.print(battery.atRateTimeToEmpty());
  Serial.println("\t\t\t" + battery.ErrorCode());

  Serial.print("At Rate OK (0x07):\t\t\t");
  Serial.print(battery.atRateOK() ? "true" : "false");
  Serial.println("\t\t\t" + battery.ErrorCode());

  Serial.print("Temperature (0x08):\t\t");
  Serial.print(battery.temperature(), 1);
  Serial.println(" Kelvin\t\t" + battery.ErrorCode());

  Serial.print("\tTemperature:\t\t");
  Serial.print(battery.temperatureC(), 1);
  Serial.println(" Celsius\t\t" + battery.ErrorCode());

  Serial.print("\tTemperature:\t\t");
  Serial.print(battery.temperatureF()); 
  Serial.println(" Fahrenheit\t\t" + battery.ErrorCode());
  
  Serial.print("Voltage (0x09):\t\t\t");
  Serial.print(battery.voltage());
  Serial.println(" mV\t\t" + battery.ErrorCode());

  Serial.print("Current (0x0a):\t\t");
  Serial.print(battery.current());
  Serial.println("\t\t" + battery.ErrorCode());
  Serial.println("\t\t\tmA - for discharge, + for charge\t\t" + battery.ErrorCode());

  Serial.print("Average Current (0x0b):\t");
  Serial.print(battery.averageCurrent());
  Serial.println("\t\t" + battery.ErrorCode());
  Serial.println("\t\tmA - for discharge, + for charge\t" + battery.ErrorCode());

  Serial.print("Max Error (0x0c):\t\t\t");
  Serial.print(battery.maxError());
  Serial.println("%\t\t\t" + battery.ErrorCode());

  Serial.print("Relative State Of Charge (0x0d): ");
  Serial.print(battery.relativeStateOfCharge());
  Serial.println("% of Full Charge Capacity\t" + battery.ErrorCode());
 
  Serial.print("Absolute State Of Charge (0x0e): ");
  Serial.print(battery.absoluteStateOfCharge());
  Serial.println("% of Full Charge Capacity\t" + battery.ErrorCode());

  Serial.print("Remaining Capacity (0x0f):\t");
  Serial.print(battery.remainingCapacity());
  Serial.print(mode.capacity_mode ? " 10mW" : " mA");
  Serial.println("\t\t\t\t" + battery.ErrorCode());

  Serial.print("Full Capacity (0x10):\t");
  Serial.print(battery.fullCapacity());
  Serial.print(mode.capacity_mode ? " 10mWh" : " mAh");
  Serial.println("\t\t\t\t" + battery.ErrorCode());

  Serial.print("Run Time To Empty (0x11):\t");
  Serial.print(battery.runTimeToEmpty());
  Serial.println(" minutes\t\t\t" + battery.ErrorCode());

  Serial.print("Average Time To Empty (0x12):\t");
  Serial.print(battery.avgTimeToEmpty());
  Serial.println(" minutes\t\t" + battery.ErrorCode());

  Serial.print("Average Time To Full (0x13):\t");
  Serial.print(battery.avgTimeToFull());
  Serial.println(" minutes\t\t" + battery.ErrorCode());

  Serial.print("Desired Charging Current (0x14):");
  Serial.print(battery.chargingCurrent());
  Serial.println(" mA\t\t\t\t" + battery.ErrorCode());
  Serial.print("Desired Charging Voltage (0x15):");
  Serial.print(battery.chargingVoltage());
  Serial.println(" mV\t\t\t\t" + battery.ErrorCode());

  Serial.print("Battery Status (0x16):");
  Serial.println("\t\t\t\t\t\t" + battery.ErrorCode());
  BatteryStatus status = battery.batteryStatus();
  Serial.print("  Over Charged Alarm:\t\t");
  Serial.println(status.over_charged_alarm ? "Fully charged" : "No charging, alarm cleared");
  Serial.print("  Terminate Charge Alarm:\t");
  Serial.println(status.term_charge_alarm ? "Suspend charging" : "No charging, alarm cleared");
  Serial.print("  Over Temperature Alarm:\t");
  Serial.println(status.over_temp_alarm ? "Above limit" : "Within acceptable range");
  Serial.print("  Terminate Discharge Alarm:\t");
  Serial.println(status.term_discharge_alarm ? "Capacity depleted" : "Discharge not detected");
  Serial.print("  Remaining Capacity Alarm:\t");
  Serial.println(status.rem_capacity_alarm ? "RemainingCapacity() < RemainingCapacityAlarm()" : "RemainingCapacityAlarm() < RemainingCapacity() or 0");
  Serial.print("  Remaining Time Alarm:\t\t");
  Serial.println(status.rem_time_alarm ? "AverageTimeToEmpty() < RemainingTimeAlarm()" : "RemainingTimeAlarm() < AverageTimeToEmpty() or 0");
  Serial.print("  Initialized:\t\t\t");
  Serial.println(status.initialized ? "First calibrated" : "Calibration or configuration information lost");
  Serial.print("  Discharging:\t\t\t");
  Serial.println(status.discharging ? "True" : "False");
  Serial.print("  Fully Charged:\t\t");
  Serial.println(status.fully_charged ? "True" : "False");
  Serial.print("  Fully Discharged:\t\t");
  Serial.println(status.fully_discharged ? "True" : "RelativeStateOfCharge() > 20%");

  Serial.print("Cycle Count (0x17):\t");
  Serial.print(battery.cycleCount());
  Serial.println(" times\t\t\t\t" + battery.ErrorCode());

  Serial.print("Design Capacity (0x18):\t");
  Serial.print(battery.designCapacity());
  Serial.print(mode.capacity_mode ? " 10mWh" : " mAh");
  Serial.println("\t\t\t" + battery.ErrorCode());

  Serial.print("Design Voltage (0x19):\t");
  Serial.print(battery.designVoltage());
  Serial.println(" mV\t\t\t\t" + battery.ErrorCode());

  Serial.print("Protocol (0x1a):\t");
  Serial.print(battery.specificationInfo());
  Serial.println("\t\t\t\t\t" + battery.ErrorCode());
  
  Serial.print("Manufacture Date (0x1b): ");
  Serial.print(battery.manufactureDate());
  Serial.print("\tDay: ");
  Serial.print(battery.manufactureDay());
  Serial.print(" Month: ");
  Serial.print(battery.manufactureMonth());
  Serial.print(" Year: ");
  Serial.print(battery.manufactureYear());
  Serial.println("\t" + battery.ErrorCode());
  
  Serial.print("Serial Number (0x1c):\t");
  Serial.print(battery.serialNumber());
  Serial.println("\t\t\t\t" + battery.ErrorCode());
  
  Serial.print("Manufacturer Name (0x20):\t");
  Serial.print(battery.manufacturerName());
  Serial.println("\t\t\t\t" + battery.ErrorCode());
  
  Serial.print("Device Name (0x21):\t");
  Serial.print(battery.deviceName());
  Serial.println("\t\t\t\t" + battery.ErrorCode());
  
  Serial.print("Device Chemistry (0x22):\t");
  Serial.print(battery.deviceChemistry());
  Serial.println("\t\t\t\t" + battery.ErrorCode());
  
  Serial.print("Manufacturer Data (0x23):\t");
  Serial.print(battery.manufacturerData());
  Serial.println("\t\t\t\t" + battery.ErrorCode());
  
// Following functions are not part of the smart battery specification version 1.1
  Serial.print("voltageCellFour (0x3c):\t");
  Serial.print(battery.voltageCellFour());
  Serial.println("\t\t\t\t" + battery.ErrorCode());
  
  Serial.print("voltageCellThree (0x3d):\t");
  Serial.print(battery.voltageCellThree());
  Serial.println("\t\t\t\t" + battery.ErrorCode());
  
  Serial.print("voltageCellTwo (0x3e):\t");
  Serial.print(battery.voltageCellTwo());
  Serial.println("\t\t\t\t" + battery.ErrorCode());
  
  Serial.print("voltageCellOne (0x3f):\t");
  Serial.print(battery.voltageCellOne());
  Serial.println("\t\t\t\t" + battery.ErrorCode());

  Serial.print("FET's status (0x46):\t");
  FETcontrol fets = battery.FETControl();
  Serial.print(fets.raw, HEX);
  Serial.println("\t\t\t\t\t" + battery.ErrorCode());  
  Serial.print("  Charge FET:\t\t");
  Serial.println(fets.chg?"ON":"OFF");
  Serial.print("  DisCharge FET:\t\t");
  Serial.println(fets.dsg?"ON":"OFF");

  Serial.print("State Of Health (0x4f):\t");
  Serial.print(battery.stateOfHealth());
  Serial.println("%\t\t\t\t" + battery.ErrorCode());

  Serial.print("Safety Alert (0x50):\t");
  Serial.print(battery.Safetyalert().raw);
  Serial.println("%\t\t\t\t" + battery.ErrorCode());

  Serial.print("Safety Status (0x51):\t");
  Serial.print(battery.Safetystatus().raw);
  Serial.println("%\t\t\t\t" + battery.ErrorCode());

  Serial.print("PF Alert (0x52):\t");
  Serial.print(battery.PFalert().raw);
  Serial.println("%\t\t\t\t\t" + battery.ErrorCode());

  Serial.print("PF Status (0x53):\t");
  Serial.print(battery.PFstatus().raw);
  Serial.println("%\t\t\t\t" + battery.ErrorCode());

  char tekst[] = "\033[H"; // Ansi terminal code for home position
  Serial.write(tekst, 5); // goto home position
}