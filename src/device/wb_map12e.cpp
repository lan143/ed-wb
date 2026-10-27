#include <cmath>

#include "wb_map12e.h"

using namespace EDWB;

std::pair<uint16_t, bool> MAP12E::readInputU16(uint16_t reg) const
{
    auto val = _client.inputRegisterRead(_address, reg);
    if (val < 0) {
        return std::make_pair(0, false);
    }

    return std::make_pair(static_cast<uint16_t>(val), true);
}

std::pair<int16_t, bool> MAP12E::readInputS16(uint16_t reg) const
{
    auto val = _client.inputRegisterRead(_address, reg);
    if (val < 0) {
        return std::make_pair(0, false);
    }

    return std::make_pair(static_cast<int16_t>(static_cast<uint16_t>(val)), true);
}

std::pair<uint32_t, bool> MAP12E::readInputU32BE(uint16_t reg) const
{
    auto high = readInputU16(reg);
    if (!high.second) {
        return std::make_pair(0, false);
    }

    auto low = readInputU16(reg + 1);
    if (!low.second) {
        return std::make_pair(0, false);
    }

    uint32_t result = (static_cast<uint32_t>(high.first) << 16)
        | static_cast<uint32_t>(low.first);

    return std::make_pair(result, true);
}

std::pair<int32_t, bool> MAP12E::readInputS32BE(uint16_t reg) const
{
    auto val = readInputU32BE(reg);
    if (!val.second) {
        return std::make_pair(0, false);
    }

    return std::make_pair(static_cast<int32_t>(val.first), true);
}

std::pair<int32_t, bool> MAP12E::readInputS32LE(uint16_t reg) const
{
    auto low = readInputU16(reg);
    if (!low.second) {
        return std::make_pair(0, false);
    }

    auto high = readInputU16(reg + 1);
    if (!high.second) {
        return std::make_pair(0, false);
    }

    uint32_t result = static_cast<uint32_t>(low.first)
        | (static_cast<uint32_t>(high.first) << 16);

    return std::make_pair(static_cast<int32_t>(result), true);
}

std::pair<uint64_t, bool> MAP12E::readInputU64LE(uint16_t reg) const
{
    auto w0 = readInputU16(reg);
    if (!w0.second) {
        return std::make_pair(static_cast<uint64_t>(0), false);
    }

    auto w1 = readInputU16(reg + 1);
    if (!w1.second) {
        return std::make_pair(static_cast<uint64_t>(0), false);
    }

    auto w2 = readInputU16(reg + 2);
    if (!w2.second) {
        return std::make_pair(static_cast<uint64_t>(0), false);
    }

    auto w3 = readInputU16(reg + 3);
    if (!w3.second) {
        return std::make_pair(static_cast<uint64_t>(0), false);
    }

    uint64_t result = static_cast<uint64_t>(w0.first)
        | (static_cast<uint64_t>(w1.first) << 16)
        | (static_cast<uint64_t>(w2.first) << 32)
        | (static_cast<uint64_t>(w3.first) << 48);

    return std::make_pair(result, true);
}

std::pair<uint16_t, bool> MAP12E::readHoldingU16(uint16_t reg) const
{
    auto val = _client.holdingRegisterRead(_address, reg);
    if (val < 0) {
        return std::make_pair(0, false);
    }

    return std::make_pair(static_cast<uint16_t>(val), true);
}

bool MAP12E::writeHoldingU16(uint16_t reg, uint16_t value) const
{
    return _client.holdingRegisterWrite(_address, reg, value) > 0;
}

bool MAP12E::writeHoldingS16(uint16_t reg, int16_t value) const
{
    return writeHoldingU16(reg, static_cast<uint16_t>(value));
}

bool MAP12E::validateChannel(uint8_t channel) const
{
    return channel >= 1 && channel <= 4;
}

bool MAP12E::validatePhase(MAP12EPhase phase) const
{
    return static_cast<uint16_t>(phase) <= static_cast<uint16_t>(MAP12E_PHASE_L3);
}

bool MAP12E::validateLine(MAP12ELine line) const
{
    return static_cast<uint16_t>(line) <= static_cast<uint16_t>(MAP12E_LINE_L3L1);
}

// Global measurements.

std::pair<float_t, bool> MAP12E::getVoltage(MAP12EPhase phase) const
{
    if (!validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    auto val = readInputU16(0x1410 + 2 * static_cast<uint16_t>(phase));
    if (!val.second || val.first == 0xFFFF) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.01f, true);
}

std::pair<float_t, bool> MAP12E::getVoltageHighRes(MAP12EPhase phase) const
{
    if (!validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    auto val = readInputU32BE(0x1410 + 2 * static_cast<uint16_t>(phase));
    if (!val.second || val.first == 0xFFFFFFFF) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1.52588e-7f, true);
}

std::pair<float_t, bool> MAP12E::getPeakVoltage(MAP12EPhase phase) const
{
    if (!validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    auto val = readInputS32LE(0x1800 + 2 * static_cast<uint16_t>(phase));
    if (!val.second || val.first == 0x7FFFFFFF) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.01f, true);
}

std::pair<float_t, bool> MAP12E::getAggregatedPeakVoltage(MAP12EPhase phase) const
{
    if (!validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    auto val = readInputS32LE(0x1810 + 2 * static_cast<uint16_t>(phase));
    if (!val.second || val.first == 0x7FFFFFFF) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.01f, true);
}

std::pair<float_t, bool> MAP12E::getLineVoltage(MAP12ELine line) const
{
    if (!validateLine(line)) {
        return std::make_pair(0.0f, false);
    }

    if (_fwVersion < Version{2, 5, 0}) {
        return std::make_pair(0.0f, false);
    }

    auto val = readInputU16(0x141C + static_cast<uint16_t>(line));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.01f, true);
}

std::pair<float_t, bool> MAP12E::getFrequency() const
{
    auto val = readInputU16(0x10F8);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.01f, true);
}

std::pair<float_t, bool> MAP12E::getVoltageAngle(MAP12EPhase phase) const
{
    if (!validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    auto val = readInputU16(0x10FD + static_cast<uint16_t>(phase));
    if (!val.second || val.first == 0x0FFF) {
        return std::make_pair(0.0f, false);
    }

    auto raw = static_cast<int16_t>(val.first);
    return std::make_pair(static_cast<float_t>(raw) * 0.1f, true);
}

// Per-channel measurements.

std::pair<float_t, bool> MAP12E::getCurrent(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU16(chBase + 0x1416 + 2 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.016f, true);
}

std::pair<float_t, bool> MAP12E::getCurrentHighRes(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU32BE(chBase + 0x1416 + 2 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 2.44141e-7f, true);
}

std::pair<float_t, bool> MAP12E::getCurrentPeak(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS32LE(chBase + 0x1808 + 2 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.016f, true);
}

std::pair<float_t, bool> MAP12E::getAggregatedCurrentPeak(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS32LE(chBase + 0x1818 + 2 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.016f, true);
}

std::pair<float_t, bool> MAP12E::getActivePower(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS32BE(chBase + 0x1302 + 2 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.00512f, true);
}

std::pair<float_t, bool> MAP12E::getTotalActivePower(uint8_t channel) const
{
    if (!validateChannel(channel)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS32BE(chBase + 0x1300);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.00512f, true);
}

std::pair<float_t, bool> MAP12E::getReactivePower(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS32BE(chBase + 0x130A + 2 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.00512f, true);
}

std::pair<float_t, bool> MAP12E::getTotalReactivePower(uint8_t channel) const
{
    if (!validateChannel(channel)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS32BE(chBase + 0x1308);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.00512f, true);
}

std::pair<float_t, bool> MAP12E::getApparentPower(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS32BE(chBase + 0x1312 + 2 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.00512f, true);
}

std::pair<float_t, bool> MAP12E::getTotalApparentPower(uint8_t channel) const
{
    if (!validateChannel(channel)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS32BE(chBase + 0x1310);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.00512f, true);
}

std::pair<float_t, bool> MAP12E::getNonActivePower(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS32BE(chBase + 0x1582 + 2 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.00512f, true);
}

std::pair<float_t, bool> MAP12E::getTotalNonActivePower(uint8_t channel) const
{
    if (!validateChannel(channel)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS32BE(chBase + 0x1580);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.00512f, true);
}

std::pair<float_t, bool> MAP12E::getPowerFactor(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS16(chBase + 0x10BC + 1 + static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.001f, true);
}

std::pair<float_t, bool> MAP12E::getTotalPowerFactor(uint8_t channel) const
{
    if (!validateChannel(channel)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS16(chBase + 0x10BC);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.001f, true);
}

std::pair<float_t, bool> MAP12E::getPhaseAngle(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputS16(chBase + 0x10F9 + static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 0.1f, true);
}

// Energy counters.

std::pair<float_t, bool> MAP12E::getForwardActiveEnergy(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1204 + 4 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1e-5f, true);
}

std::pair<float_t, bool> MAP12E::getReverseActiveEnergy(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1214 + 4 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1e-5f, true);
}

std::pair<float_t, bool> MAP12E::getTotalForwardActiveEnergy(uint8_t channel) const
{
    if (!validateChannel(channel)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1200);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1e-5f, true);
}

std::pair<float_t, bool> MAP12E::getTotalReverseActiveEnergy(uint8_t channel) const
{
    if (!validateChannel(channel)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1210);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1e-5f, true);
}

std::pair<float_t, bool> MAP12E::getForwardReactiveEnergy(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1224 + 4 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1e-5f, true);
}

std::pair<float_t, bool> MAP12E::getReverseReactiveEnergy(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1234 + 4 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1e-5f, true);
}

std::pair<float_t, bool> MAP12E::getTotalForwardReactiveEnergy(uint8_t channel) const
{
    if (!validateChannel(channel)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1220);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1e-5f, true);
}

std::pair<float_t, bool> MAP12E::getTotalReverseReactiveEnergy(uint8_t channel) const
{
    if (!validateChannel(channel)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1230);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1e-5f, true);
}

std::pair<float_t, bool> MAP12E::getForwardNonActiveEnergy(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1504 + 4 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1.422222e-9f, true);
}

std::pair<float_t, bool> MAP12E::getReverseNonActiveEnergy(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1514 + 4 * static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1.422222e-9f, true);
}

std::pair<float_t, bool> MAP12E::getTotalForwardNonActiveEnergy(uint8_t channel) const
{
    if (!validateChannel(channel)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1500);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1.422222e-9f, true);
}

std::pair<float_t, bool> MAP12E::getTotalReverseNonActiveEnergy(uint8_t channel) const
{
    if (!validateChannel(channel)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>((channel - 1) * 0x1000);
    auto val = readInputU64LE(chBase + 0x1510);
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair(static_cast<float_t>(val.first) * 1.422222e-9f, true);
}

// Configuration.

std::pair<uint16_t, bool> MAP12E::getCTTransformRatio(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0, false);
    }

    uint16_t chBase = static_cast<uint16_t>(channel * 0x1000);
    return readHoldingU16(chBase + 0x460 + static_cast<uint16_t>(phase));
}

bool MAP12E::setCTTransformRatio(uint8_t channel, MAP12EPhase phase, uint16_t ratio) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return false;
    }

    uint16_t chBase = static_cast<uint16_t>(channel * 0x1000);
    return writeHoldingU16(chBase + 0x460 + static_cast<uint16_t>(phase), ratio);
}

std::pair<float_t, bool> MAP12E::getCTPhaseDelay(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(0.0f, false);
    }

    uint16_t chBase = static_cast<uint16_t>(channel * 0x1000);
    auto val = readHoldingU16(chBase + 0x463 + static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(0.0f, false);
    }

    auto raw = static_cast<int16_t>(val.first);
    return std::make_pair(static_cast<float_t>(raw) * 0.001f, true);
}

bool MAP12E::setCTPhaseDelay(uint8_t channel, MAP12EPhase phase, float_t degrees) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return false;
    }

    if (degrees < -32.767f || degrees > 32.767f) {
        return false;
    }

    uint16_t chBase = static_cast<uint16_t>(channel * 0x1000);
    auto raw = static_cast<int16_t>(roundf(degrees * 1000.0f));
    return writeHoldingS16(chBase + 0x463 + static_cast<uint16_t>(phase), raw);
}

std::pair<uint16_t, bool> MAP12E::getCTInputPhase(uint8_t channel, uint8_t ctInput) const
{
    if (!validateChannel(channel) || ctInput < 1 || ctInput > 3) {
        return std::make_pair(0, false);
    }

    if (_fwVersion < Version{2, 3, 2}) {
        return std::make_pair(0, false);
    }

    uint16_t chBase = static_cast<uint16_t>(channel * 0x1000);
    return readHoldingU16(chBase + 0x4A0 + (ctInput - 1));
}

bool MAP12E::setCTInputPhase(uint8_t channel, uint8_t ctInput, uint16_t phase) const
{
    if (!validateChannel(channel) || ctInput < 1 || ctInput > 3) {
        return false;
    }

    if (phase < 1 || phase > 3) {
        return false;
    }

    if (_fwVersion < Version{2, 3, 2}) {
        return false;
    }

    uint16_t chBase = static_cast<uint16_t>(channel * 0x1000);
    return writeHoldingU16(chBase + 0x4A0 + (ctInput - 1), phase);
}

std::pair<bool, bool> MAP12E::getCTInvert(uint8_t channel, MAP12EPhase phase) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return std::make_pair(false, false);
    }

    if (_fwVersion < Version{2, 10, 0}) {
        return std::make_pair(false, false);
    }

    uint16_t chBase = static_cast<uint16_t>(channel * 0x1000);
    auto val = readHoldingU16(chBase + 0x4B0 + static_cast<uint16_t>(phase));
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool MAP12E::setCTInvert(uint8_t channel, MAP12EPhase phase, bool invert) const
{
    if (!validateChannel(channel) || !validatePhase(phase)) {
        return false;
    }

    if (_fwVersion < Version{2, 10, 0}) {
        return false;
    }

    uint16_t chBase = static_cast<uint16_t>(channel * 0x1000);
    return writeHoldingU16(chBase + 0x4B0 + static_cast<uint16_t>(phase), invert ? 1 : 0);
}

std::pair<uint16_t, bool> MAP12E::getPeakAggregationPeriod() const
{
    if (_fwVersion < Version{2, 3, 0}) {
        return std::make_pair(0, false);
    }

    return readHoldingU16(0x10F0);
}

bool MAP12E::setPeakAggregationPeriod(uint16_t seconds) const
{
    if (seconds == 0) {
        return false;
    }

    if (_fwVersion < Version{2, 3, 0}) {
        return false;
    }

    return writeHoldingU16(0x10F0, seconds);
}

std::pair<uint16_t, bool> MAP12E::getPeakDetectPeriod() const
{
    if (_fwVersion < Version{2, 13, 0}) {
        return std::make_pair(0, false);
    }

    return readHoldingU16(0x10F4);
}

bool MAP12E::setPeakDetectPeriod(uint16_t milliseconds) const
{
    if (milliseconds == 0 || milliseconds > 255) {
        return false;
    }

    if (_fwVersion < Version{2, 13, 0}) {
        return false;
    }

    return writeHoldingU16(0x10F4, milliseconds);
}

std::pair<bool, bool> MAP12E::getPhaseLossDetection() const
{
    if (_fwVersion < Version{2, 6, 3}) {
        return std::make_pair(false, false);
    }

    auto val = readHoldingU16(0x0066);
    if (!val.second) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val.first != 0, true);
}

bool MAP12E::setPhaseLossDetection(bool enable) const
{
    if (_fwVersion < Version{2, 6, 3}) {
        return false;
    }

    return writeHoldingU16(0x0066, enable ? 1 : 0);
}
