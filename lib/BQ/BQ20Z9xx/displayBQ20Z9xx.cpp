#include <bitset>
#include "displayBQ209xx.h"

DisplayBQ20Z9xx::DisplayBQ20Z9xx(uint8_t address): bq20z9xx(address), Display(address) {
  // list of the different commands, including function pointers to these funtions. This to be able to call them via user input
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
    info.emplace_back(&Display::displaymanufacturerAccess, DEVICEINFO, "ManufacturerAccess");
    // Specific BQ20Z9xx functions
    info.emplace_back(&displaymanufacturerAccessType, DEVICEINFO, "manufacturerAccessType");

}

void DisplayBQ20Z9xx::setup(uint8_t address) {
  batteryAddress = address;
}

void DisplayBQ20Z9xx::loop() {

}

// specific BQ20Zxx commands
void DisplayBQ20Z9xx::displaymanufacturerAccessType() { // command 0x00 0x0001
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessType (0x00->0x0001):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  ansi.print("BQ20Z");
  manufacturerAccessType();
  ansi.print(smbus::text);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void DisplayBQ20Z9xx::displayCommandNames(){
    for (const auto& it : info) {
      Serial.println(it.name); 
    }
}
