/**
 * @file SBSCommands.h
 * @author 
 * @brief 
 * @version 1.0
 * @date 12-2024
 *
 * @copyright
 *
 */
#pragma once
#include <Arduino.h>
#include "SBSCommands.h"
#include <bit>

struct {
    struct command{
    String name;
    uint8_t reg;
    uint16_t system_data;
  };
  command DeviceType {"DeviceType()", SBS_COMMAND.ManufacturerAccess.reg, 0x0001};
  command FirmwareVersion {"FirmwareVersion()", SBS_COMMAND.ManufacturerAccess.reg, 0x0002};
  command HardwareVersion {"HardwareVersion()", SBS_COMMAND.ManufacturerAccess.reg, 0x0003};
  command ManufacturerStatus {"ManufacturerStatus()", SBS_COMMAND.ManufacturerAccess.reg, 0x0006};
  command ChemistryID {"ChemistryID()", SBS_COMMAND.ManufacturerAccess.reg, 0x0008};
  /**> The commands in this section cause the bq20z90/bq20z95 to take actions when written. No data is returned.*/
  command Shutdown {"Shutdown()", SBS_COMMAND.ManufacturerAccess.reg, 0x0010}; /**> Instructs the bq20z90/bq20z95 to verify and enter shutdown mode.*/
  command Sleep {"Sleep()", SBS_COMMAND.ManufacturerAccess.reg, 0x0011}; /**> Instructs the bq20z90/bq20z95 to verify and enter sleep mode if no other command is sent after the Sleep command.*/
  command SealDevice {"SealDevice()", SBS_COMMAND.ManufacturerAccess.reg, 0x0020}; /**> Instructs the bq20z90/bq20z95 to verify and enter sleep mode if no other command is sent after the Sleep command.*/
  command PermanentFailClear {"PermanentFailClear(PFKey)", SBS_COMMAND.ManufacturerAccess.reg}; /**> This 2 step command needs to be written to ManufacturerAccess in following order: 1st word of the PFKey
  first followed by the 2nd word of the PFKey. If the command fails 4 seconds must pass before the command can be reissued. It instructs the bq20z90/bq20z95 to clear
  the PFStatus, clear the [PF] flag, clear the Fuse Flag, reset the SAFE pin, and unlock the data flash for writes.*/
  command UnsealDevice {"UnsealDevice()", SBS_COMMAND.ManufacturerAccess.reg}; /**> Instructs the bq20z90/bq20z95 to enable access to the SBS functions and data flash space and clear the [SS] flag.
  This 2 step command needs to be written to ManufacturerAccess in the following order: 1st word of the UnSealKey first followed by the 2nd word of the UnSealKey.
  If the command fails 4 seconds must pass before the command can be reissued.*/
  command FullAccessDevice {"FullAccessDevice()", SBS_COMMAND.ManufacturerAccess.reg, }; /**> Instructs the bq20z90/bq20z95 to enable full access to all SBS functions and data flash space and set the
  [FAS] flag. This 2 step command needs to be written to ManufacturerAccess in the following order: 1st word of the FullAccessKey first followed by the 2nd word of the FullAccessKey.*/
  command FETControl {"FETControl()", 0x46}; /**> This read- or write-word function allows direct control of the FETs for test purposes.*/
  command StateOfHealth {"StateOfHealth()", 0x4f}; /**> This read word function returns the state of health of the battery in %.*/
  command SafetyAlert {"SafetyAlert()", 0x50}; /**> This read-word function returns indications of pending safety issues, such as running safety timers, or fail counters that are 
  nonzero but have not reached the required time or value to trigger a SafetyStatus failure.*/
  command SafetyStatus {"SafetyStatus()", 0x51}; /**> This read word function returns the status of the 1st level safety features.*/
  command PFAlert {"PFAlert()", 0x52}; /**> This read-word function returns indications of pending safety issues, such as running safety timers that have not reached the required 
  time to trigger a PFAlert failure.*/
  command PFStatus {"PFStatus()", 0x53}; /**> The permanent failure status register indicates the source of the bq20z90/bq20z95 permanent-failure condition.*/
  command OperationStatus {"OperationStatus()", 0x54}; /**> This read-word function returns the current operation status of the bq20z90/bq20z95.*/
  command UnSealKey {"UnSealKey()", 0x60}; /**> This read- or write-block command allows the user to change the Unseal key for the Sealed-to-Unsealed security-state transition.
  This function is only available when the bq20z90/bq20z95 is in the Full-Access mode*/
}BQ20Z9xx_COMMAND;

/**
 * @struct ManufacturerBatStatus
 * @brief A struct to hold flags indicating the battery state generated by writing 0x0006 to the Manufacturer Access Register.
 */
struct ManufacturerStatus {
  uint16_t raw;                 /**< Data Read. */
  String failure {""};
  String permfailure {""};
  bool chg_fet;                 /**< On if 1 (True), Off if 0 (False). */
  bool dsg_fet;                 /**< On if 1 (True), Off if 0 (False). */
};

/**
 * @struct SafetyAlert
 * @brief A struct to hold the flags of teh pending safety issues register.
 */
struct SafetyAlert {
  uint16_t raw;                 /**< Data read */
  bool scd;                     /**< Discharge short-circuit alert. */
  bool scc;                     /**< Charge short-circuit alert. */
  bool aocd;                    /**< AFE discharge overcurrent alert. */
  bool wdf;                     /**< AFE watchdog alert. */  
  bool hwdg;                    /**< Host watchdog alert. */
  bool pf;                      /**< Permanent failure alert. */
  bool cov;                     /**< Cell overvoltage alert. */
  bool cuv;                     /**< Cell undervoltage alert. */
  bool pov;                     /**< Pack overvoltage alert. */
  bool puv;                     /**< Pack undervoltage alert. */
  bool occ2;                    /**< Tier-2 charge overcurrent alert. */
  bool ocd2;                    /**< Tier-2 discharge overcurrent alert. */
  bool occ;                     /**< Charge overcurrent alert. */
  bool ocd;                     /**< Discharge overcurrent alert. */
  bool otc;                     /**< Charge overtemperature alert. */
  bool otd;                     /**< Discharge overtemperature alert. */
};

/**
 * @struct SafetyStatus
 * @brief A struct to hold the flags of the 1st level safety features register.
 */
struct SafetyStatus {
  uint16_t raw;                 /**< Data read */
  bool scd;                     /**< Discharge short-circuit condition. */
  bool scc;                     /**< Charge short-circuit condition. */
  bool aocd;                    /**< AFE discharge overcurrent condition. */
  bool wdf;                     /**< AFE watchdog condition. */  
  bool hwdg;                    /**< Host watchdog condition. */
  bool pf;                      /**< Permanent failure and SAFE pin has been driven high.. */
  bool cov;                     /**< Cell overvoltage condition. */
  bool cuv;                     /**< Cell undervoltage condition. */
  bool pov;                     /**< Pack overvoltage condition. */
  bool puv;                     /**< Pack undervoltage condition. */
  bool occ2;                    /**< Tier-2 charge overcurrent condition. */
  bool ocd2;                    /**< Tier-2 discharge overcurrent condition. */
  bool occ;                     /**< Charge overcurrent condition. */
  bool ocd;                     /**< Discharge overcurrent condition. */
  bool otc;                     /**< Charge overtemperature condition. */
  bool otd;                     /**< Discharge overtemperature condition. */
};

/**
 * @struct PFAlert
 * @brief A struct to hold the flags of the pending safety issues register.
 * The permanent failure status register indicates the source of the bq20z90/bq20z95 permanent-failure condition.
 */
struct PFAlert {
  uint16_t raw;                 /**< Data read */
  bool pfin;                    /**< External Input Indication of permanent failure alert. */
  bool sov;                     /**< Safety-Overvoltage permanent failure alert. */
  bool sotc;                    /**< Charge Safety Overtemperature permanent failure alert. */
  bool sotd;                    /**< Discharge Safety Overtemperature permanent failure alert. */  
  bool cim;                     /**< Cell-Imbalance permanent failure alert. */
  bool cfetf;                   /**< Charge-FET-Failure permanent failure alert. */
  bool dfetf;                   /**< Discharge-FET-Failure permanent failure alert. */
  bool dff;                     /**< Data Flash Fault permanent failure alert. */
  bool ace_c;                   /**< Permanent AFE Communications failure alert. */
  bool afe_p;                   /**< Periodic AFE Communications permanent failure alert. */
  bool socc;                    /**< Charge Safety-Overcurrent permanent failure alert. */
  bool socd;                    /**< Discharge Safety Overcurrent permanent failure alert. */
  bool sopt;                    /**< Open Thermistor permanent failure alert. */
  bool fbf;                     /**< Fuse Blow Failure alert. */
};

/**
 * @struct PFStatus
 * @brief A struct to hold the permanent failure status register flags.
 * The permanent failure status register indicates the source of the bq20z90/bq20z95 permanent-failure condition.
 */
struct PFStatus {
  uint16_t raw;                 /**< Data read */
  bool pfin;                    /**< External Input Indication of permanent failure. */
  bool sov;                     /**< Safety-Overvoltage permanent failure. */
  bool sotc;                    /**< Charge Safety Overtemperature permanent failure. */
  bool sotd;                    /**< Discharge Safety Overtemperature permanent failure. */  
  bool cim;                     /**< Cell-Imbalance permanent failure. */
  bool cfetf;                   /**< Charge-FET-Failure permanent failure. */
  bool dfetf;                   /**< Discharge-FET-Failure permanent failure. */
  bool dff;                     /**< Data Flash Fault permanent failure. */
  bool afe_c;                   /**< Permanent AFE Communications failure. */
  bool afe_p;                   /**< Periodic AFE Communications permanent failure. */
  bool socc;                    /**< Charge Safety-Overcurrent permanent failure. */
  bool socd;                    /**< Discharge Safety Overcurrent permanent failure. */
  bool sopt;                    /**< Open Thermistor permanent failure. */
  bool fbf;                     /**< Fuse Blow Failure. */
};

/**
 * @struct OperationStatus
 * @brief A struct to hold the operation status flags.
 *
 * This struct holds various flags that represent the operation status.
 */
struct OperationStatus {
  uint16_t raw;                 /**< Data read */
  bool pres;                    /**< Low indicating that the system is present (battery inserted). */
  bool fas;                     /**< Low means full access security mode. */
  bool ss;                      /**< True  is Sealed security mode. */    
  bool csv;                     /**< Data Flash checksum value has been generated. */   
  bool ldmd;                    /**< Load mode for Impedance Track modeling. 0 = constant current, 1 = constant power. */      
  bool wake;                    /**< bq20z90/bq20z95 WAKE mode . */          
  bool dsg;                     /**< Replica of the SBS:BatteryStatus(0x16)[DSG] flag.. */ 
  bool xdsg;                    /**< Discharge fault. */
  bool xdsgi;                   /**< Discharge disabled due to a current issue. */
  bool r_dis;                   /**< Ra Table resistance updates are disabled. */
  bool vok;                     /**< Voltages are OK for a QMAX update. */
  bool qen;                     /**< QMAX updates are enabled. */
};

/**
 * @struct FETcontrol
 * @brief This read- or write-word function allows direct control of the FETs for test purposes.
 */
struct FETcontrol {
  uint8_t raw;
  bool od;
  bool zvchg;
  bool chg;                     /**< Charge FET Control, 0 = turn OFF CHG FET. CHG FET doesn't turn off in discharge mode to protect the FET body diode. 1 = turn ON CHG FET */
  bool dsg;                     /**< Discharge FET control 0 = turn OFF DSG FET. DSG FET doesn't turn of in charge mode to protect the FET body diode. 1 = turn ON DSG FET */
};
