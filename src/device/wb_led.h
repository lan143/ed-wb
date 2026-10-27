#pragma once

#include <iarduino_Modbus.h>
#include "wb_device.h"

namespace EDWB
{
    enum LEDMode : uint16_t
    {
        LED_MODE_WWWW = 0,
        LED_MODE_2WWW = 1,
        LED_MODE_CCTWW = 2,
        LED_MODE_WW2W = 16,
        LED_MODE_2W2W = 17,
        LED_MODE_CCT2W = 18,
        LED_MODE_WWCCT = 32,
        LED_MODE_2WCCT = 33,
        LED_MODE_CCTCCT = 34,
        LED_MODE_RGBW = 256,
        LED_MODE_4W = 512
    };

    enum SafeMode : uint16_t
    {
        SAFE_MODE_DONT_BLOCK_INPUT = 0,
        SAFE_MODE_INPUT_BLOCK_IN = 1,
        SAFE_MODE_DONT_BLOCK_IN = 2
    };

    enum InputType : uint8_t
    {
        INPUT_TYPE_SHORT_CLICK = 0,
        INPUT_TYPE_LONG_CLICK = 1,
        INPUT_TYPE_DOUBLE_CLICK = 2,
        INPUT_TYPE_SHORT_AND_LONG_CLICK = 3
    };

    enum RemoteChangeAction : uint16_t
    {
        REMOTE_CHANGE_STOP = 0,
        REMOTE_CHANGE_DECREASE = 1,
        REMOTE_CHANGE_INCREASE = 2,
        REMOTE_CHANGE_INCREASE_DECREASE = 3
    };

    class LED : public WBDevice
    {
    public:
        LED(ModbusClient& client) : WBDevice(client) {}

        Result<bool> isChannelEnabled(uint8_t channel) const;
        bool enableChannel(uint8_t channel, bool enabled);

        Result<bool> isPairChannelsEnabled(uint8_t pair) const;
        bool enablePairChannels(uint8_t pair, bool enabled);

        Result<bool> isAllChannelsEnabled() const;
        bool enableAllChannels(bool enabled);

        Result<bool> isEnabledCCT1() const;
        bool enableCCT1(bool enabled);

        Result<bool> isEnabledCCT2() const;
        bool enableCCT2(bool enabled);

        Result<bool> isEnabledCCT1CCT2() const;
        bool enableCCT1CCT2(bool enabled);

        Result<bool> isEnabledRGB() const;
        bool enableRGB(bool enabled);

        Result<bool> isContinuousRGBChangeEnabled() const;
        bool enableContinuousRGBChange(bool enabled);

        // Input states and press counters
        Result<bool> getInputState(uint8_t input) const;
        Result<uint16_t> getInputCounter(uint8_t input) const;
        Result<uint16_t> getShortPressCount(uint8_t input) const;
        Result<uint16_t> getLongPressCount(uint8_t input) const;
        Result<uint16_t> getDoublePressCount(uint8_t input) const;
        Result<uint16_t> getShortThenLongPressCount(uint8_t input) const;

        Result<uint8_t> getChannelBrightness(uint8_t channel) const;
        bool setChannelBrightness(uint8_t channel, uint8_t brightness);

        Result<uint8_t> getPairChannelsBrightness(uint8_t pair) const;
        bool setPairChannelsBrightness(uint8_t pair, uint8_t brightness);

        Result<uint8_t> getAllChannelsBrightness() const;
        bool setAllChannelsBrightness(uint8_t brightness);

        Result<uint8_t> getTemperatureCCT1() const;
        bool setTemperatureCCT1(uint8_t temperature);

        Result<uint8_t> getBrightnessCCT1() const;
        bool setBrightnessCCT1(uint8_t brightness);

        Result<uint8_t> getTemperatureCCT2() const;
        bool setTemperatureCCT2(uint8_t temperature);

        Result<uint8_t> getBrightnessCCT2() const;
        bool setBrightnessCCT2(uint8_t brightness);

        Result<uint8_t> getTemperatureCCT1CCT2() const;
        bool setTemperatureCCT1CCT2(uint8_t temperature);

        Result<uint8_t> getBrightnessCCT1CCT2() const;
        bool setBrightnessCCT1CCT2(uint8_t brightness);

        Result<uint32_t> getRGBColor() const;
        bool setRGBColor(uint32_t color);

        // HSV components and RGB continuous change speed
        Result<uint16_t> getHue() const;
        bool setHue(uint16_t hue);

        Result<uint8_t> getSaturation() const;
        bool setSaturation(uint8_t saturation);

        Result<uint8_t> getValue() const;
        bool setValue(uint8_t value);

        Result<uint16_t> getRGBChangeSpeed() const;
        bool setRGBChangeSpeed(uint16_t periodMs);

        // Duplicate control registers with auto-enable, FW 3.8.0
        bool setChannelBrightnessAutoOn(uint8_t channel, uint8_t brightness);
        bool setPairChannelsBrightnessAutoOn(uint8_t pair, uint8_t brightness);
        bool setAllChannelsBrightnessAutoOn(uint8_t brightness);
        bool setTemperatureCCT1AutoOn(uint8_t temperature);
        bool setBrightnessCCT1AutoOn(uint8_t brightness);
        bool setTemperatureCCT2AutoOn(uint8_t temperature);
        bool setBrightnessCCT2AutoOn(uint8_t brightness);
        bool setRGBColorAutoOn(uint32_t color);
        bool setHueAutoOn(uint16_t hue);
        bool setSaturationAutoOn(uint8_t saturation);
        bool setValueAutoOn(uint8_t value);
        bool setRGBChangeSpeedAutoOn(uint16_t periodMs);
        bool setTemperatureCCT1CCT2AutoOn(uint8_t temperature);
        bool setBrightnessCCT1CCT2AutoOn(uint8_t brightness);

        // Remote smooth change, FW 3.8.0
        Result<RemoteChangeAction> getRemoteChangeAction(uint8_t param) const;
        bool setRemoteChangeAction(uint8_t param, RemoteChangeAction action);
        Result<uint16_t> getRemoteChangePeriod(uint8_t param) const;
        bool setRemoteChangePeriod(uint8_t param, uint16_t periodMs);

        Result<LEDMode> getMode() const;
        bool setMode(LEDMode mode);

        bool setInputMode(uint8_t channel, bool isButton) const;

        bool setSafeMode(uint8_t channel, SafeMode mode) const;

        bool setInputActionRaw(uint8_t channel, InputType type, uint16_t value) const;
    };
}
