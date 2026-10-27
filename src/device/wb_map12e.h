#pragma once

#include <cstdint>
#include <utility>

#include <iarduino_Modbus.h>

#include "wb_device.h"

namespace EDWB
{
    enum MAP12EPhase : uint16_t
    {
        MAP12E_PHASE_L1 = 0,
        MAP12E_PHASE_L2 = 1,
        MAP12E_PHASE_L3 = 2
    };

    enum MAP12ELine : uint16_t
    {
        MAP12E_LINE_L1L2 = 0,
        MAP12E_LINE_L2L3 = 1,
        MAP12E_LINE_L3L1 = 2
    };

    // WB-MAP12E three-phase energy meter, Modbus RTU.
    // Every measurement is an input register (RO).
    // Channel 1 addresses start at 0x1000, each next channel adds 0x1000:
    // register = channel 1 address + (channel - 1) * 0x1000.
    // Energies are returned as float: precision loss above ~10^7 kWh is
    // acceptable for display; the raw u64 value is available via readInputU64LE.
    class MAP12E : public WBDevice
    {
    public:
        MAP12E(ModbusClient& client) : WBDevice(client) {}

        // Global measurements.

        // Input 0x1410 + 2p, u16, 0.01 V; raw 0xFFFF = error. FW 2.1.
        std::pair<float_t, bool> getVoltage(MAP12EPhase phase) const;
        // Input 0x1410 + 2p, u32 BE, 1.52588e-7 V; raw 0xFFFFFFFF = error. FW 2.1.
        std::pair<float_t, bool> getVoltageHighRes(MAP12EPhase phase) const;
        // Input 0x1800 + 2p, s32 LE, 0.01 V; raw 0x7FFFFFFF = error. FW 2.1.0.
        // Since FW 2.8.0 the value is always positive.
        std::pair<float_t, bool> getPeakVoltage(MAP12EPhase phase) const;
        // Input 0x1810 + 2p, s32 LE, 0.01 V; raw 0x7FFFFFFF = error. FW 2.1.
        // Since FW 2.8.0 the value is always positive.
        std::pair<float_t, bool> getAggregatedPeakVoltage(MAP12EPhase phase) const;
        // Input 0x141C + line, u16, 0.01 V. Requires FW 2.5.0.
        std::pair<float_t, bool> getLineVoltage(MAP12ELine line) const;
        // Input 0x10F8, u16, 0.01 Hz. FW 2.1.
        std::pair<float_t, bool> getFrequency() const;
        // Input 0x10FD + p, s16, 0.1 deg; raw 0x0FFF = error. FW 2.1.
        // L1 angle is always 0 (reference phase).
        std::pair<float_t, bool> getVoltageAngle(MAP12EPhase phase) const;

        // Per-channel measurements.

        // Input 0x1416 + 2p + (channel - 1) * 0x1000, u16, 0.016 A. FW 2.1.
        std::pair<float_t, bool> getCurrent(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1416 + 2p + (channel - 1) * 0x1000, u32 BE, 2.44141e-7 A. FW 2.1.
        std::pair<float_t, bool> getCurrentHighRes(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1808 + 2p + (channel - 1) * 0x1000, s32 LE, 0.016 A. FW 2.1.0.
        // Since FW 2.8.0 the value is always positive.
        std::pair<float_t, bool> getCurrentPeak(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1818 + 2p + (channel - 1) * 0x1000, s32 LE, 0.016 A. FW 2.1.
        // Since FW 2.8.0 the value is always positive.
        std::pair<float_t, bool> getAggregatedCurrentPeak(uint8_t channel, MAP12EPhase phase) const;

        // Input 0x1302 + 2p + (channel - 1) * 0x1000, s32 BE, 0.00512 W. FW 2.1.
        std::pair<float_t, bool> getActivePower(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1300 + (channel - 1) * 0x1000, s32 BE, 0.00512 W. FW 2.1.
        std::pair<float_t, bool> getTotalActivePower(uint8_t channel) const;
        // Input 0x130A + 2p + (channel - 1) * 0x1000, s32 BE, 0.00512 var. FW 2.1.
        std::pair<float_t, bool> getReactivePower(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1308 + (channel - 1) * 0x1000, s32 BE, 0.00512 var. FW 2.1.
        std::pair<float_t, bool> getTotalReactivePower(uint8_t channel) const;
        // Input 0x1312 + 2p + (channel - 1) * 0x1000, s32 BE, 0.00512 VA. FW 2.1.
        std::pair<float_t, bool> getApparentPower(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1310 + (channel - 1) * 0x1000, s32 BE, 0.00512 VA. FW 2.1.
        std::pair<float_t, bool> getTotalApparentPower(uint8_t channel) const;
        // Input 0x1582 + 2p + (channel - 1) * 0x1000, s32 BE, 0.00512 var. FW 2.1.
        std::pair<float_t, bool> getNonActivePower(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1580 + (channel - 1) * 0x1000, s32 BE, 0.00512 var. FW 2.1.
        std::pair<float_t, bool> getTotalNonActivePower(uint8_t channel) const;

        // Input 0x10BC + 1 + p + (channel - 1) * 0x1000, s16, 0.001. FW 2.1.
        std::pair<float_t, bool> getPowerFactor(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x10BC + (channel - 1) * 0x1000, s16, 0.001. FW 2.1.
        std::pair<float_t, bool> getTotalPowerFactor(uint8_t channel) const;
        // Input 0x10F9 + p + (channel - 1) * 0x1000, s16, 0.1 deg. FW 2.1.
        std::pair<float_t, bool> getPhaseAngle(uint8_t channel, MAP12EPhase phase) const;

        // Energy counters.
        // FW 2.9.0 introduces big-endian duplicates at +0x800 blocks
        // (0x1A00..0x1A30); intentionally not wrapped - identical values,
        // use the originals below.

        // Input 0x1204 + 4p + (channel - 1) * 0x1000, u64 LE, 1e-5 kWh. FW 2.1.
        std::pair<float_t, bool> getForwardActiveEnergy(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1214 + 4p + (channel - 1) * 0x1000, u64 LE, 1e-5 kWh. FW 2.1.
        std::pair<float_t, bool> getReverseActiveEnergy(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1200 + (channel - 1) * 0x1000, u64 LE, 1e-5 kWh. FW 2.1.
        std::pair<float_t, bool> getTotalForwardActiveEnergy(uint8_t channel) const;
        // Input 0x1210 + (channel - 1) * 0x1000, u64 LE, 1e-5 kWh. FW 2.1.
        std::pair<float_t, bool> getTotalReverseActiveEnergy(uint8_t channel) const;

        // Input 0x1224 + 4p + (channel - 1) * 0x1000, u64 LE, 1e-5 kvar*h. FW 2.1.
        std::pair<float_t, bool> getForwardReactiveEnergy(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1234 + 4p + (channel - 1) * 0x1000, u64 LE, 1e-5 kvar*h. FW 2.1.
        std::pair<float_t, bool> getReverseReactiveEnergy(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1220 + (channel - 1) * 0x1000, u64 LE, 1e-5 kvar*h. FW 2.1.
        std::pair<float_t, bool> getTotalForwardReactiveEnergy(uint8_t channel) const;
        // Input 0x1230 + (channel - 1) * 0x1000, u64 LE, 1e-5 kvar*h. FW 2.1.
        std::pair<float_t, bool> getTotalReverseReactiveEnergy(uint8_t channel) const;

        // Input 0x1504 + 4p + (channel - 1) * 0x1000, u64 LE, 1.422222e-9 kvar*h. FW 2.1.
        std::pair<float_t, bool> getForwardNonActiveEnergy(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1514 + 4p + (channel - 1) * 0x1000, u64 LE, 1.422222e-9 kvar*h. FW 2.1.
        std::pair<float_t, bool> getReverseNonActiveEnergy(uint8_t channel, MAP12EPhase phase) const;
        // Input 0x1500 + (channel - 1) * 0x1000, u64 LE, 1.422222e-9 kvar*h. FW 2.1.
        std::pair<float_t, bool> getTotalForwardNonActiveEnergy(uint8_t channel) const;
        // Input 0x1510 + (channel - 1) * 0x1000, u64 LE, 1.422222e-9 kvar*h. FW 2.1.
        std::pair<float_t, bool> getTotalReverseNonActiveEnergy(uint8_t channel) const;

        // Configuration.
        // Service registers (reboot, FlashFS, stack/memory, bootloader) are intentionally not wrapped.
        // Channel X register = channel * 0x1000 + 0xNNN (X is the channel number, 1..4).

        // Holding 0xX460 + p, u16, CT transformation ratio for the phase, no scaling.
        // 0 = disabled (default). FW 2.1.
        std::pair<uint16_t, bool> getCTTransformRatio(uint8_t channel, MAP12EPhase phase) const;
        bool setCTTransformRatio(uint8_t channel, MAP12EPhase phase, uint16_t ratio) const;

        // Holding 0xX463 + p, s16, x0.001 deg, -32768..32767 raw (+-32.767 deg). FW 2.1.
        std::pair<float_t, bool> getCTPhaseDelay(uint8_t channel, MAP12EPhase phase) const;
        bool setCTPhaseDelay(uint8_t channel, MAP12EPhase phase, float_t degrees) const;

        // Holding 0xX4A0 + (ctInput - 1), u16. ctInput is the physical CT input 1..3.
        // The value is the measured phase: 1 = L1, 2 = L2, 3 = L3
        // (1-based, unlike the 0-based MAP12EPhase). Defaults 1/2/3. FW 2.3.2.
        std::pair<uint16_t, bool> getCTInputPhase(uint8_t channel, uint8_t ctInput) const;
        bool setCTInputPhase(uint8_t channel, uint8_t ctInput, uint16_t phase) const;

        // Holding 0xX4B0 + p, bool, invert the CT for the phase. FW 2.10.0.
        std::pair<bool, bool> getCTInvert(uint8_t channel, MAP12EPhase phase) const;
        bool setCTInvert(uint8_t channel, MAP12EPhase phase, bool invert) const;

        // Holding 0x10F0, u16, peak aggregation period in seconds, 1..65535 (default 60). FW 2.3.0.
        std::pair<uint16_t, bool> getPeakAggregationPeriod() const;
        bool setPeakAggregationPeriod(uint16_t seconds) const;

        // Holding 0x10F4, u16, current peak detect period in milliseconds, 1..255 (default 20). FW 2.13.0.
        std::pair<uint16_t, bool> getPeakDetectPeriod() const;
        bool setPeakDetectPeriod(uint16_t milliseconds) const;

        // Holding 0x0066, u8, detect phase breaks ERRMAP0003, 0/1 (default 0). FW 2.6.3.
        std::pair<bool, bool> getPhaseLossDetection() const;
        bool setPhaseLossDetection(bool enable) const;

    private:
        bool validateChannel(uint8_t channel) const;
        bool validatePhase(MAP12EPhase phase) const;
        bool validateLine(MAP12ELine line) const;

        std::pair<uint16_t, bool> readInputU16(uint16_t reg) const;
        std::pair<int16_t, bool> readInputS16(uint16_t reg) const;
        std::pair<uint32_t, bool> readInputU32BE(uint16_t reg) const;
        std::pair<int32_t, bool> readInputS32BE(uint16_t reg) const;
        std::pair<int32_t, bool> readInputS32LE(uint16_t reg) const;
        std::pair<uint64_t, bool> readInputU64LE(uint16_t reg) const;

        std::pair<uint16_t, bool> readHoldingU16(uint16_t reg) const;
        bool writeHoldingU16(uint16_t reg, uint16_t value) const;
        bool writeHoldingS16(uint16_t reg, int16_t value) const;
    };
}
