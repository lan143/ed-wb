#pragma once

#include <cmath>
#include <cstdint>
#include <string>
#include <utility>

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    // Inverter work mode.
    enum ANENJIWorkMode : uint16_t
    {
        PowerOn = 0,
        Standby = 1,
        Mains = 2,
        OffGrid = 3,
        Bypass = 4,
        Charging = 5,
        FaultMode = 6
    };

    // Output topology.
    enum ANENJIOutputMode : uint16_t
    {
        Single = 0,
        Parallel = 1,
        ThreePhaseP1 = 2,
        ThreePhaseP2 = 3,
        ThreePhaseP3 = 4
    };

    // Output source priority.
    enum ANENJIOutputPriority : uint16_t
    {
        UTI = 0, // Mains-PV-Battery
        SOL = 1, // PV-Mains-Battery (inverter priority)
        SBU = 2, // PV-Battery-Mains
        SUB = 3  // PV-Mains-Battery (mains priority)
    };

    // AC input voltage range.
    enum ANENJIInputVoltageRange : uint16_t
    {
        APL = 0,
        UPS = 1
    };

    // Buzzer behavior.
    enum ANENJIBuzzerMode : uint16_t
    {
        Mute = 0,
        SourceChangeAndFaults = 1,
        FaultsAndWarnings = 2,
        FaultsOnly = 3
    };

    // Battery chemistry preset.
    enum ANENJIBatteryType : uint16_t
    {
        AGM = 0,
        FLD = 1,
        USER = 2,
        Li1 = 3,
        Li2 = 4,
        Li3 = 5,
        Li4 = 6
    };

    // Battery charge source priority.
    enum ANENJIBatteryChargePriority : uint16_t
    {
        MainsPreferred = 0,
        PvPreferred = 1,
        PvEqualMains = 2,
        PvOnly = 3
    };

    // Boot behavior.
    enum ANENJIBootMode : uint16_t
    {
        LocalOrRemote = 0,
        LocalOnly = 1,
        RemoteOnly = 2
    };

    // ANENJI 4200W hybrid inverter, Modbus RTU.
    // Only function 0x03 (holding register read) and 0x10 (holding register write) are used.
    class ANENJI4200 : public Device
    {
    public:
        ANENJI4200(ModbusClient& client) : Device(client) {}

        // Identity and information.

        // Register 171 (UInt). Device type code.
        std::pair<uint16_t, bool> getDeviceType() const;
        // Register 172, 12 ASCII registers. Device name.
        std::pair<std::string, bool> getDeviceName() const;
        // Register 172, 12 ASCII registers. Device name, truncated to 24 characters.
        bool setDeviceName(std::string name);
        // Register 184 (UInt). Protocol number, 1 = valid protocol.
        std::pair<uint16_t, bool> getProtocolNumber() const;
        // Register 186, 12 ASCII registers. Serial number.
        std::pair<std::string, bool> getSerialNumber() const;
        // Register 626, 8 ASCII registers. Program version.
        std::pair<std::string, bool> getProgramVersion() const;
        // Register 643 (UInt). Rated power, W.
        std::pair<uint16_t, bool> getRatedPower() const;
        // Register 644 (UInt). Rated cell count, cells per pack.
        std::pair<uint16_t, bool> getRatedCellCount() const;

        // Status and telemetry.

        // Register 100 (ULong). Fault code bit field, bit per fault table entry.
        std::pair<uint32_t, bool> getFaultCode() const;
        // Register 104 (ULong). Unmasked warning code bit field.
        std::pair<uint32_t, bool> getWarningCode() const;
        // Register 108 (ULong). Masked warning code bit field.
        std::pair<uint32_t, bool> getMaskedWarningCode() const;
        // Register 108 (ULong). Masked warning code bit field.
        bool setMaskedWarningCode(uint32_t code);
        // Register 314 (ULong). Warning mask, bit = 1 shown, bit = 0 masked.
        std::pair<uint32_t, bool> getWarningMask() const;
        // Register 314 (ULong). Warning mask, bit = 1 shown, bit = 0 masked.
        bool setWarningMask(uint32_t mask);
        // Register 201 (UInt). Work mode.
        std::pair<ANENJIWorkMode, bool> getWorkMode() const;
        // Register 231 (UInt). Power flow status bit field.
        // Bits 0-1 PV, 2-3 mains, 4-5 battery charge/discharge, 6-7 output,
        // 8 mains charging, 9 PV charging, 10-13 icon-off flags.
        std::pair<uint16_t, bool> getPowerFlowStatus() const;
        // Register 202 (Int). Mains voltage, 0.1 V.
        std::pair<float_t, bool> getMainsVoltage() const;
        // Register 203 (Int). Mains frequency, 0.01 Hz.
        std::pair<float_t, bool> getMainsFrequency() const;
        // Register 204 (Int). Mains power, W.
        std::pair<int16_t, bool> getMainsPower() const;
        // Register 205 (Int). Inverter voltage, 0.1 V.
        std::pair<float_t, bool> getInverterVoltage() const;
        // Register 206 (Int). Inverter current, 0.1 A, signed.
        std::pair<float_t, bool> getInverterCurrent() const;
        // Register 207 (Int). Inverter frequency, 0.01 Hz.
        std::pair<float_t, bool> getInverterFrequency() const;
        // Register 208 (Int). Inverter power, W, positive = output.
        std::pair<int16_t, bool> getInverterPower() const;
        // Register 209 (Int). Inverter charging power, W.
        std::pair<int16_t, bool> getInverterChargingPower() const;
        // Register 210 (Int). Output voltage, 0.1 V.
        std::pair<float_t, bool> getOutputVoltage() const;
        // Register 211 (Int). Output current, 0.1 A, signed.
        std::pair<float_t, bool> getOutputCurrent() const;
        // Register 212 (Int). Output frequency, 0.01 Hz.
        std::pair<float_t, bool> getOutputFrequency() const;
        // Register 213 (Int). Output active power, W.
        std::pair<int16_t, bool> getOutputActivePower() const;
        // Register 214 (Int). Output apparent power, VA.
        std::pair<int16_t, bool> getOutputApparentPower() const;
        // Register 215 (Int). Battery voltage, 0.1 V.
        std::pair<float_t, bool> getBatteryVoltage() const;
        // Register 216 (Int). Battery current, 0.1 A, signed.
        std::pair<float_t, bool> getBatteryCurrent() const;
        // Register 217 (Int). Battery power, W.
        std::pair<int16_t, bool> getBatteryPower() const;
        // Register 219 (Int). PV voltage, 0.1 V.
        std::pair<float_t, bool> getPvVoltage() const;
        // Register 220 (Int). PV current, 0.1 A.
        std::pair<float_t, bool> getPvCurrent() const;
        // Register 223 (Int). PV power, W.
        std::pair<int16_t, bool> getPvPower() const;
        // Register 224 (Int). PV charging power, W.
        std::pair<int16_t, bool> getPvChargingPower() const;
        // Register 225 (Int). Load percentage, %.
        std::pair<uint8_t, bool> getLoadPercent() const;
        // Register 226 (Int). DC-DC temperature, C.
        std::pair<int16_t, bool> getDcdcTemperature() const;
        // Register 227 (Int). Inverter temperature, C.
        std::pair<int16_t, bool> getInverterTemperature() const;
        // Register 229 (UInt). Battery percentage, %.
        std::pair<uint8_t, bool> getBatteryPercent() const;
        // Register 232 (Int). Filtered battery current, 0.1 A, signed (+ charging, - discharging).
        std::pair<float_t, bool> getBatteryCurrentFiltered() const;
        // Register 233 (Int). Inverter charging current, 0.1 A.
        std::pair<float_t, bool> getInverterChargingCurrent() const;
        // Register 234 (Int). PV charging current, 0.1 A.
        std::pair<float_t, bool> getPvChargingCurrent() const;

        // Fault records.

        // Register 700 (ULong). Upper 16 bits = latest record position, lower 16 bits = total records.
        std::pair<uint32_t, bool> getFaultRecordStorage() const;
        // Register 702 (UInt). Fault record query index, 0..total records.
        std::pair<uint16_t, bool> getFaultQueryIndex() const;
        // Register 702 (UInt). Fault record query index, 0..total records.
        bool setFaultQueryIndex(uint16_t index);

        // Settings.

        // Register 300 (UInt). Output mode.
        std::pair<ANENJIOutputMode, bool> getOutputMode() const;
        // Register 300 (UInt). Output mode.
        bool setOutputMode(ANENJIOutputMode mode);
        // Register 301 (UInt). Output source priority.
        std::pair<ANENJIOutputPriority, bool> getOutputPriority() const;
        // Register 301 (UInt). Output source priority.
        bool setOutputPriority(ANENJIOutputPriority priority);
        // Register 302 (UInt). AC input voltage range.
        std::pair<ANENJIInputVoltageRange, bool> getInputVoltageRange() const;
        // Register 302 (UInt). AC input voltage range.
        bool setInputVoltageRange(ANENJIInputVoltageRange range);
        // Register 303 (UInt). Buzzer behavior.
        std::pair<ANENJIBuzzerMode, bool> getBuzzerMode() const;
        // Register 303 (UInt). Buzzer behavior.
        bool setBuzzerMode(ANENJIBuzzerMode mode);
        // Register 305 (Bool). LCD backlight, true = always on, false = timed.
        std::pair<bool, bool> getLcdBacklight() const;
        // Register 305 (Bool). LCD backlight, true = always on, false = timed.
        bool setLcdBacklight(bool enabled);
        // Register 306 (Bool). LCD auto return home, true = return after 1 minute.
        std::pair<bool, bool> getLcdAutoReturnHome() const;
        // Register 306 (Bool). LCD auto return home, true = return after 1 minute.
        bool setLcdAutoReturnHome(bool enabled);
        // Register 307 (Bool). Energy saving mode.
        std::pair<bool, bool> getEnergySavingMode() const;
        // Register 307 (Bool). Energy saving mode.
        bool setEnergySavingMode(bool enabled);
        // Register 308 (Bool). Overload auto restart.
        std::pair<bool, bool> getOverloadAutoRestart() const;
        // Register 308 (Bool). Overload auto restart.
        bool setOverloadAutoRestart(bool enabled);
        // Register 309 (Bool). Over-temperature auto restart.
        std::pair<bool, bool> getOverTemperatureAutoRestart() const;
        // Register 309 (Bool). Over-temperature auto restart.
        bool setOverTemperatureAutoRestart(bool enabled);
        // Register 310 (Bool). Overload bypass enable.
        std::pair<bool, bool> getOverloadBypassEnable() const;
        // Register 310 (Bool). Overload bypass enable.
        bool setOverloadBypassEnable(bool enabled);
        // Register 313 (Bool). Battery equalization mode enable.
        std::pair<bool, bool> getBatteryEqualizationModeEnable() const;
        // Register 313 (Bool). Battery equalization mode enable.
        bool setBatteryEqualizationModeEnable(bool enabled);
        // Register 316 (Bool). Dry contact, true = grounding box mode.
        std::pair<bool, bool> getDryContact() const;
        // Register 316 (Bool). Dry contact, true = grounding box mode.
        bool setDryContact(bool enabled);
        // Register 320 (UInt). Target output voltage, 0.1 V, typical 220/230/240.
        std::pair<float_t, bool> getTargetOutputVoltage() const;
        // Register 320 (UInt). Target output voltage, 0.1 V, typical 220/230/240.
        bool setTargetOutputVoltage(float voltage);
        // Register 321 (UInt). Target output frequency, 0.01 Hz, typical 50/60.
        std::pair<float_t, bool> getTargetOutputFrequency() const;
        // Register 321 (UInt). Target output frequency, 0.01 Hz, typical 50/60.
        bool setTargetOutputFrequency(float frequency);
        // Register 322 (UInt). Battery chemistry preset.
        std::pair<ANENJIBatteryType, bool> getBatteryType() const;
        // Register 322 (UInt). Battery chemistry preset.
        bool setBatteryType(ANENJIBatteryType type);
        // Register 323 (UInt). Battery overvoltage protection point (A), 0.1 V, max 16.5 V * J.
        std::pair<float_t, bool> getBatteryOvervoltageProtectionPoint() const;
        // Register 323 (UInt). Battery overvoltage protection point (A), 0.1 V, max 16.5 V * J.
        bool setBatteryOvervoltageProtectionPoint(float voltage);
        // Register 324 (UInt). Max charge voltage (B), 0.1 V, range C..A-1 V.
        std::pair<float_t, bool> getMaxChargeVoltage() const;
        // Register 324 (UInt). Max charge voltage (B), 0.1 V, range C..A-1 V.
        bool setMaxChargeVoltage(float voltage);
        // Register 325 (UInt). Floating charge voltage (C), 0.1 V.
        std::pair<float_t, bool> getFloatingChargeVoltage() const;
        // Register 325 (UInt). Floating charge voltage (C), 0.1 V.
        bool setFloatingChargeVoltage(float voltage);
        // Register 326 (UInt). Mains discharge recovery point (D), 0.1 V, 0 = full recovery.
        std::pair<float_t, bool> getMainsDischargeRecoveryPoint() const;
        // Register 326 (UInt). Mains discharge recovery point (D), 0.1 V, 0 = full recovery.
        bool setMainsDischargeRecoveryPoint(float voltage);
        // Register 327 (UInt). Mains mode battery low voltage protection (E), 0.1 V.
        std::pair<float_t, bool> getMainsModeBatteryLowVoltageProtection() const;
        // Register 327 (UInt). Mains mode battery low voltage protection (E), 0.1 V.
        bool setMainsModeBatteryLowVoltageProtection(float voltage);
        // Register 329 (UInt). Off-grid mode battery low voltage protection (F), 0.1 V.
        std::pair<float_t, bool> getOffGridModeBatteryLowVoltageProtection() const;
        // Register 329 (UInt). Off-grid mode battery low voltage protection (F), 0.1 V.
        bool setOffGridModeBatteryLowVoltageProtection(float voltage);
        // Register 330 (UInt). CV to float charge wait time, minutes, 1..900 (0 = default 10).
        std::pair<uint16_t, bool> getCvToFloatChargeWaitTime() const;
        // Register 330 (UInt). CV to float charge wait time, minutes, 1..900 (0 = default 10).
        bool setCvToFloatChargeWaitTime(uint16_t minutes);
        // Register 331 (UInt). Battery charge source priority.
        std::pair<ANENJIBatteryChargePriority, bool> getBatteryChargePriority() const;
        // Register 331 (UInt). Battery charge source priority.
        bool setBatteryChargePriority(ANENJIBatteryChargePriority priority);
        // Register 332 (UInt). Max charge current (G), 0.1 A, 10..80 A.
        std::pair<float_t, bool> getMaxChargeCurrent() const;
        // Register 332 (UInt). Max charge current (G), 0.1 A, 10..80 A.
        bool setMaxChargeCurrent(float current);
        // Register 333 (UInt). Max mains charge current (H), 0.1 A, 2 A..G.
        std::pair<float_t, bool> getMaxMainsChargeCurrent() const;
        // Register 333 (UInt). Max mains charge current (H), 0.1 A, 2 A..G.
        bool setMaxMainsChargeCurrent(float current);
        // Register 334 (UInt). Equalization charge voltage, 0.1 V.
        std::pair<float_t, bool> getEqualizationChargeVoltage() const;
        // Register 334 (UInt). Equalization charge voltage, 0.1 V.
        bool setEqualizationChargeVoltage(float voltage);
        // Register 335 (UInt). Battery equalization time, minutes, 0..900.
        std::pair<uint16_t, bool> getBatteryEqualizationTime() const;
        // Register 335 (UInt). Battery equalization time, minutes, 0..900.
        bool setBatteryEqualizationTime(uint16_t minutes);
        // Register 336 (UInt). Equalization timeout, minutes, 0..900.
        std::pair<uint16_t, bool> getEqualizationTimeout() const;
        // Register 336 (UInt). Equalization timeout, minutes, 0..900.
        bool setEqualizationTimeout(uint16_t minutes);
        // Register 337 (UInt). Interval between equalization cycles, days, 1..90.
        std::pair<uint16_t, bool> getTwoTimesEqualizationInterval() const;
        // Register 337 (UInt). Interval between equalization cycles, days, 1..90.
        bool setTwoTimesEqualizationInterval(uint16_t days);
        // Register 338 (Bool). Automatic mains output enable.
        std::pair<bool, bool> getAutomaticMainsOutputEnable() const;
        // Register 338 (Bool). Automatic mains output enable.
        bool setAutomaticMainsOutputEnable(bool enabled);
        // Register 341 (UInt). Mains mode SOC protection point (K), %, 20..50.
        std::pair<uint8_t, bool> getMainsModeSocProtectionPoint() const;
        // Register 341 (UInt). Mains mode SOC protection point (K), %, 20..50.
        bool setMainsModeSocProtectionPoint(uint8_t percent);
        // Register 342 (UInt). Mains mode SOC recovery point, %, 60..100.
        std::pair<uint8_t, bool> getMainsModeSocRecoveryPoint() const;
        // Register 342 (UInt). Mains mode SOC recovery point, %, 60..100.
        bool setMainsModeSocRecoveryPoint(uint8_t percent);
        // Register 343 (UInt). Off-grid mode SOC protection point, %, 3..min(K, 30).
        std::pair<uint8_t, bool> getOffGridModeSocProtectionPoint() const;
        // Register 343 (UInt). Off-grid mode SOC protection point, %, 3..min(K, 30).
        bool setOffGridModeSocProtectionPoint(uint8_t percent);
        // Register 351 (UInt). Max discharge current protection, A.
        std::pair<uint16_t, bool> getMaxDischargeCurrentProtection() const;
        // Register 351 (UInt). Max discharge current protection, A.
        bool setMaxDischargeCurrentProtection(uint16_t current);
        // Register 406 (UInt). Boot behavior.
        std::pair<ANENJIBootMode, bool> getBootMode() const;
        // Register 406 (UInt). Boot behavior.
        bool setBootMode(ANENJIBootMode mode);
        // Register 420 (Bool). Remote switch, true = remote power-on, false = shutdown.
        std::pair<bool, bool> getRemoteSwitch() const;
        // Register 420 (Bool). Remote switch, true = remote power-on, false = shutdown.
        bool setRemoteSwitch(bool enabled);

        // Write-only commands.

        // Register 425 (UInt). Force battery equalization charge, writes 1.
        bool forceBatteryEqualizationCharge();
        // Register 426 (UInt). Exit fault lock state, writes 1.
        bool exitFaultLockState();
        // Register 460 (UInt). Clear operation and fault records, writes 0xAA.
        bool clearOperationAndFaultRecords();
        // Register 461 (UInt). Reset user parameters to defaults, writes 0xAA.
        bool resetUserParametersToDefaults();

        // Raw access for undocumented-format blocks, e.g. fault records 703..728 and run log 729..744
        // after selecting the record with setFaultQueryIndex().
        std::pair<uint16_t, bool> readRegisterRaw(uint16_t addr);
        bool writeRegisterRaw(uint16_t addr, uint16_t value);

    private:
        std::pair<uint16_t, bool> readU16(uint16_t addr) const;
        std::pair<int16_t, bool> readS16(uint16_t addr) const;
        std::pair<uint32_t, bool> readU32(uint16_t addr) const;
        std::pair<std::string, bool> readAscii(uint16_t addr, uint16_t numRegs) const;
        bool writeU16(uint16_t addr, uint16_t value) const;
        bool writeU32(uint16_t addr, uint32_t value) const;
        bool writeAscii(uint16_t addr, uint16_t numRegs, std::string value) const;
    };
}
