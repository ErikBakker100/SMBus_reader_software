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

ArduinoSMBus battery(0x0B); // Replace with your battery's address

void setup() {
  Serial.begin(115200);
}

void loop() {
  delay(1000*5);
  Serial.print("Manufacturer Access (0x00): 0x");
  Serial.print(battery.manufacturerAccess(), HEX);
  Serial.println("\t" + *battery.ErrorCode());

  BatteryMode mode = battery.batteryMode(); // We need to read this first to determine output ranges further on

  Serial.print("Remaining Capacity Alarm (0x01): ");
  Serial.print(battery.remainingCapacityAlarm());
  Serial.println(mode.capacity_mode ? " 10mWh" : " mAh");
  Serial.println("\t" + *battery.ErrorCode());

  Serial.print("Remaining Time Alarm (0x02): ");
  Serial.println(battery.remainingTimeAlarm());
  Serial.println(" minutes\t" + *battery.ErrorCode());

  Serial.println("Battery Modes (0x03):"); 
  Serial.println("\t" + *battery.ErrorCode());
  Serial.print("\tInternal Charge Controller: ");
  Serial.println(mode.internal_charge_controller ? "Enabled" : "Disabled");
  Serial.print("\tPrimary Battery Support: ");
  Serial.println(mode.primary_battery_support ? "Enabled" : "Disabled");
  Serial.print("\tCondition Flag: ");
  Serial.println(mode.condition_flag ? "Set" : "Not Set");
  Serial.print("\tCharge Controller: ");
  Serial.println(mode.charge_controller_enabled ? "Enabled" : "Disabled");
  Serial.print("\tPrimary Battery: ");
  Serial.println(mode.primary_battery ? "Enabled" : "Disabled");
  Serial.print("\tAlarm Mode: ");
  Serial.println(mode.alarm_mode ? "Set" : "Not Set");
  Serial.print("\tCharger Mode: ");
  Serial.println(mode.charger_mode ? "Set" : "Not Set");
  Serial.print("\tCapacity Mode: ");
  Serial.println(mode.capacity_mode ? "Set" : "Not Set");
  Serial.print("At Rate (0x04): ");
  Serial.println(battery.atRate());
  Serial.print("At Rate Time To Full (0x05): ");
  Serial.println(battery.atRateTimeToFull());
  Serial.print("At Rate Time To Empty (0x06): ");
  Serial.println(battery.atRateTimeToEmpty());
  Serial.print("At Rate OK (0x07): ");
  Serial.println(battery.atRateOK());
  Serial.print("Temperature in Kelvin (0x08): ");
  Serial.println(battery.temperature());
  Serial.print("\tTemperature in Celsius: ");
  Serial.println(battery.temperatureC());
  Serial.print("\tTemperature in Fahrenheit: ");
  Serial.println(battery.temperatureF());
  Serial.print("Voltage in mV (0x09): ");
  Serial.println(battery.voltage());
  Serial.print("Current in mA (0x0a): ");
  Serial.println(battery.current());
  Serial.print("Average Current in mA (0x0b): ");
  Serial.println(battery.averageCurrent());
  Serial.print("Max Error in % (0x0c): ");
  Serial.println(battery.maxError());
  Serial.print("Relative State Of Charge in % of max (0x0d): ");
  Serial.println(battery.relativeStateOfCharge());
  Serial.print("Absolute State Of Charge in % of max (0x0e): ");
  Serial.println(battery.absoluteStateOfCharge());
  Serial.print("Remaining Capacity in mAh(0x0f): ");
  Serial.println(battery.remainingCapacity());
  Serial.print("Full Capacity in mAh (0x10): ");
  Serial.println(battery.fullCapacity());
  Serial.print("Run Time To Empty in minutes (0x11): ");
  Serial.println(battery.runTimeToEmpty());
  Serial.print("Average Time To Empty in minutes (0x12): ");
  Serial.println(battery.avgTimeToEmpty());
  Serial.print("Average Time To Full in minutes (0x13): ");
  Serial.println(battery.avgTimeToFull());
  Serial.print("Charging Current in mA (0x14):");
  Serial.println(battery.chargingCurrent());
  Serial.print("Charging Voltage in mV (0x15):");
  Serial.println(battery.chargingVoltage());
  Serial.println("Battery Status (0x16):");
  BatteryStatus status = battery.batteryStatus();
  Serial.print("  Over Charged Alarm: ");
  Serial.println(status.over_charged_alarm ? "True" : "False");
  Serial.print("  Terminate Charge Alarm: ");
  Serial.println(status.term_charge_alarm ? "True" : "False");
  Serial.print("  Over Temperature Alarm: ");
  Serial.println(status.over_temp_alarm ? "True" : "False");
  Serial.print("  Terminate Discharge Alarm: ");
  Serial.println(status.term_discharge_alarm ? "True" : "False");
  Serial.print("  Remaining Capacity Alarm: ");
  Serial.println(status.rem_capacity_alarm ? "True" : "False");
  Serial.print("  Remaining Time Alarm: ");
  Serial.println(status.rem_time_alarm ? "True" : "False");
  Serial.print("  Initialized: ");
  Serial.println(status.initialized ? "True" : "False");
  Serial.print("  Discharging: ");
  Serial.println(status.discharging ? "True" : "False");
  Serial.print("  Fully Charged: ");
  Serial.println(status.fully_charged ? "True" : "False");
  Serial.print("  Fully Discharged: ");
  Serial.println(status.fully_discharged ? "True" : "False");
  Serial.print("\tStatus OK: ");
  Serial.println(battery.statusOK());
  Serial.print("Cycle Count (0x17): ");
  Serial.println(battery.cycleCount());
  Serial.print("Design Capacity in mAh (0x18): ");
  Serial.println(battery.designCapacity());
  Serial.print("Design Voltage in mV (0x19): ");
  Serial.println(battery.designVoltage());
  Serial.print("Manufacture Date (0x1b): ");
  Serial.println(battery.manufactureDate());
  Serial.print("\tManufacture Day: ");
  Serial.println(battery.manufactureDay());
  Serial.print("\tManufacture Month: ");
  Serial.println(battery.manufactureMonth());
  Serial.print("\tManufacture Year: ");
  Serial.println(battery.manufactureYear());
  Serial.print("Serial Number (0x1c): ");
  Serial.println(battery.serialNumber());
  Serial.print("Manufacturer Name (0x20): ");
  Serial.println(battery.manufacturerName());
  Serial.print("Device Name (0x21): ");
  Serial.println(battery.deviceName());
  Serial.print("Device Chemistry (0x22): ");
  Serial.println(battery.deviceChemistry());
  Serial.print("Manufacturer Data (0x23): ");
  Serial.println(battery.manufacturerData());
  Serial.print("Optional Mfg Function (0x2f): ");
  Serial.println(battery.optionalMfgFunction());
  Serial.print("voltageCellFour (0x3c): ");
  Serial.println(battery.voltageCellFour());
  Serial.print("voltageCellThree (0x3d): ");
  Serial.println(battery.voltageCellThree());
  Serial.print("voltageCellTwo (0x3e): ");
  Serial.println(battery.voltageCellTwo());
  Serial.print("voltageCellOne (0x3f): ");
  Serial.println(battery.voltageCellOne());
  Serial.print("State Of Health (0x4f): ");
  Serial.println(battery.stateOfHealth());
  char tekst[] = "\033[H"; // Ansi terminal code for home position
  Serial.write(tekst, 5); // goto home position
}