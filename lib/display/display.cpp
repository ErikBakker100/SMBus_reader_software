#include "display.hpp"

void displayMainmenu() {
    ansi.clearScreen();
    ansi.println("1 = Menu,                   This menu");
    ansi.println("2 = Search address,         Find address, use 2 x x for start and end address.");
    ansi.println("3 = Standard registers,     Registers as defined in rev 1.1 of the smart battery data specification.");
    ansi.println("4 = Extended registers,     ");
    ansi.println("5 = Unseal Battery,         ");
    ansi.println("6 = Seal Battery,           ");
    ansi.println("7 = Clear Permanent Failure ");
    ansi.println("8 = Specify Command         ");
}

void displaySmallmenu() {
    ansi.clearScreen();
    ansi.println(" 1=Menu, 2=Search, 3=Standard, 4=Extended, 5=Unseal, 6=Seal, 7=Clear PF, 8=Specify Command");
}

static char* prntBits(uint8_t b);

void displayBatteryNr(uint8_t address) {
    if (address > 0) {
    uint16_t x, y; // x and y position
    ansi.print("Batteryaddress set to: ");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print("0x");
    ansi.print(address < 0x10 ? "0": "");
    ansi.println(address, HEX);
    } else ansi.println("No Battery found !");
}

void display_standard(ArduinoSMBus& battery) {
    uint16_t x, y; // x and y position
    ansi.print("Device Type (0x00 -> 0x0001):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print("bqxxx");
    ansi.print(battery.manufacturerAccessType(), HEX);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    
    ansi.print("Firmware version (0x00 -> 0x0002):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    uint16_t version = battery.manufacturerAccessFirmware();
    ansi.print(version >> 8 , HEX);
    ansi.print(".");
    ansi.print((uint8_t)version & 0x00ff);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Hardware version (0x00 -> 0x0003):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    version = battery.manufacturerAccessHardware();
    ansi.print((uint8_t)version & 0x00ff, HEX);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Battery Status (0x00 -> 0x0006):");
    ManufacturerStatus status = battery.manufacturerStatus();
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(prntBits(highByte(status.raw)));
    ansi.print(prntBits(lowByte(status.raw)));
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    ansi.gotoXY(TAB1, y+1);
    ansi.print("Status: ");
    ansi.gotoXY(TAB2, y+1);
    ansi.println(status.failure + " " + status.permfailure);
    ansi.gotoXY(TAB1, y+2);
    ansi.print("Charge FET:");
    ansi.gotoXY(TAB2, y+2);
    ansi.println(status.chg_fet ? "On" : "Off");
    ansi.gotoXY(TAB1, y+3);
    ansi.print("Discharge FET:");
    ansi.gotoXY(TAB2, y+3);
    ansi.println(status.dsg_fet ? "On" : "Off");

    battery.batteryMode(); // We need to get the Battery Mode first to determine output ranges further on

    ansi.print("Remaining Capacity Alarm (0x01):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.remainingCapacityAlarm());
    ansi.print(BatteryMode.bits.capacity_mode ? " 10mWh" : "mAh");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Remaining Time Alarm (0x02):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.remainingTimeAlarm());
    ansi.print(" minutes");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Battery Modes (0x03):"); 
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(prntBits(highByte(BatteryMode.raw)));
    ansi.print(prntBits(lowByte(BatteryMode.raw)));
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    ansi.gotoXY(TAB1, y+1);
    ansi.print("Internal Charge Controller:");
    ansi.gotoXY(TAB2, y+1);
    ansi.println(BatteryMode.bits.internal_charge_controller ? "Supported" : "Not Supported");
    ansi.gotoXY(TAB1, y+2);
    ansi.print("Primary Battery Support:");
    ansi.gotoXY(TAB2, y+2);
    ansi.println(BatteryMode.bits.primary_battery_support ? "Supported" : "Not Supported");
    ansi.gotoXY(TAB1, y+3);
    ansi.print("Condition Flag:");
    ansi.gotoXY(TAB2, y+3);
    ansi.println(BatteryMode.bits.condition_flag ? "Cycle Requested" : "Battery OK");
    ansi.gotoXY(TAB1, y+4);
    ansi.print("Internal Charge Controller:");
    ansi.gotoXY(TAB2, y+4);
    ansi.println(BatteryMode.bits.charge_controller_enabled ? "Enabled" : "Disabled");
    ansi.gotoXY(TAB1, y+5);
    ansi.print("Primary Battery:");
    ansi.gotoXY(TAB2, y+5);
    ansi.println(BatteryMode.bits.primary_battery ? "Operating in primary role" : "Operating in secondary role");
    ansi.gotoXY(TAB1, y+6);
    ansi.print("Alarm Mode:");
    ansi.gotoXY(TAB2, y+6);
    ansi.println(BatteryMode.bits.alarm_mode ? "Broadcasts disabled " : "Broadcasts enabled ");
    ansi.gotoXY(TAB1, y+7);
    ansi.print("Charger Mode:");
    ansi.gotoXY(TAB2, y+7);
    ansi.println(BatteryMode.bits.charger_mode ? "Broadcasts disabled " : "Broadcasts enabled ");
    ansi.gotoXY(TAB1, y+8);
    ansi.print("Capacity Mode:");
    ansi.gotoXY(TAB2, y+8);
    ansi.println(BatteryMode.bits.capacity_mode ? "In 10mW or 10mWh" : "In mA or mAh");

    ansi.print("At Rate (0x04):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.atRate());
    ansi.print(BatteryMode.bits.capacity_mode ? "x10mW" : "mA");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("At Rate Time To Full (0x05):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.atRateTimeToFull());
    ansi.print(" minutes");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("At Rate Time To Empty (0x06):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.atRateTimeToEmpty());
    ansi.print(" minutes");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("At Rate OK (0x07):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.atRateOK() ? "true" : "false");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Temperature (0x08):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    ansi.gotoXY(TAB2, y+1);
    ansi.print(battery.temperature(), 1);
    ansi.print("K, ");
    ansi.print(battery.temperatureC(), 1);
    ansi.print("C,");
    ansi.print(battery.temperatureF());
    ansi.println("F.");

    ansi.print("Voltage (0x09):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    float voltage = (float)battery.voltage()/1000;
    ansi.print(voltage);
    ansi.print("V");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Current (0x0a):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.current());
    ansi.print("mA");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Average Current (0x0b):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.averageCurrent());
    ansi.print("mA");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Max Error (0x0c):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.maxError());
    ansi.print("%");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Relative State Of Charge (0x0d):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.relativeStateOfCharge());
    ansi.print("% of Full Capacity");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Absolute State Of Charge (0x0e):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.absoluteStateOfCharge());
    ansi.print("% of Full Capacity");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Remaining Capacity (0x0f):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.remainingCapacity());
    ansi.print(BatteryMode.bits.capacity_mode ? "x10mWh" : " mAh");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Full Capacity (0x10):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.fullCapacity());
    ansi.print(BatteryMode.bits.capacity_mode ? "x10mWh" : " mAh");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Run Time To Empty (0x11):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.runTimeToEmpty());
    ansi.print(" minutes");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Average Time To Empty (0x12):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.avgTimeToEmpty());
    ansi.print(" minutes");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Average Time To Full (0x13):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.avgTimeToFull());
    ansi.print(" minutes");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Desired Charging Current (0x14):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.chargingCurrent());
    ansi.print("mA");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Desired Charging Voltage (0x15):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.chargingVoltage());
    ansi.print("mV");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Battery Status (0x16):");
    ansi.readCursorPosition(x, y);
    battery.batteryStatus();
    ansi.gotoXY(TAB2, y);
    ansi.print(prntBits(highByte(BatteryStatus.raw)));
    ansi.print(prntBits(lowByte(BatteryStatus.raw)));
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    ansi.gotoXY(TAB1, y+1);
    ansi.print("Over Charged Alarm:");
    ansi.gotoXY(TAB2, y+1);
    ansi.println(BatteryStatus.bits.over_charged_alarm ? "Battery fully charged" : "Cleared");
    ansi.gotoXY(TAB1, y+2);
    ansi.print("Terminate Charge Alarm:");
    ansi.gotoXY(TAB2, y+2);
    ansi.println(BatteryStatus.bits.term_charge_alarm ? "Suspend charging" : "No charging, alarm cleared");
    ansi.gotoXY(TAB1, y+3);
    ansi.print("Over Temperature Alarm:");
    ansi.gotoXY(TAB2, y+3);
    ansi.println(BatteryStatus.bits.over_temp_alarm ? "Above limit" : "Within acceptable range");
    ansi.gotoXY(TAB1, y+4);
    ansi.print("Terminate Discharge Alarm:");
    ansi.gotoXY(TAB2, y+4);
    ansi.println(BatteryStatus.bits.term_discharge_alarm ? "Capacity depleted" : "Discharge not detected");
    ansi.gotoXY(TAB1, y+5);
    ansi.print("Remaining Capacity Alarm:");
    ansi.gotoXY(TAB2, y+5);
    ansi.println(BatteryStatus.bits.rem_capacity_alarm ? "True" : "False or 0");
    ansi.gotoXY(TAB1, y+6);
    ansi.print("Remaining Time Alarm:");
    ansi.gotoXY(TAB2, y+6);
    ansi.println(BatteryStatus.bits.rem_time_alarm ? "True" : "False or 0");
    ansi.gotoXY(TAB1, y+7);
    ansi.print("Initialized:");
    ansi.gotoXY(TAB2, y+7);
    ansi.println(BatteryStatus.bits.initialized ? "First calibrated" : "Calibration or configuration lost");
    ansi.gotoXY(TAB1, y+8);
    ansi.print("Discharging:");
    ansi.gotoXY(TAB2, y+8);
    ansi.println(BatteryStatus.bits.discharging ? "True" : "False");
    ansi.gotoXY(TAB1, y+9);
    ansi.print("Fully Charged:");
    ansi.gotoXY(TAB2, y+9);
    ansi.println(BatteryStatus.bits.fully_charged ? "True" : "False");
    ansi.gotoXY(TAB1, y+10);
    ansi.print("Fully Discharged:");
    ansi.gotoXY(TAB2, y+10);
    ansi.println(BatteryStatus.bits.fully_discharged ? "True" : "False");

    ansi.print("Cycle Count (0x17):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.cycleCount());
    ansi.print(" times");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Design Capacity (0x18):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.designCapacity());
    ansi.print(BatteryMode.bits.capacity_mode ? "x10mWh" : " mAh");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    ansi.print("Design Voltage (0x19):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    voltage = (float)battery.designVoltage()/1000;
    ansi.print(voltage);
    ansi.print("V");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Protocol (0x1a):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.specificationInfo());
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Manufacture Date (0x1b): ");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.manufactureDay());
    ansi.print("-");
    ansi.print(battery.manufactureMonth());
    ansi.print("-");
    ansi.print(battery.manufactureYear());
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Serial Number (0x1c):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.serialNumber());
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Manufacturer Name (0x20):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.manufacturerName());
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
   
    ansi.print("Device Name (0x21):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.deviceName());
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Device Chemistry (0x22):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.deviceChemistry());
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Manufacturer Data (0x23):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    const char* data = battery.manufacturerData();
    ansi.gotoXY(TAB1, y+1);
    ansi.print("Pack Lot Code:");
    ansi.gotoXY(TAB2, y+1);
    ansi.print(data[0] + data[1], HEX);
    ansi.gotoXY(TAB1, y+2);
    ansi.print("PCB Lot Code:");
    ansi.gotoXY(TAB2, y+2);
    ansi.print(data[2] + data[3], HEX);
    ansi.gotoXY(TAB1, y+3);
    ansi.print("Firmware Version:");
    ansi.gotoXY(TAB2, y+3);
    ansi.print(data[4] + data[5], HEX);
    ansi.gotoXY(TAB1, y+4);
    ansi.print("Hardware Revision:");
    ansi.gotoXY(TAB2, y+4);
    ansi.print(data[6] + data[7], HEX);
    ansi.gotoXY(TAB1, y+5);
    ansi.print("Cell Revision:");
    ansi.gotoXY(TAB2, y+5);
    ansi.print(data[8] + data[9], HEX);
    ansi.gotoXY(TAB1, y+6);
    ansi.print("Partial Reset Counter:");
    ansi.gotoXY(TAB2, y+6);
    ansi.print(data[10], HEX);
    ansi.gotoXY(TAB1, y+7);
    ansi.print("Full Reset Counter:");
    ansi.gotoXY(TAB2, y+7);
    ansi.print(data[11], HEX);
    ansi.gotoXY(TAB1, y+8);
    ansi.print("Watchdog Reset Counter:");
    ansi.gotoXY(TAB2, y+8);
    ansi.print(data[12], HEX);
    ansi.gotoXY(TAB1, y+9);
    ansi.print("Check Sum:");
    ansi.gotoXY(TAB2, y+9);
    ansi.print(data[13], HEX);
    ansi.gotoXY(TAB1, y+10);
    ansi.print("String Length Byte:");
    ansi.gotoXY(TAB2, y+10);
    ansi.println(data[14], HEX);

    // Following functions are not part of the smart battery specification version 1.1

    ansi.print("Voltage Cell 1 to 4 (0x3f - 0x3c):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    ansi.gotoXY(TAB2, y+1);
    voltage = (float)battery.voltageCellOne()/1000;
    ansi.print(voltage);
    ansi.print("V, ");
    voltage = (float)battery.voltageCellTwo()/1000;
    ansi.print(voltage);
    ansi.print("V, ");
    voltage = (float)battery.voltageCellThree()/1000;
    ansi.print(voltage);
    ansi.print("V, ");
    voltage = (float)battery.voltageCellFour()/1000;
    ansi.print(voltage);
    ansi.println("V.");
}

void display_bq2020z9xx(ArduinoSMBus& battery) {
  uint16_t x, y; // x and y position
  ansi.print("FET's status (0x46):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  FETcontrol fets = battery.FETControl();
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    ansi.print(prntBits(highByte(fets.raw)));
    ansi.print(prntBits(lowByte(fets.raw)));
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    ansi.gotoXY(TAB1, y+1);
    ansi.print("Charge FET:");
    ansi.gotoXY(TAB2, y+1);
    ansi.println(fets.chg?"On":"Off");
    ansi.gotoXY(TAB1, y+2);
    ansi.print("DisCharge FET:");
    ansi.gotoXY(TAB2, y+2);
    ansi.println(fets.dsg?"On":"Off");
  }

  ansi.print("State Of Health (0x4f):");
  ansi.gotoXY(TAB2, y);
  uint16_t data = battery.stateOfHealth();
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    ansi.print(data + "%");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
  }

  ansi.print("Safety Alert (0x50):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  data = battery.Safetyalert().raw;
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    ansi.print(prntBits(highByte(data)));
    ansi.print(prntBits(lowByte(data)));
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
  }

  ansi.print("Safety Status (0x51):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  data = battery.Safetystatus().raw;
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    ansi.print(prntBits(highByte(data)));
    ansi.print(prntBits(lowByte(data)));
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
  }

  ansi.print("PF Alert (0x52):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  data = battery.PFalert().raw;
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    ansi.print(prntBits(highByte(data)));
    ansi.print(prntBits(lowByte(data)));
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
  }

  ansi.print("PF Status (0x53):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  data = battery.PFstatus().raw;
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    ansi.print(prntBits(highByte(data)));
    ansi.print(prntBits(lowByte(data)));
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
  }

  ansi.print("Operation Status (0x54):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  data = battery.Operationstatus().raw;
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    ansi.print(prntBits(highByte(data)));
    ansi.print(prntBits(lowByte(data)));
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
  }
}

void displaySealstatus(ArduinoSMBus& battery) {
  OperationStatus status = battery.Operationstatus();
  ansi.print(I2Ccode[battery.i2ccode]);
  ansi.println(" " + ErrorCodes[battery.baterrorcode]);
  ansi.println(status.ss?"sealed mode enabled":"sealed mode disabled");
  ansi.println(status.fas?"full access mode enabled":"full access mode disabled");
}

static char* prntBits(uint8_t b) {
  static char bits[9];
  for(int i = 7; i >= 0; i--) bits[7-i] = bitRead(b,i)? '1': '0'; // bitRead, 0 = right most bit
  bits[8]='\0';
  return bits;
}

void displayBatteryStatus(ArduinoSMBus& battery) {
    ansi.print("Battery Status (0x16):");
    uint16_t x, y;
    ansi.readCursorPosition(x, y);
    battery.remainingCapacity();
    battery.batteryStatus();
    ansi.gotoXY(TAB2, y);
    ansi.print(prntBits(highByte(BatteryStatus.raw)));
    ansi.print(prntBits(lowByte(BatteryStatus.raw)));
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    ansi.println(" " + ErrorCodes[battery.baterrorcode]);
}