#include "wb_led.h"

using namespace EDWB;

bool LED::isChannelEnabled(uint8_t channel) const
{
    channel--;
    if (channel > 3) {
        return false;
    }

    return _client.coilRead(_address, channel) == 1;
}

void LED::enableChannel(uint8_t channel, bool enabled)
{
    channel--;
    if (channel > 3) {
        return;
    }

    _client.coilWrite(_address, channel, enabled ? 1 : 0);
}

bool LED::isPairChannelsEnabled(uint8_t pair) const
{
    pair--;
    if (pair > 1) {
        return false;
    }

    return _client.coilRead(_address, 0x0004 + pair) == 1;
}

void LED::enablePairChannels(uint8_t pair, bool enabled)
{
    pair--;
    if (pair > 1) {
        return;
    }

    _client.coilWrite(_address, 0x0004 + pair, enabled ? 1 : 0);
}

bool LED::isAllChannelsEnabled() const
{
    return _client.coilRead(_address, 0x0006) == 1;
}

void LED::enableAllChannels(bool enabled)
{
    _client.coilWrite(_address, 0x0006, enabled ? 1 : 0);
}

bool LED::isEnabledCCT1() const
{
    return _client.coilRead(_address, 0x0007) == 1;
}

void LED::enableCCT1(bool enabled)
{
    _client.coilWrite(_address, 0x0007, enabled ? 1 : 0);
}

bool LED::isEnabledCCT2() const
{
    return _client.coilRead(_address, 0x0008) == 1;
}

void LED::enableCCT2(bool enabled)
{
    _client.coilWrite(_address, 0x0008, enabled ? 1 : 0);
}

bool LED::isEnabledRGB() const
{
    return _client.coilRead(_address, 0x0009) == 1;
}

void LED::enableRGB(bool enabled)
{
    _client.coilWrite(_address, 0x0009, enabled ? 1 : 0);
}

bool LED::isContinuousRGBChangeEnabled() const
{
    return _client.coilRead(_address, 0x000A) == 1;
}

void LED::enableContinuousRGBChange(bool enabled)
{
    _client.coilWrite(_address, 0x000A, enabled ? 1 : 0);
}

uint8_t LED::getChannelBrightness(uint8_t channel) const
{
    channel--;
    if (channel > 3) {
        return 0;
    }

    return _client.holdingRegisterRead(_address, 0x07D0 + channel);
}

void LED::setChannelBrightness(uint8_t channel, uint8_t brightness)
{
    channel--;
    if (channel > 3) {
        return;
    }

    if (brightness > 100) {
        brightness = 100;
    }

    _client.holdingRegisterWrite(_address, 0x07D0 + channel, brightness);
}

uint8_t LED::getPairChannelsBrightness(uint8_t pair) const
{
    pair--;
    if (pair > 1) {
        return 0;
    }

    return _client.holdingRegisterRead(_address, 0x07D4 + pair);
}

void LED::setPairChannelsBrightness(uint8_t pair, uint8_t brightness)
{
    pair--;
    if (pair > 1) {
        return;
    }

    if (brightness > 100) {
        brightness = 100;
    }

    _client.holdingRegisterWrite(_address, 0x07D4 + pair, brightness);
}

uint8_t LED::getAllChannelsBrightness() const
{
    return _client.holdingRegisterRead(_address, 0x07D6);
}

void LED::setAllChannelsBrightness(uint8_t brightness)
{
    if (brightness > 100) {
        brightness = 100;
    }

    _client.holdingRegisterWrite(_address, 0x07D6, brightness);
}

uint8_t LED::getTemperatureCCT1() const
{
    return _client.holdingRegisterRead(_address, 0x07D7);
}

void LED::setTemperatureCCT1(uint8_t temperature)
{
    if (temperature > 100) {
        temperature = 100;
    }

    _client.holdingRegisterWrite(_address, 0x07D7, temperature);
}

uint8_t LED::getBrightnessCCT1() const
{
    return _client.holdingRegisterRead(_address, 0x07D8);
}

void LED::setBrightnessCCT1(uint8_t brightness)
{
    if (brightness > 100) {
        brightness = 100;
    }

    _client.holdingRegisterWrite(_address, 0x07D8, brightness);
}

uint8_t LED::getTemperatureCCT2() const
{
    return _client.holdingRegisterRead(_address, 0x07D9);
}

void LED::setTemperatureCCT2(uint8_t temperature)
{
    if (temperature > 100) {
        temperature = 100;
    }

    _client.holdingRegisterWrite(_address, 0x07D9, temperature);
}

uint8_t LED::getBrightnessCCT2() const
{
    return _client.holdingRegisterRead(_address, 0x07DA);
}

void LED::setBrightnessCCT2(uint8_t brightness)
{
    if (brightness > 100) {
        brightness = 100;
    }

    _client.holdingRegisterWrite(_address, 0x07DA, brightness);
}

uint32_t LED::getRGBColor() const
{
    uint32_t color;
    for (uint8_t i = 0; i < 3; i++) {
        uint16_t part = _client.holdingRegisterRead(_address, 0x07DB + i);

        color |= (part << (24 - i * 8));
    }

    return color;
}

void LED::setRGBColor(uint32_t color)
{
    for (uint8_t i = 0; i < 3; i++) {
        _client.holdingRegisterWrite(_address, 0x07DB + i, (color >> (24 - i * 8)) & 0xFF);
    }
}


void LED::setMode(LEDMode mode)
{
    _client.holdingRegisterWrite(_address, 0x0FA0, mode);
}
