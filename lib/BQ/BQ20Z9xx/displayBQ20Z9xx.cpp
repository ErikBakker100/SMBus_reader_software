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
    info.emplace_back(&displayoptionalMFGfunctions, USAGEINFO, "optionalMFGfunction_1-4");
    info.emplace_back(&displaymanufacturerAccessType, DEVICEINFO, "manufacturerAccessType");
    info.emplace_back(&displaymanufacturerAccessFirmware, DEVICEINFO, "manufacturerAccessFirmware");
    info.emplace_back(&displaymanufacturerAccessHardware, DEVICEINFO, "manufacturerAccessHardware");
    info.emplace_back(&displaymanufacturerAccessStatus, STATUSBITS, "manufacturerAccessStatus");
    info.emplace_back(&displaymanufacturerAccessChemistryID, DEVICEINFO, "manufacturerAccessChemistryID");
    info.emplace_back(&displaymanufacturerAccessShutdown, SET, "manufacturerAccessShutdown"); // Instructs the bq20z90/bq20z95 to verify and enter shutdown mode.
    info.emplace_back(&displaymanufacturerAccessSleep, SET, "manufacturerAccessSleep"); // Instructs the bq20z90/bq20z95 to verify and enter sleep mode if no other command is sent after the Sleep command.
    info.emplace_back(&displaymanufacturerAccessSeal, SET, "manufacturerAccessSeal");
    info.emplace_back(&displaymanufacturerAccessPermanentFailClear, SET, "manufacturerAccessPermanentFailClear");
    info.emplace_back(&displaymanufacturerAccessUnseal, SET, "manufacturerAccessUnseal");
    info.emplace_back(&displaymanufacturerAccessFullAccess, SET, "manufacturerAccessFullAccess");
    info.emplace_back(&displaymanufacturerData, DEVICEINFO, "manufacturerData");
    info.emplace_back(&displayfetControl, SET, "fetControl");
    info.emplace_back(&displaystateOfHealth, STATUSBITS, "stateOfHealth");
    info.emplace_back(&displaysafetyAlert, DEVICEINFO, "safetyAlert");
    info.emplace_back(&displaysafetyStatus, STATUSBITS, "safetyStatus");
    info.emplace_back(&displaypfAlert, DEVICEINFO, "pfAlert");
    info.emplace_back(&displaypfStatus, STATUSBITS, "pfStatus");
    info.emplace_back(&displayoperationStatus, DEVICEINFO, "operationStatus");
    info.emplace_back(&displayunsealKey, DEVICEINFO, "unsealKey");
}

void DisplayBQ20Z9xx::setup(uint8_t address) {
  batteryAddress = address;
}

void DisplayBQ20Z9xx::loop() {

}

// Following functions are not part of the smart battery specification version 1.1
void DisplayBQ20Z9xx::displayoptionalMFGfunctions() {
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

void DisplayBQ20Z9xx::displaymanufacturerAccessFirmware() {   // command 0x00
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Firmware version (0x00->0x0002):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  manufacturerAccessFirmware();
  ansi.print(smbus::text);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void DisplayBQ20Z9xx::displaymanufacturerAccessHardware() {   // command 0x00
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("Hardware version (0x00->0x0003):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  manufacturerAccessHardware();
  ansi.print(smbus::text);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void DisplayBQ20Z9xx::displaymanufacturerAccessStatus() {     // command 0x00
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

void DisplayBQ20Z9xx::displaymanufacturerAccessChemistryID() { // command 0x00 0x0008
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessChemistryID (0x00->0x0008):");
  ansi.print(" ");
  manufacturerAccessChemistryID();
  ansi.print(smbus::text);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void DisplayBQ20Z9xx::displaymanufacturerAccessShutdown(){// command 0x00 0x0010
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessShutdown (0x00->0x0010):");
  manufacturerAccessShutdown();
  ansi.gotoXY(TAB2, y);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void DisplaBQ20Z9xx::displaymanufacturerAccessSleep(){// command 0x00 0x0011
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessSleep (0x00->0x0011):");
  manufacturerAccessSleep();
  ansi.gotoXY(TAB2, y);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
}

void DisplayBQ20Z9xx::displaymanufacturerAccessSeal() {       // command 0x00 0x0020
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("manufacturerAccessSeal (0x00->0x0020):");
  manufacturerAccessSeal();
  ansi.gotoXY(TAB2, y);
  ansi.gotoXY(TAB3, y);
  ansi.println(I2Ccode[i2ccode]);
  displaySealstatus();
}

void DisplayBQ20Z9xx::displaymanufacturerAccessPermanentFailClear(uint16_t key_a, uint16_t key_b){
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

void DisplayBQ20Z9xx::displaymanufacturerAccessUnseal(uint16_t key_a, uint16_t key_b){
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

void DisplayBQ20Z9xx::displaymanufacturerAccessFullAccess(uint16_t key_a, uint16_t key_b){
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

void DisplayBQ20Z9xx::displaymanufacturerData() {             // command 0x23
  uint16_t x, y; // x and y position
  ansi.readCursorPosition(x, y);
  ansi.print("ManufacturerData (0x23):");
  ansi.readCursorPosition(x, y);
  ansi.gotoXY(TAB2, y);
  manufacturerData();
  batteryStatus();
  for (uint8_t i = 0; i < 15; i++) {
    ansi.printf("%02x", text[i]);
  }
  ansi.println(" " + I2Ccode[i2ccode]);
}

void DisplayBQ20Z9xx::displayfetControl(){
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

void DisplayBQ20Z9xx::displaystateOfHealth() {                // command 0x4f
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

void DisplayBQ20Z9xx::displaysafetyAlert() {                  // command 0x50
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

void DisplayBQ20Z9xx::displaysafetyStatus() {                 // command 0x51
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

void DisplayBQ20Z9xx::displaypfAlert() {
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

void DisplayBQ20Z9xx::displaypfStatus() {
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

void DisplayBQ20Z9xx::displayoperationStatus() {              // command 0x54
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

void DisplayBQ20Z9xx::displayunsealKey(){                     // command 0x60
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

bool DisplayBQ20Z9xx::testkey(uint16_t key) {
    writeRegister(MANUFACTURERACCESS, key);
/*  switch (com) {
    case 1:
      displaymanufacturerAccessUnseal(key_a, key_b);
      if (!displaySealstatus()) test = true;
      break;
    case 2:
      displaymanufacturerAccessPermanentFailClear(key_a, key_b);
      if (!displaySealstatus()) test = true;
      break;
    case 3:
      displaymanufacturerAccessFullAccess(key_a, key_b);
      if (!displaySealstatus()) test = true;
      break;
    default:
      return false;
  } */
  return !i2ccode;
}

// returns true if sealed, false otherwise
bool DisplayBQ20Z9xx::displaySealstatus() {
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

// Call a specific function by name
void DisplayBQ20Z9xx::displayByName(const String& functionName) {
  
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
void DisplayBQ20Z9xx::displayByClassifier(uint8_t type) {
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

void DisplayBQ20Z9xx::displayCommandNames(){
    for (const auto& it : info) {
      Serial.println(it.name); 
    }
}
