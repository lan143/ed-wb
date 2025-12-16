#include "wb_led.h"

using namespace EDWB;

Result<bool> LED::isChannelEnabled(uint8_t channel) const
{
    channel--;
    if (channel > 3) {
        return Result<bool>(false, false);
    }

    auto val = _client.coilRead(_address, channel);
    if (val == -1) {
        return Result<bool>(false, false);
    }

    return Result<bool>(true, val == 1);
}

bool LED::enableChannel(uint8_t channel, bool enabled)
{
    channel--;
    if (channel > 3) {
        return false;
    }

    return _client.coilWrite(_address, channel, enabled ? 1 : 0);
}

Result<bool> LED::isPairChannelsEnabled(uint8_t pair) const
{
    pair--;
    if (pair > 1) {
        return Result<bool>(false, false);
    }

    auto val = _client.coilRead(_address, 0x0004 + pair);
    if (val == -1) {
        return Result<bool>(false, false);
    }

    return Result<bool>(true, val == 1);
}

bool LED::enablePairChannels(uint8_t pair, bool enabled)
{
    pair--;
    if (pair > 1) {
        return false;
    }

    return _client.coilWrite(_address, 0x0004 + pair, enabled ? 1 : 0);
}

Result<bool> LED::isAllChannelsEnabled() const
{
    auto val = _client.coilRead(_address, 0x0006);
    if (val == -1) {
        return Result<bool>(false, false);
    }

    return Result<bool>(true, val == 1);
}

bool LED::enableAllChannels(bool enabled)
{
    return _client.coilWrite(_address, 0x0006, enabled ? 1 : 0);
}

Result<bool> LED::isEnabledCCT1() const
{
    int8_t result = _client.coilRead(_address, 0x0007);
    if (result == -1) {
        return Result<bool>(false, false);
    }

    return Result<bool>(true, result == 1);
}

bool LED::enableCCT1(bool enabled)
{
    return _client.coilWrite(_address, 0x0007, enabled ? 1 : 0);
}

Result<bool> LED::isEnabledCCT2() const
{
    int8_t result = _client.coilRead(_address, 0x0008);
    if (result == -1) {
        return Result<bool>(false, false);
    }

    return Result<bool>(true, result == 1);
}

bool LED::enableCCT2(bool enabled)
{
    return _client.coilWrite(_address, 0x0008);
}

Result<bool> LED::isEnabledRGB() const
{
    auto val = _client.coilRead(_address, 0x0009);
    if (val == -1) {
        return Result<bool>(false, false);
    }

    return Result<bool>(true, val == 1);
}

bool LED::enableRGB(bool enabled)
{
    return _client.coilWrite(_address, 0x0009, enabled ? 1 : 0);
}

Result<bool> LED::isContinuousRGBChangeEnabled() const
{
    auto val = _client.coilRead(_address, 0x000A);
    if (val == -1) {
        return Result<bool>(false, false);
    }

    return Result<bool>(true, val == 1);
}

bool LED::enableContinuousRGBChange(bool enabled)
{
    return _client.coilWrite(_address, 0x000A, enabled ? 1 : 0);
}

Result<uint8_t> LED::getChannelBrightness(uint8_t channel) const
{
    channel--;
    if (channel > 3) {
        return Result<uint8_t>(false, 0);
    }

    auto val = _client.holdingRegisterRead(_address, 0x07D0 + channel);
    if (val == -1) {
        return Result<uint8_t>(false, 0);
    }

    return Result<uint8_t>(true, val);
}

bool LED::setChannelBrightness(uint8_t channel, uint8_t brightness)
{
    channel--;
    if (channel > 3) {
        return false;
    }

    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x07D0 + channel, brightness);
}

Result<uint8_t> LED::getPairChannelsBrightness(uint8_t pair) const
{
    pair--;
    if (pair > 1) {
        return Result<uint8_t>(false, 0);
    }

    auto val = _client.holdingRegisterRead(_address, 0x07D4 + pair);
    if (val == -1) {
        return Result<uint8_t>(false, 0);
    }

    return Result<uint8_t>(true, val);
}

bool LED::setPairChannelsBrightness(uint8_t pair, uint8_t brightness)
{
    pair--;
    if (pair > 1) {
        return false;
    }

    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x07D4 + pair, brightness);
}

Result<uint8_t> LED::getAllChannelsBrightness() const
{
    auto val = _client.holdingRegisterRead(_address, 0x07D6);
    if (val == -1) {
        return Result<uint8_t>(false, 0);
    }

    return Result<uint8_t>(true, val);
}

bool LED::setAllChannelsBrightness(uint8_t brightness)
{
    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x07D6, brightness);
}

Result<uint8_t> LED::getTemperatureCCT1() const
{
    auto val = _client.holdingRegisterRead(_address, 0x07D7);
    if (val == -1) {
        return Result<uint8_t>(false, 0);
    }

    return Result<uint8_t>(true, val);
}

bool LED::setTemperatureCCT1(uint8_t temperature)
{
    if (temperature > 100) {
        temperature = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x07D7, temperature);
}

Result<uint8_t> LED::getBrightnessCCT1() const
{
    auto val = _client.holdingRegisterRead(_address, 0x07D8);
    if (val == -1) {
        return Result<uint8_t>(false, 0);
    }

    return Result<uint8_t>(true, val);
}

bool LED::setBrightnessCCT1(uint8_t brightness)
{
    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x07D8, brightness);
}

Result<uint8_t> LED::getTemperatureCCT2() const
{
    auto val = _client.holdingRegisterRead(_address, 0x07D9);
    if (val == -1) {
        return Result<uint8_t>(false, 0);
    }

    return Result<uint8_t>(true, val);
}

bool LED::setTemperatureCCT2(uint8_t temperature)
{
    if (temperature > 100) {
        temperature = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x07D9, temperature);
}

Result<uint8_t> LED::getBrightnessCCT2() const
{
    auto val = _client.holdingRegisterRead(_address, 0x07DA);
    if (val == -1) {
        return Result<uint8_t>(false, 0);
    }

    return Result<uint8_t>(true, val);
}

bool LED::setBrightnessCCT2(uint8_t brightness)
{
    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x07DA, brightness);
}

Result<uint32_t> LED::getRGBColor() const
{
    uint32_t color;
    for (uint8_t i = 0; i < 3; i++) {
        uint16_t part = _client.holdingRegisterRead(_address, 0x07DB + i);
        if (part == -1) {
            return Result<uint32_t>(false, 0);
        }

        color |= (part << (16 - i * 8));
    }

    return Result<uint32_t>(true, color);
}

bool LED::setRGBColor(uint32_t color)
{
    for (uint8_t i = 0; i < 3; i++) {
        auto result = _client.holdingRegisterWrite(_address, 0x07DB + i, (color >> (16 - i * 8)) & 0xFF);
        if (!result) {
            return false;
        }
    }

    return true;
}

bool LED::setMode(LEDMode mode)
{
    return _client.holdingRegisterWrite(_address, 0x0FA0, mode);
}

bool LED::setInputMode(uint8_t channel, bool isButton) const
{
    channel--;
    if (channel > 3) {
        return false;
    }

    return _client.holdingRegisterWrite(_address, 0x01A0 + channel, isButton ? 0 : 1);
}

bool LED::setSafeMode(uint8_t channel, SafeMode mode) const
{
    channel--;
    if (channel > 3) {
        return false;
    }

    return _client.holdingRegisterWrite(_address, 0x0280 + channel, mode);
}


bool LED::setInputActionRaw(uint8_t channel, InputType type, uint16_t value) const
{
    uint16_t baseReg;
    switch (type) {
        case INPUT_TYPE_SHORT_CLICK:
            baseReg = 0x03E8;
            break;
        case INPUT_TYPE_LONG_CLICK:
            baseReg = 0x03FC;
            break;
        case INPUT_TYPE_DOUBLE_CLICK:
            baseReg = 0x0410;
            break;
        case INPUT_TYPE_SHORT_AND_LONG_CLICK:
            baseReg = 0x0424;
            break;
        default:
            return false;
    }

    channel--;
    if (channel > 3) {
        return false;
    }

    return _client.holdingRegisterWrite(_address, baseReg + channel, value);
}
