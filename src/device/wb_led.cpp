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
    return _client.coilWrite(_address, 0x0008, enabled ? 1 : 0);
}

Result<bool> LED::isEnabledCCT1CCT2() const
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return Result<bool>(false, false);
    }

    int8_t result = _client.coilRead(_address, 0x000B);
    if (result == -1) {
        return Result<bool>(false, false);
    }

    return Result<bool>(true, result == 1);
}

bool LED::enableCCT1CCT2(bool enabled)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    return _client.coilWrite(_address, 0x000B, enabled ? 1 : 0);
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

Result<bool> LED::getInputState(uint8_t input) const
{
    input--;
    if (input > 3) {
        return Result<bool>(false, false);
    }

    auto val = _client.discreteInputRead(_address, 0x0000 + input);
    if (val == -1) {
        return Result<bool>(false, false);
    }

    return Result<bool>(true, val == 1);
}

Result<uint16_t> LED::getInputCounter(uint8_t input) const
{
    input--;
    if (input > 3) {
        return Result<uint16_t>(false, 0);
    }

    auto val = _client.inputRegisterRead(_address, 0x0020 + input);
    if (val == -1) {
        return Result<uint16_t>(false, 0);
    }

    return Result<uint16_t>(true, val);
}

Result<uint16_t> LED::getShortPressCount(uint8_t input) const
{
    input--;
    if (input > 3) {
        return Result<uint16_t>(false, 0);
    }

    auto val = _client.inputRegisterRead(_address, 0x01D0 + input);
    if (val == -1) {
        return Result<uint16_t>(false, 0);
    }

    return Result<uint16_t>(true, val);
}

Result<uint16_t> LED::getLongPressCount(uint8_t input) const
{
    input--;
    if (input > 3) {
        return Result<uint16_t>(false, 0);
    }

    auto val = _client.inputRegisterRead(_address, 0x01E0 + input);
    if (val == -1) {
        return Result<uint16_t>(false, 0);
    }

    return Result<uint16_t>(true, val);
}

Result<uint16_t> LED::getDoublePressCount(uint8_t input) const
{
    input--;
    if (input > 3) {
        return Result<uint16_t>(false, 0);
    }

    auto val = _client.inputRegisterRead(_address, 0x01F0 + input);
    if (val == -1) {
        return Result<uint16_t>(false, 0);
    }

    return Result<uint16_t>(true, val);
}

Result<uint16_t> LED::getShortThenLongPressCount(uint8_t input) const
{
    input--;
    if (input > 3) {
        return Result<uint16_t>(false, 0);
    }

    auto val = _client.inputRegisterRead(_address, 0x0200 + input);
    if (val == -1) {
        return Result<uint16_t>(false, 0);
    }

    return Result<uint16_t>(true, val);
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

Result<uint8_t> LED::getTemperatureCCT1CCT2() const
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return Result<uint8_t>(false, 0);
    }

    auto val = _client.holdingRegisterRead(_address, 0x07E2);
    if (val == -1) {
        return Result<uint8_t>(false, 0);
    }

    return Result<uint8_t>(true, val);
}

bool LED::setTemperatureCCT1CCT2(uint8_t temperature)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (temperature > 100) {
        temperature = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x07E2, temperature);
}

Result<uint8_t> LED::getBrightnessCCT1CCT2() const
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return Result<uint8_t>(false, 0);
    }

    auto val = _client.holdingRegisterRead(_address, 0x07E3);
    if (val == -1) {
        return Result<uint8_t>(false, 0);
    }

    return Result<uint8_t>(true, val);
}

bool LED::setBrightnessCCT1CCT2(uint8_t brightness)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x07E3, brightness);
}

Result<uint32_t> LED::getRGBColor() const
{
    uint32_t color = 0;
    for (uint8_t i = 0; i < 3; i++) {
        int32_t part = _client.holdingRegisterRead(_address, 0x07DB + i);
        if (part == -1) {
            return Result<uint32_t>(false, 0);
        }

        color |= (uint32_t)(part & 0xFF) << (16 - i * 8);
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

Result<uint16_t> LED::getHue() const
{
    auto val = _client.holdingRegisterRead(_address, 0x07DE);
    if (val == -1) {
        return Result<uint16_t>(false, 0);
    }

    return Result<uint16_t>(true, val);
}

bool LED::setHue(uint16_t hue)
{
    if (hue > 360) {
        hue = 360;
    }

    return _client.holdingRegisterWrite(_address, 0x07DE, hue);
}

Result<uint8_t> LED::getSaturation() const
{
    auto val = _client.holdingRegisterRead(_address, 0x07DF);
    if (val == -1) {
        return Result<uint8_t>(false, 0);
    }

    return Result<uint8_t>(true, val);
}

bool LED::setSaturation(uint8_t saturation)
{
    if (saturation > 100) {
        saturation = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x07DF, saturation);
}

// V component of the HSV palette (RGB strip brightness)
Result<uint8_t> LED::getValue() const
{
    auto val = _client.holdingRegisterRead(_address, 0x07E0);
    if (val == -1) {
        return Result<uint8_t>(false, 0);
    }

    return Result<uint8_t>(true, val);
}

bool LED::setValue(uint8_t value)
{
    if (value > 100) {
        value = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x07E0, value);
}

Result<uint16_t> LED::getRGBChangeSpeed() const
{
    auto val = _client.holdingRegisterRead(_address, 0x07E1);
    if (val == -1) {
        return Result<uint16_t>(false, 0);
    }

    return Result<uint16_t>(true, val);
}

bool LED::setRGBChangeSpeed(uint16_t periodMs)
{
    if (periodMs < 3) {
        periodMs = 3;
    } else if (periodMs > 10000) {
        periodMs = 10000;
    }

    return _client.holdingRegisterWrite(_address, 0x07E1, periodMs);
}

bool LED::setMode(LEDMode mode)
{
    return _client.holdingRegisterWrite(_address, 0x0FA0, mode);
}

Result<LEDMode> LED::getMode() const
{
    auto val = _client.holdingRegisterRead(_address, 0x0FA0);
    if (val == -1) {
        return Result<LEDMode>(false, LED_MODE_RGBW);
    }

    return Result<LEDMode>(true, LEDMode(val));
}

bool LED::setInputMode(uint8_t channel, bool isButton) const
{
    if (!_inited || _fwVersion < Version{3, 5, 0}) {
        return false;
    }

    channel--;
    if (channel > 3) {
        return false;
    }

    return _client.holdingRegisterWrite(_address, 0x01A0 + channel, isButton ? 0 : 1);
}

bool LED::setSafeMode(uint8_t channel, SafeMode mode) const
{
    if (!_inited || _fwVersion < Version{3, 5, 0}) {
        return false;
    }

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

bool LED::setChannelBrightnessAutoOn(uint8_t channel, uint8_t brightness)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    channel--;
    if (channel > 3) {
        return false;
    }

    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x0834 + channel, brightness);
}

bool LED::setPairChannelsBrightnessAutoOn(uint8_t pair, uint8_t brightness)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    pair--;
    if (pair > 1) {
        return false;
    }

    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x0838 + pair, brightness);
}

bool LED::setAllChannelsBrightnessAutoOn(uint8_t brightness)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x083A, brightness);
}

bool LED::setTemperatureCCT1AutoOn(uint8_t temperature)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (temperature > 100) {
        temperature = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x083B, temperature);
}

bool LED::setBrightnessCCT1AutoOn(uint8_t brightness)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x083C, brightness);
}

bool LED::setTemperatureCCT2AutoOn(uint8_t temperature)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (temperature > 100) {
        temperature = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x083D, temperature);
}

bool LED::setBrightnessCCT2AutoOn(uint8_t brightness)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x083E, brightness);
}

bool LED::setRGBColorAutoOn(uint32_t color)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    for (uint8_t i = 0; i < 3; i++) {
        auto result = _client.holdingRegisterWrite(_address, 0x083F + i, (color >> (16 - i * 8)) & 0xFF);
        if (!result) {
            return false;
        }
    }

    return true;
}

bool LED::setHueAutoOn(uint16_t hue)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (hue > 360) {
        hue = 360;
    }

    return _client.holdingRegisterWrite(_address, 0x0842, hue);
}

bool LED::setSaturationAutoOn(uint8_t saturation)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (saturation > 100) {
        saturation = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x0843, saturation);
}

bool LED::setValueAutoOn(uint8_t value)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (value > 100) {
        value = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x0844, value);
}

bool LED::setRGBChangeSpeedAutoOn(uint16_t periodMs)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (periodMs < 3) {
        periodMs = 3;
    } else if (periodMs > 10000) {
        periodMs = 10000;
    }

    return _client.holdingRegisterWrite(_address, 0x0845, periodMs);
}

bool LED::setTemperatureCCT1CCT2AutoOn(uint8_t temperature)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (temperature > 100) {
        temperature = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x0846, temperature);
}

bool LED::setBrightnessCCT1CCT2AutoOn(uint8_t brightness)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (brightness > 100) {
        brightness = 100;
    }

    return _client.holdingRegisterWrite(_address, 0x0847, brightness);
}

Result<RemoteChangeAction> LED::getRemoteChangeAction(uint8_t param) const
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return Result<RemoteChangeAction>(false, REMOTE_CHANGE_STOP);
    }

    if (param > 19) {
        return Result<RemoteChangeAction>(false, REMOTE_CHANGE_STOP);
    }

    auto val = _client.holdingRegisterRead(_address, 0x0898 + param);
    if (val == -1) {
        return Result<RemoteChangeAction>(false, REMOTE_CHANGE_STOP);
    }

    return Result<RemoteChangeAction>(true, RemoteChangeAction(val));
}

bool LED::setRemoteChangeAction(uint8_t param, RemoteChangeAction action)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (param > 19 || action > REMOTE_CHANGE_INCREASE_DECREASE) {
        return false;
    }

    return _client.holdingRegisterWrite(_address, 0x0898 + param, action);
}

Result<uint16_t> LED::getRemoteChangePeriod(uint8_t param) const
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return Result<uint16_t>(false, 0);
    }

    if (param > 19) {
        return Result<uint16_t>(false, 0);
    }

    auto val = _client.holdingRegisterRead(_address, 0x08FC + param);
    if (val == -1) {
        return Result<uint16_t>(false, 0);
    }

    return Result<uint16_t>(true, val);
}

bool LED::setRemoteChangePeriod(uint8_t param, uint16_t periodMs)
{
    if (!_inited || _fwVersion < Version{3, 8, 0}) {
        return false;
    }

    if (param > 19) {
        return false;
    }

    if (periodMs < 5) {
        periodMs = 5;
    } else if (periodMs > 500) {
        periodMs = 500;
    }

    return _client.holdingRegisterWrite(_address, 0x08FC + param, periodMs);
}
