#include "display.h"
#include <bitset>

Display::Display(uint8_t address) : smbuscommands(address) {
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
  manufacturerName();
  ansi.print(smbus::text);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaydeviceName() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Device Name (0x21):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  deviceName();
  ansi.print(smbus::text);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void Display::displaydeviceChemistry() {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Device Chemistry (0x22):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  deviceChemistry();
  ansi.print(smbus::text);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

// Following functions are not part of the smart battery specification version 1.1
void Display::displayBatteryAddress() {
    uint16_t x, y; // x and y position
    ansi.readCursorPosition(x, y);
    ansi.print("Batteryaddress set to: ");
    ansi.gotoXY(TAB2, y);
    ansi.print("0x");
    ansi.print(address() < 0x10 ? "0": "");
    ansi.println(address(), HEX);
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

// prints 32-bit integer in this form: 0000 0000 0000 0000 0000 0000 0000 0000
void Display::printBits(uint32_t n) {
  byte numBits = 32;  // 2^numBits must be big enough to include the number n
  char b;
  char c = ' ';   // delimiter character
  for (byte i = 0; i < numBits; i++) {
    // shift 1 and mask to identify each bit value
    b = (n & (1 << (numBits - 1 - i))) > 0 ? '1' : '0'; // slightly faster to print chars than ints (saves conversion)
    ansi.print(b);
    if (i < (numBits - 1) && ((numBits-i - 1) % 4 == 0 )) ansi.print(c); // print a separator at every 4 bits
  }
}
