#pragma once

#include <Arduino.h>
#include <version/version.h>

#include "device.h"

namespace EDWB
{
    enum ParityBit : uint8_t
    {
        ParityBitNone,
        ParityBitOdd,
        ParityBitEven
    };

    enum WBDeviceError : uint8_t
    {
        WBDeviceErrorNone,
        WBDeviceErrorModbus,
        WBDeviceErrorUnknownSpeed,
        WBDeviceErrorUnknownParityBit,
        WBDeviceErrorUnknownStopBits,
        WBDeviceErrorUnknownAddress
    };

    class WBDevice : public Device
    {
    public:
        WBDevice(ModbusClient& client) : Device(client) {}

        void init(uint8_t address);

        std::pair<uint32_t, WBDeviceError> getSpeed();
        WBDeviceError setSpeed(uint32_t speed);

        std::pair<ParityBit, WBDeviceError> getParityBit();
        WBDeviceError setParityBit(ParityBit parityBit);

        std::pair<uint8_t, WBDeviceError> getStopBits();
        WBDeviceError setStopBits(uint8_t stopBits);

        std::pair<uint8_t, WBDeviceError> getAddress();
        WBDeviceError setAddress(uint8_t address);

        std::pair<std::string, WBDeviceError> getModel();
        std::pair<std::string, WBDeviceError> getFWVersion();

    protected:
        Version _fwVersion;
    };
}
