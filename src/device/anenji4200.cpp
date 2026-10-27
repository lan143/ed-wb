#include "anenji4200.h"

using namespace EDWB;

namespace
{
    uint16_t toRawUnsigned(float value, float multiplier)
    {
        return static_cast<uint16_t>(roundf(value * multiplier));
    }
}

std::pair<uint16_t, bool> ANENJI4200::readU16(uint16_t addr) const
{
    auto val = _client.holdingRegisterRead(_address, addr);
    if (val < 0) {
        return std::make_pair(0, false);
    }

    return std::make_pair(static_cast<uint16_t>(val), true);
}

std::pair<int16_t, bool> ANENJI4200::readS16(uint16_t addr) const
{
    auto val = _client.holdingRegisterRead(_address, addr);
    if (val < 0) {
        return std::make_pair(0, false);
    }

    return std::make_pair(static_cast<int16_t>(static_cast<uint16_t>(val)), true);
}

std::pair<uint32_t, bool> ANENJI4200::readU32(uint16_t addr) const
{
    auto high = _client.holdingRegisterRead(_address, addr);
    if (high < 0) {
        return std::make_pair(0, false);
    }

    auto low = _client.holdingRegisterRead(_address, addr + 1);
    if (low < 0) {
        return std::make_pair(0, false);
    }

    uint32_t result = (static_cast<uint32_t>(static_cast<uint16_t>(high)) << 16)
        | static_cast<uint32_t>(static_cast<uint16_t>(low));

    return std::make_pair(result, true);
}

std::pair<std::string, bool> ANENJI4200::readAscii(uint16_t addr, uint16_t numRegs) const
{
    std::string result;
    result.reserve(numRegs * 2);

    for (uint16_t i = 0; i < numRegs; i++) {
        auto val = _client.holdingRegisterRead(_address, addr + i);
        if (val < 0) {
            return std::make_pair(std::string(), false);
        }

        uint16_t raw = static_cast<uint16_t>(val);
        result.push_back(static_cast<char>(raw >> 8));
        result.push_back(static_cast<char>(raw & 0xFF));
    }

    while (!result.empty() && result.back() == '\0') {
        result.pop_back();
    }

    return std::make_pair(result, true);
}

bool ANENJI4200::writeU16(uint16_t addr, uint16_t value) const
{
    if (!_client.beginTransmission(_address, HOLDING_REGISTERS, addr, 1)) {
        return false;
    }

    if (!_client.write(value)) {
        return false;
    }

    return _client.endTransmission();
}

bool ANENJI4200::writeU32(uint16_t addr, uint32_t value) const
{
    if (!_client.beginTransmission(_address, HOLDING_REGISTERS, addr, 2)) {
        return false;
    }

    if (!_client.write(static_cast<uint16_t>(value >> 16))) {
        return false;
    }

    if (!_client.write(static_cast<uint16_t>(value & 0xFFFF))) {
        return false;
    }

    return _client.endTransmission();
}

bool ANENJI4200::writeAscii(uint16_t addr, uint16_t numRegs, std::string value) const
{
    std::string data = value.substr(0, numRegs * 2);
    data.resize(numRegs * 2, '\0');

    if (!_client.beginTransmission(_address, HOLDING_REGISTERS, addr, numRegs)) {
        return false;
    }

    for (uint16_t i = 0; i < numRegs; i++) {
        uint16_t high = static_cast<uint8_t>(data[i * 2]);
        uint16_t low = static_cast<uint8_t>(data[i * 2 + 1]);
        if (!_client.write(static_cast<uint16_t>((high << 8) | low))) {
            return false;
        }
    }

    return _client.endTransmission();
}

std::pair<uint16_t, bool> ANENJI4200::getDeviceType() const
{
    return readU16(171);
}

std::pair<std::string, bool> ANENJI4200::getDeviceName() const
{
    return readAscii(172, 12);
}

bool ANENJI4200::setDeviceName(std::string name)
{
    return writeAscii(172, 12, name);
}

std::pair<uint16_t, bool> ANENJI4200::getProtocolNumber() const
{
    return readU16(184);
}

std::pair<std::string, bool> ANENJI4200::getSerialNumber() const
{
    return readAscii(186, 12);
}

std::pair<std::string, bool> ANENJI4200::getProgramVersion() const
{
    return readAscii(626, 8);
}

std::pair<uint16_t, bool> ANENJI4200::getRatedPower() const
{
    return readU16(643);
}

std::pair<uint16_t, bool> ANENJI4200::getRatedCellCount() const
{
    return readU16(644);
}

std::pair<uint32_t, bool> ANENJI4200::getFaultCode() const
{
    return readU32(100);
}

std::pair<uint32_t, bool> ANENJI4200::getWarningCode() const
{
    return readU32(104);
}

std::pair<uint32_t, bool> ANENJI4200::getMaskedWarningCode() const
{
    return readU32(108);
}

bool ANENJI4200::setMaskedWarningCode(uint32_t code)
{
    return writeU32(108, code);
}

std::pair<uint32_t, bool> ANENJI4200::getWarningMask() const
{
    return readU32(314);
}

bool ANENJI4200::setWarningMask(uint32_t mask)
{
    return writeU32(314, mask);
}

std::pair<ANENJIWorkMode, bool> ANENJI4200::getWorkMode() const
{
    auto val = readU16(201);
    if (!val.second) {
        return std::make_pair(static_cast<ANENJIWorkMode>(0), false);
    }

    return std::make_pair(static_cast<ANENJIWorkMode>(val.first), true);
}

std::pair<uint16_t, bool> ANENJI4200::getPowerFlowStatus() const
{
    return readU16(231);
}

std::pair<float_t, bool> ANENJI4200::getMainsVoltage() const
{
    auto val = readS16(202);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<float_t, bool> ANENJI4200::getMainsFrequency() const
{
    auto val = readS16(203);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.01f, true);
}

std::pair<int16_t, bool> ANENJI4200::getMainsPower() const
{
    return readS16(204);
}

std::pair<float_t, bool> ANENJI4200::getInverterVoltage() const
{
    auto val = readS16(205);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<float_t, bool> ANENJI4200::getInverterCurrent() const
{
    auto val = readS16(206);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<float_t, bool> ANENJI4200::getInverterFrequency() const
{
    auto val = readS16(207);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.01f, true);
}

std::pair<int16_t, bool> ANENJI4200::getInverterPower() const
{
    return readS16(208);
}

std::pair<int16_t, bool> ANENJI4200::getInverterChargingPower() const
{
    return readS16(209);
}

std::pair<float_t, bool> ANENJI4200::getOutputVoltage() const
{
    auto val = readS16(210);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<float_t, bool> ANENJI4200::getOutputCurrent() const
{
    auto val = readS16(211);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<float_t, bool> ANENJI4200::getOutputFrequency() const
{
    auto val = readS16(212);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.01f, true);
}

std::pair<int16_t, bool> ANENJI4200::getOutputActivePower() const
{
    return readS16(213);
}

std::pair<int16_t, bool> ANENJI4200::getOutputApparentPower() const
{
    return readS16(214);
}

std::pair<float_t, bool> ANENJI4200::getBatteryVoltage() const
{
    auto val = readS16(215);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<float_t, bool> ANENJI4200::getBatteryCurrent() const
{
    auto val = readS16(216);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<int16_t, bool> ANENJI4200::getBatteryPower() const
{
    return readS16(217);
}

std::pair<float_t, bool> ANENJI4200::getPvVoltage() const
{
    auto val = readS16(219);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<float_t, bool> ANENJI4200::getPvCurrent() const
{
    auto val = readS16(220);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<int16_t, bool> ANENJI4200::getPvPower() const
{
    return readS16(223);
}

std::pair<int16_t, bool> ANENJI4200::getPvChargingPower() const
{
    return readS16(224);
}

std::pair<uint8_t, bool> ANENJI4200::getLoadPercent() const
{
    auto val = readS16(225);
    if (!val.second) {
        return std::make_pair(0, false);
    }

    return std::make_pair(static_cast<uint8_t>(val.first), true);
}

std::pair<int16_t, bool> ANENJI4200::getDcdcTemperature() const
{
    return readS16(226);
}

std::pair<int16_t, bool> ANENJI4200::getInverterTemperature() const
{
    return readS16(227);
}

std::pair<uint8_t, bool> ANENJI4200::getBatteryPercent() const
{
    auto val = readU16(229);
    if (!val.second) {
        return std::make_pair(0, false);
    }

    return std::make_pair(static_cast<uint8_t>(val.first), true);
}

std::pair<float_t, bool> ANENJI4200::getBatteryCurrentFiltered() const
{
    auto val = readS16(232);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<float_t, bool> ANENJI4200::getInverterChargingCurrent() const
{
    auto val = readS16(233);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<float_t, bool> ANENJI4200::getPvChargingCurrent() const
{
    auto val = readS16(234);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

std::pair<uint32_t, bool> ANENJI4200::getFaultRecordStorage() const
{
    return readU32(700);
}

std::pair<uint16_t, bool> ANENJI4200::getFaultQueryIndex() const
{
    return readU16(702);
}

bool ANENJI4200::setFaultQueryIndex(uint16_t index)
{
    return writeU16(702, index);
}

std::pair<ANENJIOutputMode, bool> ANENJI4200::getOutputMode() const
{
    auto val = readU16(300);
    if (!val.second) {
        return std::make_pair(static_cast<ANENJIOutputMode>(0), false);
    }

    return std::make_pair(static_cast<ANENJIOutputMode>(val.first), true);
}

bool ANENJI4200::setOutputMode(ANENJIOutputMode mode)
{
    return writeU16(300, static_cast<uint16_t>(mode));
}

std::pair<ANENJIOutputPriority, bool> ANENJI4200::getOutputPriority() const
{
    auto val = readU16(301);
    if (!val.second) {
        return std::make_pair(static_cast<ANENJIOutputPriority>(0), false);
    }

    return std::make_pair(static_cast<ANENJIOutputPriority>(val.first), true);
}

bool ANENJI4200::setOutputPriority(ANENJIOutputPriority priority)
{
    return writeU16(301, static_cast<uint16_t>(priority));
}

std::pair<ANENJIInputVoltageRange, bool> ANENJI4200::getInputVoltageRange() const
{
    auto val = readU16(302);
    if (!val.second) {
        return std::make_pair(static_cast<ANENJIInputVoltageRange>(0), false);
    }

    return std::make_pair(static_cast<ANENJIInputVoltageRange>(val.first), true);
}

bool ANENJI4200::setInputVoltageRange(ANENJIInputVoltageRange range)
{
    return writeU16(302, static_cast<uint16_t>(range));
}

std::pair<ANENJIBuzzerMode, bool> ANENJI4200::getBuzzerMode() const
{
    auto val = readU16(303);
    if (!val.second) {
        return std::make_pair(static_cast<ANENJIBuzzerMode>(0), false);
    }

    return std::make_pair(static_cast<ANENJIBuzzerMode>(val.first), true);
}

bool ANENJI4200::setBuzzerMode(ANENJIBuzzerMode mode)
{
    return writeU16(303, static_cast<uint16_t>(mode));
}

std::pair<bool, bool> ANENJI4200::getLcdBacklight() const
{
    auto val = readU16(305);
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool ANENJI4200::setLcdBacklight(bool enabled)
{
    return writeU16(305, enabled ? 1 : 0);
}

std::pair<bool, bool> ANENJI4200::getLcdAutoReturnHome() const
{
    auto val = readU16(306);
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool ANENJI4200::setLcdAutoReturnHome(bool enabled)
{
    return writeU16(306, enabled ? 1 : 0);
}

std::pair<bool, bool> ANENJI4200::getEnergySavingMode() const
{
    auto val = readU16(307);
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool ANENJI4200::setEnergySavingMode(bool enabled)
{
    return writeU16(307, enabled ? 1 : 0);
}

std::pair<bool, bool> ANENJI4200::getOverloadAutoRestart() const
{
    auto val = readU16(308);
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool ANENJI4200::setOverloadAutoRestart(bool enabled)
{
    return writeU16(308, enabled ? 1 : 0);
}

std::pair<bool, bool> ANENJI4200::getOverTemperatureAutoRestart() const
{
    auto val = readU16(309);
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool ANENJI4200::setOverTemperatureAutoRestart(bool enabled)
{
    return writeU16(309, enabled ? 1 : 0);
}

std::pair<bool, bool> ANENJI4200::getOverloadBypassEnable() const
{
    auto val = readU16(310);
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool ANENJI4200::setOverloadBypassEnable(bool enabled)
{
    return writeU16(310, enabled ? 1 : 0);
}

std::pair<bool, bool> ANENJI4200::getBatteryEqualizationModeEnable() const
{
    auto val = readU16(313);
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool ANENJI4200::setBatteryEqualizationModeEnable(bool enabled)
{
    return writeU16(313, enabled ? 1 : 0);
}

std::pair<bool, bool> ANENJI4200::getDryContact() const
{
    auto val = readU16(316);
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool ANENJI4200::setDryContact(bool enabled)
{
    return writeU16(316, enabled ? 1 : 0);
}

std::pair<float_t, bool> ANENJI4200::getTargetOutputVoltage() const
{
    auto val = readU16(320);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

bool ANENJI4200::setTargetOutputVoltage(float voltage)
{
    return writeU16(320, toRawUnsigned(voltage, 10.0f));
}

std::pair<float_t, bool> ANENJI4200::getTargetOutputFrequency() const
{
    auto val = readU16(321);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.01f, true);
}

bool ANENJI4200::setTargetOutputFrequency(float frequency)
{
    return writeU16(321, toRawUnsigned(frequency, 100.0f));
}

std::pair<ANENJIBatteryType, bool> ANENJI4200::getBatteryType() const
{
    auto val = readU16(322);
    if (!val.second) {
        return std::make_pair(static_cast<ANENJIBatteryType>(0), false);
    }

    return std::make_pair(static_cast<ANENJIBatteryType>(val.first), true);
}

bool ANENJI4200::setBatteryType(ANENJIBatteryType type)
{
    return writeU16(322, static_cast<uint16_t>(type));
}

std::pair<float_t, bool> ANENJI4200::getBatteryOvervoltageProtectionPoint() const
{
    auto val = readU16(323);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

bool ANENJI4200::setBatteryOvervoltageProtectionPoint(float voltage)
{
    return writeU16(323, toRawUnsigned(voltage, 10.0f));
}

std::pair<float_t, bool> ANENJI4200::getMaxChargeVoltage() const
{
    auto val = readU16(324);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

bool ANENJI4200::setMaxChargeVoltage(float voltage)
{
    return writeU16(324, toRawUnsigned(voltage, 10.0f));
}

std::pair<float_t, bool> ANENJI4200::getFloatingChargeVoltage() const
{
    auto val = readU16(325);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

bool ANENJI4200::setFloatingChargeVoltage(float voltage)
{
    return writeU16(325, toRawUnsigned(voltage, 10.0f));
}

std::pair<float_t, bool> ANENJI4200::getMainsDischargeRecoveryPoint() const
{
    auto val = readU16(326);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

bool ANENJI4200::setMainsDischargeRecoveryPoint(float voltage)
{
    return writeU16(326, toRawUnsigned(voltage, 10.0f));
}

std::pair<float_t, bool> ANENJI4200::getMainsModeBatteryLowVoltageProtection() const
{
    auto val = readU16(327);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

bool ANENJI4200::setMainsModeBatteryLowVoltageProtection(float voltage)
{
    return writeU16(327, toRawUnsigned(voltage, 10.0f));
}

std::pair<float_t, bool> ANENJI4200::getOffGridModeBatteryLowVoltageProtection() const
{
    auto val = readU16(329);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

bool ANENJI4200::setOffGridModeBatteryLowVoltageProtection(float voltage)
{
    return writeU16(329, toRawUnsigned(voltage, 10.0f));
}

std::pair<uint16_t, bool> ANENJI4200::getCvToFloatChargeWaitTime() const
{
    return readU16(330);
}

bool ANENJI4200::setCvToFloatChargeWaitTime(uint16_t minutes)
{
    return writeU16(330, minutes);
}

std::pair<ANENJIBatteryChargePriority, bool> ANENJI4200::getBatteryChargePriority() const
{
    auto val = readU16(331);
    if (!val.second) {
        return std::make_pair(static_cast<ANENJIBatteryChargePriority>(0), false);
    }

    return std::make_pair(static_cast<ANENJIBatteryChargePriority>(val.first), true);
}

bool ANENJI4200::setBatteryChargePriority(ANENJIBatteryChargePriority priority)
{
    return writeU16(331, static_cast<uint16_t>(priority));
}

std::pair<float_t, bool> ANENJI4200::getMaxChargeCurrent() const
{
    auto val = readU16(332);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

bool ANENJI4200::setMaxChargeCurrent(float current)
{
    return writeU16(332, toRawUnsigned(current, 10.0f));
}

std::pair<float_t, bool> ANENJI4200::getMaxMainsChargeCurrent() const
{
    auto val = readU16(333);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

bool ANENJI4200::setMaxMainsChargeCurrent(float current)
{
    return writeU16(333, toRawUnsigned(current, 10.0f));
}

std::pair<float_t, bool> ANENJI4200::getEqualizationChargeVoltage() const
{
    auto val = readU16(334);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

bool ANENJI4200::setEqualizationChargeVoltage(float voltage)
{
    return writeU16(334, toRawUnsigned(voltage, 10.0f));
}

std::pair<uint16_t, bool> ANENJI4200::getBatteryEqualizationTime() const
{
    return readU16(335);
}

bool ANENJI4200::setBatteryEqualizationTime(uint16_t minutes)
{
    return writeU16(335, minutes);
}

std::pair<uint16_t, bool> ANENJI4200::getEqualizationTimeout() const
{
    return readU16(336);
}

bool ANENJI4200::setEqualizationTimeout(uint16_t minutes)
{
    return writeU16(336, minutes);
}

std::pair<uint16_t, bool> ANENJI4200::getTwoTimesEqualizationInterval() const
{
    return readU16(337);
}

bool ANENJI4200::setTwoTimesEqualizationInterval(uint16_t days)
{
    return writeU16(337, days);
}

std::pair<bool, bool> ANENJI4200::getAutomaticMainsOutputEnable() const
{
    auto val = readU16(338);
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool ANENJI4200::setAutomaticMainsOutputEnable(bool enabled)
{
    return writeU16(338, enabled ? 1 : 0);
}

std::pair<uint8_t, bool> ANENJI4200::getMainsModeSocProtectionPoint() const
{
    auto val = readU16(341);
    if (!val.second) {
        return std::make_pair(0, false);
    }

    return std::make_pair(static_cast<uint8_t>(val.first), true);
}

bool ANENJI4200::setMainsModeSocProtectionPoint(uint8_t percent)
{
    return writeU16(341, percent);
}

std::pair<uint8_t, bool> ANENJI4200::getMainsModeSocRecoveryPoint() const
{
    auto val = readU16(342);
    if (!val.second) {
        return std::make_pair(0, false);
    }

    return std::make_pair(static_cast<uint8_t>(val.first), true);
}

bool ANENJI4200::setMainsModeSocRecoveryPoint(uint8_t percent)
{
    return writeU16(342, percent);
}

std::pair<uint8_t, bool> ANENJI4200::getOffGridModeSocProtectionPoint() const
{
    auto val = readU16(343);
    if (!val.second) {
        return std::make_pair(0, false);
    }

    return std::make_pair(static_cast<uint8_t>(val.first), true);
}

bool ANENJI4200::setOffGridModeSocProtectionPoint(uint8_t percent)
{
    return writeU16(343, percent);
}

std::pair<uint16_t, bool> ANENJI4200::getMaxDischargeCurrentProtection() const
{
    return readU16(351);
}

bool ANENJI4200::setMaxDischargeCurrentProtection(uint16_t current)
{
    return writeU16(351, current);
}

std::pair<ANENJIBootMode, bool> ANENJI4200::getBootMode() const
{
    auto val = readU16(406);
    if (!val.second) {
        return std::make_pair(static_cast<ANENJIBootMode>(0), false);
    }

    return std::make_pair(static_cast<ANENJIBootMode>(val.first), true);
}

bool ANENJI4200::setBootMode(ANENJIBootMode mode)
{
    return writeU16(406, static_cast<uint16_t>(mode));
}

std::pair<bool, bool> ANENJI4200::getRemoteSwitch() const
{
    auto val = readU16(420);
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool ANENJI4200::setRemoteSwitch(bool enabled)
{
    return writeU16(420, enabled ? 1 : 0);
}

bool ANENJI4200::forceBatteryEqualizationCharge()
{
    return writeU16(425, 1);
}

bool ANENJI4200::exitFaultLockState()
{
    return writeU16(426, 1);
}

bool ANENJI4200::clearOperationAndFaultRecords()
{
    return writeU16(460, 0xAA);
}

bool ANENJI4200::resetUserParametersToDefaults()
{
    return writeU16(461, 0xAA);
}

std::pair<uint16_t, bool> ANENJI4200::readRegisterRaw(uint16_t addr)
{
    return readU16(addr);
}

bool ANENJI4200::writeRegisterRaw(uint16_t addr, uint16_t value)
{
    return writeU16(addr, value);
}
