#include "display.hpp"
#include <bitset>

void displayMainmenu() {
    ansi.clearScreen();
    ansi.println("1 = Menu,                   This menu");
    ansi.println("2 = Search address,         Find address, use 2 x x for start and end address.");
    ansi.println("3 = Standard SBS commands,  Reads Registers as defined in rev 1.1 of the smart battery data specification.");
    ansi.println("4 = IC specific commands,   ");
    ansi.println("5 = Unseal Battery,         ");
    ansi.println("6 = Seal Battery,           ");
    ansi.println("7 = Clear Permanent Failure ");
    ansi.println("8 = Specify Command         Use 8 x for command, x can be name or reg, use 8 x x for range in hex.");
}

void displaySmallmenu() {
    ansi.clearScreen();
    ansi.println("1=Menu, 2=Search, 3=Standard, 4=Extended, 5=Unseal, 6=Seal, 7=Clear PF, 8=Specify Command");
}

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

void display_sbscommands(ArduinoSMBus& battery) {
    uint16_t x, y; // x and y position
    battery.batteryMode(); // We need to get the Battery Mode first to determine output ranges further on
    ansi.print(battery.SBS_COMMAND.RemainingCapacityAlarm.name + " (0x01):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.remainingCapacityAlarm());
    ansi.print(battery.SBS_COMMAND.batterymode.bits.capacity_mode ? " 10mWh" : "mAh");
    ansi.gotoXY(TAB3, y);
    ansi.print(I2Ccode[battery.i2ccode]);

    ansi.print(battery.SBS_COMMAND.RemainingCapacityAlarm.name + " (0x02):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.remainingTimeAlarm());
    ansi.print(" minutes");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print(battery.SBS_COMMAND.BatteryMode.name + " (0x03):"); 
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    printBits(battery.SBS_COMMAND.batterymode.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    ansi.gotoXY(TAB1, y+1);
    ansi.print("Internal Charge Controller:");
    ansi.gotoXY(TAB2, y+1);
    ansi.println(battery.SBS_COMMAND.batterymode.bits.internal_charge_controller ? "Supported" : "Not Supported");
    ansi.gotoXY(TAB1, y+2);
    ansi.print("Primary Battery Support:");
    ansi.gotoXY(TAB2, y+2);
    ansi.println(battery.SBS_COMMAND.batterymode.bits.primary_battery_support ? "Supported" : "Not Supported");
    ansi.gotoXY(TAB1, y+3);
    ansi.print("Condition Flag:");
    ansi.gotoXY(TAB2, y+3);
    ansi.println(battery.SBS_COMMAND.batterymode.bits.condition_flag ? "Cycle Requested" : "Battery OK");
    ansi.gotoXY(TAB1, y+4);
    ansi.print("Internal Charge Controller:");
    ansi.gotoXY(TAB2, y+4);
    ansi.println(battery.SBS_COMMAND.batterymode.bits.charge_controller_enabled ? "Enabled" : "Disabled");
    ansi.gotoXY(TAB1, y+5);
    ansi.print("Primary Battery:");
    ansi.gotoXY(TAB2, y+5);
    ansi.println(battery.SBS_COMMAND.batterymode.bits.primary_battery ? "Operating in primary role" : "Operating in secondary role");
    ansi.gotoXY(TAB1, y+6);
    ansi.print("Alarm Mode:");
    ansi.gotoXY(TAB2, y+6);
    ansi.println(battery.SBS_COMMAND.batterymode.bits.alarm_mode ? "Broadcasts disabled " : "Broadcasts enabled ");
    ansi.gotoXY(TAB1, y+7);
    ansi.print("Charger Mode:");
    ansi.gotoXY(TAB2, y+7);
    ansi.println(battery.SBS_COMMAND.batterymode.bits.charger_mode ? "Broadcasts disabled " : "Broadcasts enabled ");
    ansi.gotoXY(TAB1, y+8);
    ansi.print("Capacity Mode:");
    ansi.gotoXY(TAB2, y+8);
    ansi.println(battery.SBS_COMMAND.batterymode.bits.capacity_mode ? "In 10mW or 10mWh" : "In mA or mAh");

    ansi.print("At Rate (0x04):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.atRate());
    ansi.print(battery.SBS_COMMAND.batterymode.bits.capacity_mode ? "x10mW" : "mA");
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
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.temperature(), 1);
    ansi.print("K, ");
    ansi.print(battery.temperatureC(), 1);
    ansi.print("C, ");
    ansi.print(battery.temperatureF());
    ansi.println("F.");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

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
    ansi.print(battery.SBS_COMMAND.batterymode.bits.capacity_mode ? "x10mWh" : " mAh");
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);

    ansi.print("Full Capacity (0x10):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
    ansi.print(battery.fullCapacity());
    ansi.print(battery.SBS_COMMAND.batterymode.bits.capacity_mode ? "x10mWh" : " mAh");
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
    printBits(battery.SBS_COMMAND.batterystatus.raw);
    ansi.gotoXY(TAB3, y);
    ansi.print(I2Ccode[battery.i2ccode] + " " + errorcodes[battery.SBS_COMMAND.batterystatus.bits.error_codes]);
    ansi.gotoXY(TAB1, y+1);
    ansi.print("Fully Discharged:");
    ansi.gotoXY(TAB2, y+1);
    ansi.print(battery.SBS_COMMAND.batterystatus.bits.fully_discharged ? "True" : "False");
    ansi.gotoXY(TAB1, y+2);
    ansi.print("Fully Charged:");
    ansi.gotoXY(TAB2, y+2);
    ansi.print(battery.SBS_COMMAND.batterystatus.bits.fully_charged ? "True" : "False");
    ansi.gotoXY(TAB1, y+3);
    ansi.print("Discharging:");
    ansi.gotoXY(TAB2, y+3);
    ansi.print(battery.SBS_COMMAND.batterystatus.bits.discharging ? "True" : "False");
    ansi.gotoXY(TAB1, y+4);
    ansi.print("Initialized:");
    ansi.gotoXY(TAB2, y+4);
    ansi.print(battery.SBS_COMMAND.batterystatus.bits.initialized ? "Calibrated" : "Calibrating");
    ansi.gotoXY(TAB1, y+5);
    ansi.print("Remaining Time Alarm:");
    ansi.gotoXY(TAB2, y+5);
    ansi.print(battery.SBS_COMMAND.batterystatus.bits.rem_time_alarm ? "Set" : "Not set");
    ansi.gotoXY(TAB1, y+6);
    ansi.print("Remaining Capacity Alarm:");
    ansi.gotoXY(TAB2, y+6);
    ansi.print(battery.SBS_COMMAND.batterystatus.bits.rem_capacity_alarm ? "Set" : "Not set");
    ansi.gotoXY(TAB1, y+7);
    ansi.print("Terminate Discharge Alarm:");
    ansi.gotoXY(TAB2, y+7);
    ansi.print(battery.SBS_COMMAND.batterystatus.bits.term_discharge_alarm ? "Capacity depleted" : "Discharge not detected");
    ansi.gotoXY(TAB1, y+8);
    ansi.print("Over Temperature Alarm:");
    ansi.gotoXY(TAB2, y+8);
    ansi.print(battery.SBS_COMMAND.batterystatus.bits.over_temp_alarm ? "Above limit" : "Within acceptable range");
    ansi.gotoXY(TAB1, y+9);
    ansi.print("Terminate Charge Alarm:");
    ansi.gotoXY(TAB2, y+9);
    ansi.print(battery.SBS_COMMAND.batterystatus.bits.term_charge_alarm ? "Suspend charging" : "No charging, alarm cleared");
    ansi.gotoXY(TAB1, y+10);    
    ansi.print("Over Charged Alarm:");
    ansi.gotoXY(TAB2, y+10);
    ansi.println(battery.SBS_COMMAND.batterystatus.bits.over_charged_alarm ? "Battery fully charged" : "Cleared");

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
    ansi.print(battery.SBS_COMMAND.batterymode.bits.capacity_mode ? "x10mWh" : " mAh");
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

    // Following functions are not part of the smart battery specification version 1.1
    
    ansi.print("Voltage Cell 1 to 4 (0x3f - 0x3c):");
    ansi.readCursorPosition(x, y);
    ansi.gotoXY(TAB2, y);
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
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
}

void display_bq20z9xx(ArduinoSMBus& battery) {
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print(battery.BQ20Z9xx_COMMAND.DeviceType.name + "(0x00 -> 0x0001):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print("BQ20Z");
  ansi.print(battery.manufacturerAccessType(), HEX);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[battery.i2ccode]);
  
  ansi.print("Firmware version (0x00 -> 0x0002):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  uint16_t version = battery.manufacturerAccessFirmware();
  ansi.print(version >> 8 , DEC);
  ansi.print(".");
  ansi.print((uint8_t)version, DEC);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[battery.i2ccode]);

  ansi.print("Hardware version (0x00 -> 0x0003):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  version = battery.manufacturerAccessHardware();
  ansi.print((uint8_t)version, HEX);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[battery.i2ccode]);

  ansi.print(battery.BQ20Z9xx_COMMAND.ManufacturerStatus.name + " (0x00 -> 0x0006):");
  battery.manufacturerStatus();
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  printBits(battery.BQ20Z9xx_COMMAND.manufacturerstatus.raw);
  ansi.gotoXY(TAB3, y);
  ansi.print(I2Ccode[battery.i2ccode]);
  ansi.gotoXY(TAB1, y+1);
  ansi.print("State: ");
  ansi.gotoXY(TAB2, y+1);
  ansi.print(statuscodes[battery.BQ20Z9xx_COMMAND.manufacturerstatus.bits.state]);
  if (battery.BQ20Z9xx_COMMAND.manufacturerstatus.bits.state == 9) ansi.print(", " + permanentfailurecodes[battery.BQ20Z9xx_COMMAND.manufacturerstatus.bits.pf]);
  ansi.gotoXY(TAB1, y+2);
  ansi.print("FETs:");
  ansi.gotoXY(TAB2, y+2);
  ansi.println(fetcodes[battery.BQ20Z9xx_COMMAND.manufacturerstatus.bits.fet]);

  ansi.print(battery.BQ20Z9xx_COMMAND.ManufacturerData.name + " (0x23):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  battery.manufacturerData();
  battery.batteryStatus();
  for (uint8_t i = 0; i < 15; i++) {
    ansi.printf("%02x", battery.BQ20Z9xx_COMMAND.manufacturerdata.raw[i]);
  }
  ansi.println(" " + I2Ccode[battery.i2ccode]);
  ansi.gotoXY(TAB1, y+1);
  ansi.print("Pack Lot Code:");
  ansi.gotoXY(TAB2, y+1);
  ansi.print(battery.BQ20Z9xx_COMMAND.manufacturerdata.bytes.PackLotCode, HEX);
  ansi.gotoXY(TAB1, y+2);
  ansi.print("PCB Lot Code:");
  ansi.gotoXY(TAB2, y+2);
  ansi.print(battery.BQ20Z9xx_COMMAND.manufacturerdata.bytes.PCBLotCode, HEX);
  ansi.gotoXY(TAB1, y+3);
  ansi.print("Firmware Version:");
  ansi.gotoXY(TAB2, y+3);
  ansi.print(battery.BQ20Z9xx_COMMAND.manufacturerdata.bytes.FirmwareVersion, HEX);
  ansi.gotoXY(TAB1, y+4);
  ansi.print("Hardware Revision:");
  ansi.gotoXY(TAB2, y+4);
  ansi.print(battery.BQ20Z9xx_COMMAND.manufacturerdata.bytes.HardwareRevision, HEX);
  ansi.gotoXY(TAB1, y+5);
  ansi.print("Cell Revision:");
  ansi.gotoXY(TAB2, y+5);
  ansi.print(battery.BQ20Z9xx_COMMAND.manufacturerdata.bytes.CellRevision, HEX);
  ansi.gotoXY(TAB1, y+6);
  ansi.print("Partial Reset Counter:");
  ansi.gotoXY(TAB2, y+6);
  ansi.print(battery.BQ20Z9xx_COMMAND.manufacturerdata.bytes.PartialResetCounter, HEX);
  ansi.gotoXY(TAB1, y+7);
  ansi.print("Full Reset Counter:");
  ansi.gotoXY(TAB2, y+7);
  ansi.print(battery.BQ20Z9xx_COMMAND.manufacturerdata.bytes.FullResetCounter, HEX);
  ansi.gotoXY(TAB1, y+8);
  ansi.print("Watchdog Reset Counter:");
  ansi.gotoXY(TAB2, y+8);
  ansi.print(battery.BQ20Z9xx_COMMAND.manufacturerdata.bytes.WatchdogResetCounter, HEX);
  ansi.gotoXY(TAB1, y+9);
  ansi.print("Check Sum:");
  ansi.gotoXY(TAB2, y+9);
  ansi.println(battery.BQ20Z9xx_COMMAND.manufacturerdata.bytes.CheckSum, HEX);
  ansi.gotoXY(TAB1, y+10);
  ansi.print("String Length:");
  ansi.gotoXY(TAB2, y+10);
  ansi.println(battery.BQ20Z9xx_COMMAND.manufacturerdata.bytes.Length, DEC);

  ansi.print("FETControl (0x46):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  battery.FETControl();
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(battery.BQ20Z9xx_COMMAND.fetcontrol.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    ansi.gotoXY(TAB1, y+1);
    ansi.print("Charge FET:");
    ansi.gotoXY(TAB2, y+1);
    ansi.println(battery.BQ20Z9xx_COMMAND.fetcontrol.bits.chg?"On":"Off");
    ansi.gotoXY(TAB1, y+2);
    ansi.print("DisCharge FET:");
    ansi.gotoXY(TAB2, y+2);
    ansi.println(battery.BQ20Z9xx_COMMAND.fetcontrol.bits.dsg?"On":"Off");
  }

  ansi.print("State Of Health (0x4f):");
  ansi.readCursorPosition(x, y);
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
  battery.Safetyalert();
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(battery.BQ20Z9xx_COMMAND.safetyalert.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
  }

  ansi.print("Safety Status (0x51):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  battery.Safetystatus();
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(battery.BQ20Z9xx_COMMAND.safetystatus.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
  }

  ansi.print("PF Alert (0x52):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  battery.PFalert();
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(battery.BQ20Z9xx_COMMAND.pfalert.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
  }

  ansi.print("PF Status (0x53):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  battery.PFstatus();
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(battery.BQ20Z9xx_COMMAND.pfstatus.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
  }

  ansi.print("Operation Status (0x54):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  battery.Operationstatus();
  if (battery.i2ccode) {
    ansi.print(I2Ccode[battery.i2ccode]);
    ansi.println(", is device unsealed ?");
  } else { 
    printBits(battery.BQ20Z9xx_COMMAND.operationstatus.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
  }
}

void displaySealstatus(ArduinoSMBus& battery) {
  battery.Operationstatus();
  ansi.print(I2Ccode[battery.i2ccode]);
  ansi.println(" " + errorcodes[battery.baterrorcode]);
  ansi.println(battery.BQ20Z9xx_COMMAND.operationstatus.bits.ss?"sealed mode enabled":"sealed mode disabled");
  ansi.println(battery.BQ20Z9xx_COMMAND.operationstatus.bits.fas?"full access mode enabled":"full access mode disabled");
}

void displayBatteryStatus(ArduinoSMBus& battery) {
    ansi.print("Battery Status (0x16):");
    uint16_t x, y;
    ansi.readCursorPosition(x, y);
    battery.batteryStatus();
    ansi.gotoXY(TAB2, y);
    printBits(battery.SBS_COMMAND.batterystatus.raw);
    ansi.gotoXY(TAB3, y);
    ansi.println(I2Ccode[battery.i2ccode]);
    ansi.println(" " + errorcodes[battery.baterrorcode]);
}

// prints 8-bit integer in this form: 0000 0000
void printBits(uint8_t n) {
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
void printBits(uint16_t n) {
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