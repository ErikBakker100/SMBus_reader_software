#include "display.h"
#include <bitset>

Display::Display(uint8_t address): BQICTYPE(address) {
  // list of the different commands, including function pointers to these funtions. This to be able to call them via user input
    info.emplace_back(&Display::displaymanufacturerAccess, DEVICEINFO, "ManufacturerAccess");
    info.emplace_back(&Display::displayremainingCapacityAlarm, USAGEINFO, "remainingCapacityAlarm");
    info.emplace_back(&Display::displayremainingTimeAlarm, USAGEINFO, "remainingTimeAlarm");
    info.emplace_back(&Display::displaybatteryMode, STATUSBITS, "batteryMode");
    info.emplace_back(&Display::displayatRate, ATRATES, "atRate");
    info.emplace_back(&Display::displayatRateTimeToFull, ATRATES, "atRateTimeToFull");
    info.emplace_back(&Display::displayatRateTimeToEmpty, ATRATES, "atRateTimeToEmpty");
    info.emplace_back(&Display::displayatRateOK, ATRATES, "atRateOK");
    info.emplace_back(&Display::displaytemperature, USAGEINFO, "temperature");
    info.emplace_back(&Display::displayvoltage, USAGEINFO, "voltage");
    info.emplace_back(&Display::displaycurrent, USAGEINFO, "current");
    info.emplace_back(&Display::displayaverageCurrent, USAGEINFO, "averageCurrent");
    info.emplace_back(&Display::displaymaxError, USAGEINFO, "maxError");
    info.emplace_back(&Display::displayrelativeStateOfCharge, COMPUTEDINFO, "relativeStateOfCharge");
    info.emplace_back(&Display::displayabsoluteStateOfCharge, COMPUTEDINFO, "absoluteStateOfCharge");
    info.emplace_back(&Display::displayremainingCapacity, USAGEINFO, "remainingCapacity");
    info.emplace_back(&Display::displayfullCapacity, DEVICEINFO, "fullCapacity");
    info.emplace_back(&Display::displayrunTimeToEmpty, COMPUTEDINFO, "runTimeToEmpty");
    info.emplace_back(&Display::displayavgTimeToEmpty, COMPUTEDINFO, "avgTimeToEmpty");
    info.emplace_back(&Display::displayavgTimeToFull, COMPUTEDINFO, "avgTimeToFull");
    info.emplace_back(&Display::displaychargingCurrent, USAGEINFO, "chargingCurrent");
    info.emplace_back(&Display::displaychargingVoltage, USAGEINFO, "chargingVoltage");
    info.emplace_back(&Display::displaybatteryStatus, STATUSBITS, "batteryStatus");
    info.emplace_back(&Display::displaycycleCount, USAGEINFO, "cycleCount");
    info.emplace_back(&Display::displaydesignCapacity, DEVICEINFO, "designCapacity");
    info.emplace_back(&Display::displaydesignVoltage, DEVICEINFO, "designVoltage");
    info.emplace_back(&Display::displayspecificationInfo, DEVICEINFO, "specificationInfo");
    info.emplace_back(&Display::displaymanufactureDate, DEVICEINFO, "manufactureDate");
    info.emplace_back(&Display::displayserialNumber, DEVICEINFO, "serialNumber");
    info.emplace_back(&Display::displaymanufacturerName, DEVICEINFO, "manufacturerName");
    info.emplace_back(&Display::displaydeviceName, DEVICEINFO, "deviceName");
    info.emplace_back(&Display::displaydeviceChemistry, DEVICEINFO, "deviceChemistry");
    info.emplace_back(&Display::displayoptionalMFGfunctions, USAGEINFO, "optionalMFGfunction_1-4");
    info.emplace_back(&Display::displaymanufacturerAccessType, DEVICEINFO, "manufacturerAccessType");
    info.emplace_back(&Display::displaymanufacturerAccessFirmware, DEVICEINFO, "manufacturerAccessFirmware");
    info.emplace_back(&Display::displaymanufacturerAccessHardware, DEVICEINFO, "manufacturerAccessHardware");
    info.emplace_back(&Display::displaymanufacturerAccessStatus, STATUSBITS, "manufacturerAccessStatus");
    info.emplace_back(&Display::displaymanufacturerAccessChemistryID, DEVICEINFO, "manufacturerAccessChemistryID");
    info.emplace_back(&Display::displaymanufacturerAccessShutdown, SET, "manufacturerAccessShutdown"); // Instructs the bq20z90/bq20z95 to verify and enter shutdown mode.
    info.emplace_back(&Display::displaymanufacturerAccessSleep, SET, "manufacturerAccessSleep"); // Instructs the bq20z90/bq20z95 to verify and enter sleep mode if no other command is sent after the Sleep command.
    info.emplace_back(&Display::displaymanufacturerAccessSeal, SET, "manufacturerAccessSeal");
    info.emplace_back(&Display::displaymanufacturerAccessPermanentFailClear, SET, "manufacturerAccessPermanentFailClear");
    info.emplace_back(&Display::displaymanufacturerAccessUnseal, SET, "manufacturerAccessUnseal");
    info.emplace_back(&Display::displaymanufacturerAccessFullAccess, SET, "manufacturerAccessFullAccess");
    info.emplace_back(&Display::displaymanufacturerData, DEVICEINFO, "manufacturerData");
    info.emplace_back(&Display::displayfetControl, SET, "fetControl");
    info.emplace_back(&Display::displaystateOfHealth, STATUSBITS, "stateOfHealth");
    info.emplace_back(&Display::displaysafetyAlert, DEVICEINFO, "safetyAlert");
    info.emplace_back(&Display::displaysafetyStatus, STATUSBITS, "safetyStatus");
    info.emplace_back(&Display::displaypfAlert, DEVICEINFO, "pfAlert");
    info.emplace_back(&Display::displaypfStatus, STATUSBITS, "pfStatus");
    info.emplace_back(&Display::displayoperationStatus, DEVICEINFO, "operationStatus");
    info.emplace_back(&Display::displayunsealKey, DEVICEINFO, "unsealKey");
}

void Display::displaymanufacturerAccess() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccess (0x00):");
  ansi.gotoXY(TAB2, y);
  ansi.print(manufacturerAccess(), HEX);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayremainingCapacityAlarm() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("remainingCapacityAlarm (0x01):");
  batteryMode(); // We need to get the Battery Mode first to determine output ranges further on
  ansi.gotoXY(TAB2, y);
  ansi.print(remainingCapacityAlarm());
  ansi.print(batterymode.bits.capacity_mode ? " 10mWh" : "mAh");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayremainingTimeAlarm() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("remainingTimeAlarm (0x02):");
  ansi.gotoXY(TAB2, y);
  ansi.print(remainingTimeAlarm());
  ansi.print(" minutes");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaybatteryMode() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("BatteryMode (0x03):"); 
  batteryMode();
  ansi.gotoXY(TAB2, y);
  printBits(batterymode.raw);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
  ansi.gotoXY(TAB1, y+1);
  ansi.print("Internal Charge Controller:");
  ansi.gotoXY(TAB2, y+1);
  ansi.println(batterymode.bits.internal_charge_controller ? "Supported" : "Not Supported");
  ansi.gotoXY(TAB1, y+2);
  ansi.print("Primary Battery Support:");
  ansi.gotoXY(TAB2, y+2);
  ansi.println(batterymode.bits.primary_battery_support ? "Supported" : "Not Supported");
  ansi.gotoXY(TAB1, y+3);
  ansi.print("Condition Flag:");
  ansi.gotoXY(TAB2, y+3);
  ansi.println(batterymode.bits.condition_flag ? "Cycle Requested" : "Battery OK");
  ansi.gotoXY(TAB1, y+4);
  ansi.print("Internal Charge Controller:");
  ansi.gotoXY(TAB2, y+4);
  ansi.println(batterymode.bits.charge_controller_enabled ? "Enabled" : "Disabled");
  ansi.gotoXY(TAB1, y+5);
  ansi.print("Primary Battery:");
  ansi.gotoXY(TAB2, y+5);
  ansi.println(batterymode.bits.primary_battery ? "Operating in primary role" : "Operating in secondary role");
  ansi.gotoXY(TAB1, y+6);
  ansi.print("Alarm Mode:");
  ansi.gotoXY(TAB2, y+6);
  ansi.println(batterymode.bits.alarm_mode ? "Broadcasts disabled " : "Broadcasts enabled ");
  ansi.gotoXY(TAB1, y+7);
  ansi.print("Charger Mode:");
  ansi.gotoXY(TAB2, y+7);
  ansi.println(batterymode.bits.charger_mode ? "Broadcasts disabled " : "Broadcasts enabled ");
  ansi.gotoXY(TAB1, y+8);
  ansi.print("Capacity Mode:");
  ansi.gotoXY(TAB2, y+8);
  ansi.println(batterymode.bits.capacity_mode ? "In 10mW or 10mWh" : "In mA or mAh");
}

void Display::displayatRate() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("At Rate (0x04):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(atRate());
  batteryMode();
  ansi.print(batterymode.bits.capacity_mode ? "x10mW" : "mA");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayatRateTimeToFull() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("At Rate Time To Full (0x05):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(atRateTimeToFull());
  ansi.print(" minutes");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayatRateTimeToEmpty() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("At Rate Time To Empty (0x06):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(atRateTimeToEmpty());
  ansi.print(" minutes");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayatRateOK() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("At Rate OK (0x07):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(atRateOK() ? "true" : "false");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaytemperature(){
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Temperature (0x08):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(temperature(), 1);
  ansi.print("K, ");
  ansi.print(temperatureC(), 1);
  ansi.print("C, ");
  ansi.print(temperatureF());
  ansi.println("F.");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayvoltage() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Voltage (0x09):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print((float)voltage()/1000);
  ansi.print("V");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaycurrent() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Current (0x0a):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(current());
  ansi.print("mA");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayaverageCurrent() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Average Current (0x0b):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(averageCurrent());
  ansi.print("mA");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaymaxError() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Max Error (0x0c):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(maxError());
  ansi.print("%");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayrelativeStateOfCharge() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Relative State Of Charge (0x0d):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(relativeStateOfCharge());
  ansi.print("% of Full Capacity");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayabsoluteStateOfCharge() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Absolute State Of Charge (0x0e):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(absoluteStateOfCharge());
  ansi.print("% of Full Capacity");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayremainingCapacity() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Remaining Capacity (0x0f):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(remainingCapacity());
  batteryMode();
  ansi.print(batterymode.bits.capacity_mode ? "x10mWh" : " mAh");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayfullCapacity() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Full Capacity (0x10):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(fullCapacity());
  batteryMode();
  ansi.print(batterymode.bits.capacity_mode ? "x10mWh" : " mAh");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayrunTimeToEmpty() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Run Time To Empty (0x11):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(runTimeToEmpty());
  ansi.print(" minutes");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayavgTimeToEmpty() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Average Time To Empty (0x12):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(avgTimeToEmpty());
  ansi.print(" minutes");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayavgTimeToFull() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Average Time To Full (0x13):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(avgTimeToFull());
  ansi.print(" minutes");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaychargingCurrent() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Desired Charging Current (0x14):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(chargingCurrent());
  ansi.print("mA");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaychargingVoltage() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Desired Charging Voltage (0x15):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(chargingVoltage());
  ansi.print("mV");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaybatteryStatus() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Battery Status (0x16):");
  ansi.readCursorPosition(x, y);
  batteryStatus();
  ansi.gotoXY(TAB2, y);
  printBits(batterystatus.raw);
  ansi.gotoXY(TAB3, y);
  ansi.print(I2Ccode[i2ccode]);
  ansi.gotoXY(TAB1, y+1);
  ansi.print("Fully Discharged:");
  ansi.gotoXY(TAB2, y+1);
  ansi.print(batterystatus.bits.fully_discharged ? "True" : "False");
  ansi.gotoXY(TAB1, y+2);
  ansi.print("Fully Charged:");
  ansi.gotoXY(TAB2, y+2);
  ansi.print(batterystatus.bits.fully_charged ? "True" : "False");
  ansi.gotoXY(TAB1, y+3);
  ansi.print("Discharging:");
  ansi.gotoXY(TAB2, y+3);
  ansi.print(batterystatus.bits.discharging ? "True" : "False");
  ansi.gotoXY(TAB1, y+4);
  ansi.print("Initialized:");
  ansi.gotoXY(TAB2, y+4);
  ansi.print(batterystatus.bits.initialized ? "Calibrated" : "Calibrating");
  ansi.gotoXY(TAB1, y+5);
  ansi.print("Remaining Time Alarm:");
  ansi.gotoXY(TAB2, y+5);
  ansi.print(batterystatus.bits.rem_time_alarm ? "Set" : "Not set");
  ansi.gotoXY(TAB1, y+6);
  ansi.print("Remaining Capacity Alarm:");
  ansi.gotoXY(TAB2, y+6);
  ansi.print(batterystatus.bits.rem_capacity_alarm ? "Set" : "Not set");
  ansi.gotoXY(TAB1, y+7);
  ansi.print("Terminate Discharge Alarm:");
  ansi.gotoXY(TAB2, y+7);
  ansi.print(batterystatus.bits.term_discharge_alarm ? "Capacity depleted" : "Discharge not detected");
  ansi.gotoXY(TAB1, y+8);
  ansi.print("Over Temperature Alarm:");
  ansi.gotoXY(TAB2, y+8);
  ansi.print(batterystatus.bits.over_temp_alarm ? "Above limit" : "Within acceptable range");
  ansi.gotoXY(TAB1, y+9);
  ansi.print("Terminate Charge Alarm:");
  ansi.gotoXY(TAB2, y+9);
  ansi.print(batterystatus.bits.term_charge_alarm ? "Suspend charging" : "No charging, alarm cleared");
  ansi.gotoXY(TAB1, y+10);    
  ansi.print("Over Charged Alarm:");
  ansi.gotoXY(TAB2, y+10);
  ansi.println(batterystatus.bits.over_charged_alarm ? "Battery fully charged" : "Cleared");
}

void Display::displaycycleCount() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Cycle Count (0x17):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(cycleCount());
  ansi.print(" times");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaydesignCapacity() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Design Capacity (0x18):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(designCapacity());
  ansi.print(batterymode.bits.capacity_mode ? "x10mWh" : " mAh");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaydesignVoltage() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Design Voltage (0x19):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print((float)designVoltage()/1000);
  ansi.print("V");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayspecificationInfo() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Protocol (0x1a):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(specificationInfo());
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaymanufactureDate() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Manufacture Date (0x1b): ");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(manufactureDay());
  ansi.print("-");
  ansi.print(manufactureMonth());
  ansi.print("-");
  ansi.print(manufactureYear());
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displayserialNumber() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Serial Number (0x1c):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(serialNumber());
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaymanufacturerName() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Manufacturer Name (0x20):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(manufacturerName());
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaydeviceName() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Device Name (0x21):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(deviceName());
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaydeviceChemistry() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Device Chemistry (0x22):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print(deviceChemistry());
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

// Following functions are not part of the smart battery specification version 1.1
void Display::displayoptionalMFGfunctions() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Voltage Cell 1 to 4 (0x3f-0x3c):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print((float)optionalMFGfunction4()/1000);
  ansi.print("V, ");
  ansi.print((float)optionalMFGfunction3()/1000);
  ansi.print("V, ");
  ansi.print((float)optionalMFGfunction2()/1000);
  ansi.print("V, ");
  ansi.print((float)optionalMFGfunction1()/1000);
  ansi.println("V.");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

// specific BQ20Zxx commands
void Display::displaymanufacturerAccessType() { // command 0x00 0x0001
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessType (0x00->0x0001):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print("BQ20Z");
  ansi.print(manufacturerAccessType(), HEX);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaymanufacturerAccessFirmware() {   // command 0x00
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Firmware version (0x00->0x0002):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  uint16_t version = manufacturerAccessFirmware();
  ansi.print(version >> 8 , DEC);
  ansi.print(".");
  ansi.print((uint8_t)version, DEC);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaymanufacturerAccessHardware() {   // command 0x00
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Hardware version (0x00->0x0003):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print((uint8_t)manufacturerAccessHardware(), HEX);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaymanufacturerAccessStatus() {     // command 0x00
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("ManufacturerStatus (0x00->0x0006):");
  manufacturerAccessStatus();
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  printBits(manufacturerstatus.raw);
  ansi.gotoXY(TAB3, y);
  ansi.print(I2Ccode[i2ccode]);
  ansi.gotoXY(TAB1, y+1);
  ansi.print("State: ");
  ansi.gotoXY(TAB2, y+1);
  ansi.print(statuscodes[manufacturerstatus.bits.state]);
  if (manufacturerstatus.bits.state == 9) ansi.print(", " + permanentfailurecodes[manufacturerstatus.bits.pf]);
  ansi.gotoXY(TAB1, y+2);
  ansi.print("FETs:");
  ansi.gotoXY(TAB2, y+2);
  ansi.println(fetcodes[manufacturerstatus.bits.fet]);
}

void Display::displaymanufacturerAccessChemistryID() { // command 0x00 0x0008
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessChemistryID (0x00->0x0008):");
  ansi.print(" ");
  ansi.print(manufacturerAccessChemistryID(), HEX);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaymanufacturerAccessShutdown(){// command 0x00 0x0010
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessShutdown (0x00->0x0010):");
  manufacturerAccessShutdown();
  ansi.gotoXY(TAB2, y);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaymanufacturerAccessSleep(){// command 0x00 0x0011
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessSleep (0x00->0x0011):");
  manufacturerAccessSleep();
  ansi.gotoXY(TAB2, y);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaymanufacturerAccessSeal() {       // command 0x00 0x0020
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessSeal (0x00->0x0020):");
  manufacturerAccessSeal();
  ansi.gotoXY(TAB2, y);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
  displaySealstatus();
}

void Display::displaymanufacturerAccessPermanentFailClear(uint16_t key_a, uint16_t key_b){
  uint16_t x, y; // x and y position
  if (displaySealstatus()) ansi.println("Put in Unsealed or Full Access mode first");
  else {
    manufacturerAccessPermanentFailClear(key_a, key_b);
    ansi.readCursorPosition(x, y);
    ansi.print("manufacturerAccessPermanentFailClear:");
    ansi.gotoXY(TAB2, y);
    ansi.print("Keys: a:");
    ansi.print(key_a, HEX);
    ansi.print(", b:");
    ansi.print(key_b, HEX);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[i2ccode]);
  }
}

void Display::displaymanufacturerAccessUnseal(uint16_t key_a, uint16_t key_b){
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessUnseal:");
  manufacturerAccessUnseal(key_a, key_b);
  ansi.gotoXY(TAB2, y);
  ansi.print("Used keys: a:");
  ansi.print(key_a, HEX);
  ansi.print(", b:");
  ansi.print(key_b, HEX);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaymanufacturerAccessFullAccess(uint16_t key_a, uint16_t key_b){
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessFullAccess:");
  manufacturerAccessFullAccess(key_a, key_b);
  ansi.gotoXY(TAB2, y);
  ansi.print("Used keys: a:");
  ansi.print(key_a, HEX);
  ansi.print(", b:");
  ansi.print(key_b, HEX);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaymanufacturerData() {             // command 0x23
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("ManufacturerData (0x23):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  manufacturerData();
  batteryStatus();
  for (uint8_t i = 0; i < 15; i++) {
    ansi.printf("%02x", manufacturerdata.raw[i]);
  }
  ansi.println(" " + I2Ccode[i2ccode]);
  ansi.gotoXY(TAB1, y+1);
  ansi.print("Pack Lot Code:");
  ansi.gotoXY(TAB2, y+1);
  ansi.print(manufacturerdata.bytes.PackLotCode, HEX);
  ansi.gotoXY(TAB1, y+2);
  ansi.print("PCB Lot Code:");
  ansi.gotoXY(TAB2, y+2);
  ansi.print(manufacturerdata.bytes.PCBLotCode, HEX);
  ansi.gotoXY(TAB1, y+3);
  ansi.print("Firmware Version:");
  ansi.gotoXY(TAB2, y+3);
  ansi.print(manufacturerdata.bytes.FirmwareVersion, HEX);
  ansi.gotoXY(TAB1, y+4);
  ansi.print("Hardware Revision:");
  ansi.gotoXY(TAB2, y+4);
  ansi.print(manufacturerdata.bytes.HardwareRevision, HEX);
  ansi.gotoXY(TAB1, y+5);
  ansi.print("Cell Revision:");
  ansi.gotoXY(TAB2, y+5);
  ansi.print(manufacturerdata.bytes.CellRevision, HEX);
  ansi.gotoXY(TAB1, y+6);
  ansi.print("Partial Reset Counter:");
  ansi.gotoXY(TAB2, y+6);
  ansi.print(manufacturerdata.bytes.PartialResetCounter, HEX);
  ansi.gotoXY(TAB1, y+7);
  ansi.print("Full Reset Counter:");
  ansi.gotoXY(TAB2, y+7);
  ansi.print(manufacturerdata.bytes.FullResetCounter, HEX);
  ansi.gotoXY(TAB1, y+8);
  ansi.print("Watchdog Reset Counter:");
  ansi.gotoXY(TAB2, y+8);
  ansi.print(manufacturerdata.bytes.WatchdogResetCounter, HEX);
  ansi.gotoXY(TAB1, y+9);
  ansi.print("Check Sum:");
  ansi.gotoXY(TAB2, y+9);
  ansi.println(manufacturerdata.bytes.CheckSum, HEX);
  ansi.gotoXY(TAB1, y+10);
  ansi.print("String Length:");
  ansi.gotoXY(TAB2, y+10);
  ansi.println(manufacturerdata.bytes.Length, DEC);
}

void Display::displayfetControl(){
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("FETControl (0x46):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  fetControl();
  if (i2ccode) {
    ansi.print(I2Ccode[i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(fetcontrol.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[i2ccode]);
    ansi.gotoXY(TAB1, y+1);
    ansi.print("Charge FET:");
    ansi.gotoXY(TAB2, y+1);
    ansi.println(fetcontrol.bits.chg?"On":"Off");
    ansi.gotoXY(TAB1, y+2);
    ansi.print("DisCharge FET:");
    ansi.gotoXY(TAB2, y+2);
    ansi.println(fetcontrol.bits.dsg?"On":"Off");
  }
}

void Display::displaystateOfHealth() {                // command 0x4f
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("State Of Health (0x4f):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  uint16_t data = stateOfHealth();
  if (i2ccode) {
    ansi.print(I2Ccode[i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    ansi.print(data + "%");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[i2ccode]);
  }
}

void Display::displaysafetyAlert() {                  // command 0x50
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Safety Alert (0x50):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  safetyAlert();
  if (i2ccode) {
    ansi.print(I2Ccode[i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(safetyalert.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[i2ccode]);
  }
}

void Display::displaysafetyStatus() {                 // command 0x51
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Safety Status (0x51):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  safetyStatus();
  if (i2ccode) {
    ansi.print(I2Ccode[i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(safetystatus.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[i2ccode]);
  }
}

void Display::displaypfAlert() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("PF Alert (0x52):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  pfAlert();
  if (i2ccode) {
    ansi.print(I2Ccode[i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(pfalert.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[i2ccode]);
  }
}

void Display::displaypfStatus() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("PF Status (0x53):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  pfStatus();
  if (i2ccode) {
    ansi.print(I2Ccode[i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(pfstatus.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[i2ccode]);
  }
}

void Display::displayoperationStatus() {              // command 0x54
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Operation Status (0x54):");
  ansi.gotoXY(TAB2, y);
  operationStatus();
  if (i2ccode) {
    ansi.print(I2Ccode[i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(operationstatus.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[i2ccode]);
  }
}

void Display::displayunsealKey(){                     // command 0x60
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("unsealKey (0x60):");
  ansi.gotoXY(TAB2, y);
  uint32_t key = unsealKey();
    if (i2ccode) {
    ansi.print(I2Ccode[i2ccode]);
    ansi.println(", is device in full access mode ?");
  } else { 
    ansi.print(key, HEX);;
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[i2ccode]);
  }
}

bool Display::findkey(uint8_t) {
  uint16_t key_a, key_b;
  for (uint16_t a = 0x00; a<= 0xffff; a++) {

  }
  return true;
}


// returns true if sealed, false otherwise
bool Display::displaySealstatus() {
  bool status {true};
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  operationStatus();
  ansi.gotoXY(TAB1, y);
  ansi.print("Mode: ");
  ansi.gotoXY(TAB2, y);
  if(!i2ccode) {
    status = false;
    ansi.print(operationstatus.bits.ss?"":"Unsealed");
    ansi.print(operationstatus.bits.fas?"":" Full access");
    ansi.println();
  } else ansi.println("Sealed");
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
  return status;
}

void Display::displayBatteryAddress() {
    uint16_t x, y; // x and y position
    ansi.readCursorPosition(x, y);
    ansi.print("Batteryaddress set to: ");
    ansi.gotoXY(TAB2, y);
    ansi.print("0x");
    ansi.print(address() < 0x10 ? "0": "");
    ansi.println(address(), HEX);
}

// Helper to simulate remove_cvref_t
template <typename T>
using remove_cvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

// Call a specific function by name
void Display::displayByName(const String& functionName) {
  
  auto it = std::find_if(info.begin(), info.end(), [&functionName](const Info<Display>& entry) {return entry.name == functionName;});
  if (it != info.end()) {
    std::visit([this](auto& f) {
      using FunctionType = remove_cvref_t<decltype(f)>;
      // Handle different member function signatures
      if constexpr (std::is_same_v<FunctionType, void (Display::*)()>) { 
          (this->*f)();
      } else if constexpr (std::is_same_v<FunctionType, void (Display::*)(uint16_t, uint16_t)>) {
          (this->*f)(0, 0); // Provide default arguments
      } else { Serial.println("Unsupported function type."); }
    }, it->dc);
  } else {
    Serial.print("Function \"" + functionName + "\" not found.\n");
  }
}

// Call all functions with the same classifier
void Display::displayByClassifier(uint8_t type) {
  if (type > 5) return;
  for (const auto& it : info) {
    if (it.monitor_group == type) {
      std::visit([this](auto& f) {
        using FunctionType = remove_cvref_t<decltype(f)>;
        // Handle different member function signatures
        if constexpr (std::is_same_v<FunctionType, void (Display::*)()>) {
            (this->*f)();
        } else if constexpr (std::is_same_v<FunctionType, void (Display::*)(uint16_t, uint16_t)>) {
            (this->*f)(0, 0); // Provide default arguments
        } else { Serial.println("Unsupported function type."); }
      }, it.dc);
    }
  }
}

void Display::displayCommandNames(){
    for (const auto& it : info) {
      Serial.println(it.name); 
    }
}

// prints 8-bit integer in this form: 0000 0000
void Display::printBits(uint8_t n) {
  byte numBits = 8;  // 2^numBits must be big enough to include the number n
  char b;
  char c = ' ';   // delimiter character
  for (byte i = 0; i < numBits; i++) {
    // shift 1 and mask to identify each bit value
    b = (n & (1 << (numBits - 1 - i))) > 0 ? '1' : '0'; // slightly faster to print chars than ints (saves conversion)
    ansi.print(b);
    if (i < (numBits - 1) && ((numBits-i - 1) % 4 == 0 )) ansi.print(c); // print a separator at every 4 bits
  }
}

// prints 16-bit integer in this form: 0000 0000 0000 0000
void Display::printBits(uint16_t n) {
  byte numBits = 16;  // 2^numBits must be big enough to include the number n
  char b;
  char c = ' ';   // delimiter character
  for (byte i = 0; i < numBits; i++) {
    // shift 1 and mask to identify each bit value
    b = (n & (1 << (numBits - 1 - i))) > 0 ? '1' : '0'; // slightly faster to print chars than ints (saves conversion)
    ansi.print(b);
    if (i < (numBits - 1) && ((numBits-i - 1) % 4 == 0 )) ansi.print(c); // print a separator at every 4 bits
  }
}
