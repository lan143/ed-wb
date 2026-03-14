#include "wb_mai6.h"

using namespace EDWB;

std::pair<uint16_t, bool> MAI6::getSensorTypeP(uint8_t channel)
{
    if (!validateChannel(channel)) {
        return std::make_pair(0, false);
    }

    auto reg = 0x1000 * channel + 0x400;
    auto val = _client.holdingRegisterRead(_address, reg);
    if (val == -1) {
        return std::make_pair(0, false);
    }

    return std::make_pair(val, true);
}

bool MAI6::setSensorTypeP(uint8_t channel, uint16_t type)
{
    if (!validateChannel(channel)) {
        return false;
    }

    auto reg = 0x1000 * channel + 0x400;

    return _client.holdingRegisterWrite(_address, reg, type);
}

std::pair<uint16_t, bool> MAI6::getSensorTypeN(uint8_t channel)
{
    if (!validateChannel(channel)) {
        return std::make_pair(0, false);
    }

    auto reg = 0x1000 * channel + 0x401;
    auto val = _client.holdingRegisterRead(_address, reg);
    if (val == -1) {
        return std::make_pair(0, false);
    }

    return std::make_pair(val, true);
}

bool MAI6::setSensorTypeN(uint8_t channel, uint16_t type)
{
    if (!validateChannel(channel)) {
        return false;
    }

    auto reg = 0x1000 * channel + 0x401;

    return _client.holdingRegisterWrite(_address, reg, type);
}

bool MAI6::setMinCalculatedValueP(uint8_t channel, uint16_t value)
{
    if (!validateChannel(channel)) {
        return false;
    }

    auto reg = 0x1000 * channel + 0x408;

    return _client.holdingRegisterWrite(_address, reg, value);
}

bool MAI6::setMinCalculatedValueN(uint8_t channel, uint16_t value)
{
    if (!validateChannel(channel)) {
        return false;
    }

    auto reg = 0x1000 * channel + 0x409;

    return _client.holdingRegisterWrite(_address, reg, value);
}

bool MAI6::setMaxCalculatedValueP(uint8_t channel, uint16_t value)
{
    if (!validateChannel(channel)) {
        return false;
    }

    auto reg = 0x1000 * channel + 0x40A;

    return _client.holdingRegisterWrite(_address, reg, value);
}

bool MAI6::setMaxCalculatedValueN(uint8_t channel, uint16_t value)
{
    if (!validateChannel(channel)) {
        return false;
    }

    auto reg = 0x1000 * channel + 0x40B;

    return _client.holdingRegisterWrite(_address, reg, value);
}

std::pair<uint32_t, bool> MAI6::getMeasurementP(uint8_t channel)
{
    if (!validateChannel(channel)) {
        return std::make_pair(0, false);
    }

    uint32_t result;
    auto reg = 0x1000 * channel + 0x500;

    for (uint8_t i = 0; i < 2; i++) {
        auto val = _client.inputRegisterRead(_address, reg + i);
        if (val == -1) {
            return std::make_pair(0, false);
        }

        result |= val << (16 - i * 16);
    }

    return std::make_pair(result, true);
}

std::pair<uint32_t, bool> MAI6::getMeasurementN(uint8_t channel)
{
    if (!validateChannel(channel)) {
        return std::make_pair(0, false);
    }

    uint32_t result;
    auto reg = 0x1000 * channel + 0x502;

    for (uint8_t i = 0; i < 2; i++) {
        auto val = _client.inputRegisterRead(_address, reg + i);
        if (val == -1) {
            return std::make_pair(0, false);
        }

        result |= val << (16 - i * 16);
    }

    return std::make_pair(result, true);
}

std::pair<uint16_t, bool> MAI6::getCalculatedValueP(uint8_t channel)
{
    if (!validateChannel(channel)) {
        return std::make_pair(0, false);
    }

    auto reg = 0x1000 * channel + 0x504;
    auto val = _client.inputRegisterRead(_address, reg);
    if (val == -1) {
        return std::make_pair(0, false);
    }

    return std::make_pair(val, true);
}

std::pair<uint16_t, bool> MAI6::getCalculatedValueN(uint8_t channel)
{
    if (!validateChannel(channel)) {
        return std::make_pair(0, false);
    }

    auto reg = 0x1000 * channel + 0x505;
    auto val = _client.inputRegisterRead(_address, reg);
    if (val == -1) {
        return std::make_pair(0, false);
    }

    return std::make_pair(val, true);
}
