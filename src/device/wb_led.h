#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

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

    class LED : public Device
    {
    public:
        LED(ModbusClient& client) : Device(client) {}

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

        Result<bool> isEnabledRGB() const;
        bool enableRGB(bool enabled);

        Result<bool> isContinuousRGBChangeEnabled() const;
        bool enableContinuousRGBChange(bool enabled);

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

        Result<uint32_t> getRGBColor() const;
        bool setRGBColor(uint32_t color);

        bool setMode(LEDMode mode);

        bool setInputMode(uint8_t channel, bool isButton) const;

        bool setSafeMode(uint8_t channel, SafeMode mode) const;

        bool setInputActionRaw(uint8_t channel, InputType type, uint16_t value) const;
    };
}
