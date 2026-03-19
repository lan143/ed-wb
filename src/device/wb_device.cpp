#include <log/log.h>

#include "wb_device.h"

using namespace EDWB;

void WBDevice::init(uint8_t address)
{
    Device::init(address);

    auto version = getFWVersion();
    if (version.second == WBDeviceErrorNone) {
        _fwVersion = version.first;
    } else {
        LOGE("init", "failed to load firmware version");
    }
};

std::pair<uint32_t, WBDeviceError> WBDevice::getSpeed()
{
    auto val = _client.holdingRegisterRead(_address, 0x006E);
    if (val == -1) {
        return std::make_pair(0, WBDeviceErrorModbus);
    }

    return std::make_pair(uint32_t(val) * 100, WBDeviceErrorNone);
}

WBDeviceError WBDevice::setSpeed(uint32_t speed)
{
    auto val = speed / 100;
    switch (val) {
        case 12:
        case 24:
        case 48:
        case 96:
        case 192:
        case 384:
        case 576:
        case 1152:
            break;
        default:
            return WBDeviceErrorUnknownSpeed;
    }

    return _client.holdingRegisterWrite(_address, 0x006E, val) > 0 ? WBDeviceErrorNone : WBDeviceErrorModbus;
}

std::pair<ParityBit, WBDeviceError> WBDevice::getParityBit()
{
    auto val = _client.holdingRegisterRead(_address, 0x006F);
    if (val == -1) {
        return std::make_pair(ParityBitNone, WBDeviceErrorModbus);
    }

    return std::make_pair(ParityBit(val), WBDeviceErrorNone);
}

WBDeviceError WBDevice::setParityBit(ParityBit parityBit)
{
    switch (parityBit) {
        case ParityBitNone:
        case ParityBitOdd:
        case ParityBitEven:
            break;
        default:
            return WBDeviceErrorUnknownParityBit;
    }

    return _client.holdingRegisterWrite(_address, 0x006F, parityBit) > 0 ? WBDeviceErrorNone : WBDeviceErrorModbus;
}

std::pair<uint8_t, WBDeviceError> WBDevice::getStopBits()
{
    auto val = _client.holdingRegisterRead(_address, 0x0070);
    if (val == -1) {
        return std::make_pair(ParityBitNone, WBDeviceErrorModbus);
    }

    return std::make_pair(val, WBDeviceErrorNone);
}

WBDeviceError WBDevice::setStopBits(uint8_t stopBits)
{
    if (stopBits < 1 || stopBits > 2) {
        return WBDeviceErrorUnknownStopBits;
    }

    return _client.holdingRegisterWrite(_address, 0x0070, stopBits) > 0 ? WBDeviceErrorNone : WBDeviceErrorModbus;
}

std::pair<uint8_t, WBDeviceError> WBDevice::getAddress()
{
    auto val = _client.holdingRegisterRead(_address, 0x0080);
    if (val == -1) {
        return std::make_pair(ParityBitNone, WBDeviceErrorModbus);
    }

    return std::make_pair(val, WBDeviceErrorNone);
}

WBDeviceError WBDevice::setAddress(uint8_t address)
{
    if (address < 1 || address > 254) {
        return WBDeviceErrorUnknownAddress;
    }

    return _client.holdingRegisterWrite(_address, 0x0080, address) > 0 ? WBDeviceErrorNone : WBDeviceErrorModbus;
}

std::pair<std::string, WBDeviceError> WBDevice::getModel()
{
    std::string name;
    name.reserve(10);

    for (uint16_t i = 0; i < 5; i++) {
        auto val = _client.inputRegisterRead(_address, 0x00C8 + i);
        if (val == -1) {
            return std::make_pair("", WBDeviceErrorModbus);
        }

        if (!val) {
            break;
        }

        name += (char)val;
    }

    return std::make_pair(name, WBDeviceErrorNone);
}

std::pair<std::string, WBDeviceError> WBDevice::getFWVersion()
{
    std::string version;
    version.reserve(30);

    for (uint16_t i = 0; i < 15; i++) {
        auto val = _client.inputRegisterRead(_address, 0x00FA + i);
        if (val == -1) {
            return std::make_pair("", WBDeviceErrorModbus);
        }

        if (!val) {
            break;
        }

        version += (char)val;
    }

    return std::make_pair(version, WBDeviceErrorNone);
}
